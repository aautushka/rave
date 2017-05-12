#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#include <list>

namespace tdd
{

template <typename http> class body;
template <typename http> class header;

template <typename machine>
class body
{
public:

	void react(const char* input)
	{
		message_.append(input);	
	}

	std::string get_message()
	{
		return message_;
	}

private:
	std::string message_;
};

template <template <typename> typename new_state, typename machine, template <typename> typename old_state>
void transition(old_state<machine>* state)
{
	auto base = static_cast<machine*>(state);
	base->template transition<new_state>();
}

template <typename machine, template <typename> typename state>
void post(state<machine>* s, const char* event)
{
	auto base = static_cast<machine*>(s);
	base->post(event);
}

template <typename machine>
class header
{
public:
	void react(const char* input)
	{
		header_.append(input);
		size_t header_end = header_.find("\r\n\r\n");

		if (header_end != std::string::npos)
		{
			size_t non_header_size = header_.size() - header_end - 4;

			auto base = static_cast<machine*>(this);

			transition<body>(this);
			post(this, input + strlen(input) - non_header_size); 
		}
	}

	std::string get_header()
	{
		return header_;
	}

private:
	std::string header_;
};

class foo{};
class bar{};

template <typename ... Ts> class type_list;

template <typename T, typename ... Ts>
class type_list<T, Ts ...> : public T, public type_list<Ts ...>
{
public:
	using head = T;
	using tail = type_list<Ts ...>;
};

template <>
struct type_list<> 
{ 
};

template <int N, typename List>
struct get_type
{
	using type = typename get_type<N - 1, typename List::tail>::type;
};

template <typename List>
struct get_type<0, List>
{
	using type = typename List::head;	
};

template <typename List>
struct get_size
{
	enum {value = 1 + get_size<typename List::tail>::value };
};

template <>
struct get_size<type_list<>>
{
	enum {value = 0};
};


template <typename T, typename List>
struct get_index
{
	enum { value = std::is_same<T, typename List::head>::value ? 0 : 1 + get_index<T, typename List::tail>::value };
};

template <typename T>
struct get_index<T, type_list<>>
{
	enum { value = 0 };
};

template <int N, typename T>
struct dispatcher
{
	void dispatch(int state, const char* event);
};

template <typename T>
struct dispatcher<0, T>
{
	static_assert(get_size<T>::value == 0, "");

	template <typename Event>	
	void dispatch(int state, Event event)
	{
	};
};

template <typename T>
struct dispatcher<1, T> : public T
{
	static_assert(get_size<T>::value == 1, "");
	using dispatcher_type = dispatcher<1, T>;

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename get_type<0, T>::type;
			this->state0::react(event);
			static_cast<state0*>(this)->react(event);
			break;
		};
	};
};

template <typename T>
struct dispatcher<2, T> : public T
{
	static_assert(get_size<T>::value == 2, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename get_type<0, T>::type;
			this->state0::react(event);
			break;
		case 1:
			using state1 = typename get_type<1, T>::type;
			this->state1::react(event);
			break;
		};
	};
};
template <typename T>
struct dispatcher<3, T> : public T
{
	static_assert(get_size<T>::value == 3, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename get_type<0, T>::type;
			this->state0::react(event);
			break;
		case 1:
			using state1 = typename get_type<1, T>::type;
			this->state1::react(event);
			break;
		case 2:
			using state2 = typename get_type<2, T>::type;
			this->state2::react(event);
			break;
		};
	}
};

template <typename T, template <class> class ... Ts>
class machine : public dispatcher<get_size<type_list<Ts<T> ...>>::value, type_list<Ts<T> ...>>
{
public:
	using base_type = type_list<Ts<T> ...>;
	using dispatcher_type = dispatcher<get_size<type_list<Ts<T> ...>>::value, type_list<Ts<T> ...>>;

	machine()
	{
	}

	void post(const char* event)
	{
		pending_.push_back(event);
	}

	void send(const char* event)
	{
		react(event);
	}

	template <template <class> class State>
	void transition()
	{
		state_ = get_index<State<T>, base_type>::value;
	}

	void react(const char* event)
	{
		pending_.push_back(event);
		
		while (!pending_.empty())
		{
			event = pending_.front();
			pending_.pop_front();

			dispatcher_type::dispatch(state_, event);
		}
	}

private:

	std::list<const char*> pending_;
	int state_ = 0;
};

class http2 : public machine<http2, header, body>
{
};

} // namespace tdd

struct http2_parser_test : public ::testing::Test
{
	tdd::http2 http;
};

TEST_F(http2_parser_test, reads_complete_header)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	
	EXPECT_EQ("GET / HTTP/1.1\r\n\r\n", http.get_header());
}

TEST_F(http2_parser_test, finds_out_header_boundary)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	http.react("this is not a header");

	EXPECT_EQ("GET / HTTP/1.1\r\n\r\n", http.get_header());
}

TEST_F(http2_parser_test, reads_http_message)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	http.react("this is a message"); 

	EXPECT_EQ("this is a message", http.get_message());
}

