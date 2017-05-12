#include <gtest/gtest.h>
#include <functional>
#include <list>

namespace tdd
{

enum state
{
	READING_HEADER,
	READING_BODY 
};

class chttp
{
public:
	void react(const char* event)
	{
		pending_.push_back(event);
		
		while (!pending_.empty())
		{
			event = pending_.front();
			pending_.pop_front();

			switch (state_)
			{
				case READING_HEADER:
					read_header(event);
					break;
				case READING_BODY:
					read_body(event);
					break;
			}
		}
	}

	void post(const char* event)
	{
		pending_.push_back(event);
	}

	void transition(int state)
	{
		state_ = state;
	}

	std::string get_message()
	{
		return message_;
	}

	std::string get_header()
	{
		return header_;
	}

private:
	void read_header(const char* input)
	{
		header_.append(input);
		size_t header_end = header_.find("\r\n\r\n");

		if (header_end != std::string::npos)
		{
			size_t non_header_size = header_.size() - header_end - 4;

			transition(READING_BODY);
			post(input + strlen(input) - non_header_size);
		}
	}

	void read_body(const char* input)
	{
		message_.append(input);
	}

	int state_ = READING_HEADER;
	std::list<const char*> pending_;

	std::string message_;
	std::string header_;
};

}

struct c_http_parser_test : public ::testing::Test
{
	tdd::chttp http;
};

TEST_F(c_http_parser_test, reads_complete_header)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	
	EXPECT_EQ("GET / HTTP/1.1\r\n\r\n", http.get_header());
}

TEST_F(c_http_parser_test, finds_out_header_boundary)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	http.react("this is not a header");

	EXPECT_EQ("GET / HTTP/1.1\r\n\r\n", http.get_header());
}

TEST_F(c_http_parser_test, reads_http_message)
{
	http.react("GET / HTTP/1.1\r\n\r\n");
	http.react("this is a message"); 

	EXPECT_EQ("this is a message", http.get_message());
}

