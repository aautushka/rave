#include <gtest/gtest.h>
#include <functional>
#include <list>

namespace tdd
{

template <typename http> class body;
template <typename http> class header;

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

	void react(const char input)
	{
		message_.append(1, input);
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

	void react(const char* input, int size)
	{
		header_.append(input, size);
		size_t header_end = header_.find("\r\n\r\n");

		if (header_end != std::string::npos)
		{
			size_t non_header_size = header_.size() - header_end - 4;

			http* base = static_cast<http*>(this);

			base->transition(state<body<http>>());
			base->post(input + strlen(input) - non_header_size);
		}
	}

	void react(char input)
	{
		react(&input, 1);
	}

	void react(const char* input)
	{
		react(input, strlen(input));
	}

	std::string get_header()
	{
		return header_;
	}

private:
	std::string header_;
};

class http : public header<http>, public body<http>
{
public:
	http()
	{
		state_ = [=](const char* input){this->header::react(input);};
	}

	void react(const char* event)
	{
		pending_.push_back(event);
		
		while (!pending_.empty())
		{
			event = pending_.front();
			pending_.pop_front();

			state_(event);
		}
	}

	void react(char event)
	{
		//state_(event);
	}

	void post(const char* event)
	{
		pending_.push_back(event);
	}

	void send(const char* event)
	{
		react(event);
	}

	template <typename state>
	void transition(state)
	{
		using state_type = typename state::type;
		state_ = [=](const char* input){this->state_type::react(input);};
	}

	template <typename T>
	void hello(T t) { }
private:
	std::function<void(const char*)> state_;
	std::list<const char*> pending_;
};

}

struct http_parser_test : public ::testing::Test
{
	tdd::http http;
};

TEST_F(http_parser_test, reads_complete_header)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	
	EXPECT_EQ("GET / HTTP/1.1\r\n\r\n", http.get_header());
}

TEST_F(http_parser_test, finds_out_header_boundary)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	http.react("this is not a header");

	EXPECT_EQ("GET / HTTP/1.1\r\n\r\n", http.get_header());
}

TEST_F(http_parser_test, reads_http_message)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	http.react("this is a message"); 

	EXPECT_EQ("this is a message", http.get_message());
}

