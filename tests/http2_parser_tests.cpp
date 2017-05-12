#include <gtest/gtest.h>
#include <functional>
#include <list>

namespace tdd
{

template <typename http2> class body;
template <typename http2> class header;

template <typename T>
class state
{
public:
	using type = T;
};

template <typename http>
class body : public state<body<http>>
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

template <typename http>
class header : public state<header<http>>
{
public:
	using self_type = header;

	void react(const char* input)
	{
		header_.append(input);
		size_t header_end = header_.find("\r\n\r\n");

		if (header_end != std::string::npos)
		{
			size_t non_header_size = header_.size() - header_end - 4;

			http* base = static_cast<http*>(this);

			base->transition(1);
			base->post(input + strlen(input) - non_header_size);
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

class http2 : public type_list<header<http2>, body<http2>>
{
public:
	using  base_type = type_list<header<http2>, body<http2>>;

	http2()
	{
	}

	void react(const char* event);

	void post(const char* event)
	{
		pending_.push_back(event);
	}

	void send(const char* event)
	{
		react(event);
	}

	void transition(int state)
	{
		state_ = state;
	}

private:
	template <int N>
	void dispatch(const char* event);

	std::list<const char*> pending_;
	int state_ = 0;
};

template <> void http2::dispatch<2>(const char* event)
{
	switch (state_)
	{
	case 0:
		using state0 = get_type<0, base_type>::type;
		this->state0::react(event);
		break;
	case 1:
		using state1 = get_type<1, base_type>::type;
		this->state1::react(event);
		break;
	};
};


void http2::react(const char* event)
{
	pending_.push_back(event);
	
	while (!pending_.empty())
	{
		event = pending_.front();
		pending_.pop_front();

		dispatch<get_size<base_type>::value>(event);
	}
}

}

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

