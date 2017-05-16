#pragma once 

#include "rave/rave2.h"

namespace m9
{
struct event_a{};
struct event_b{};

struct state
{
template <typename T>
void react(T) { }
};

template <typename machine>
struct bs;

template <typename machine>
struct as : public state
{
	/* void react(event_a)
	{
		++as_;
	}
	
	void react(event_b b)
	{
		rave2::transit<bs>(this);
		rave2::send(this, b);
	}*/

	int get_a() { return as_; }

	volatile int as_ = 0;
};

template <typename machine>
struct bs
{
	void react(event_a a)
	{
		rave2::transit<as>(this);
		rave2::send(this, a);
	}

	void react(event_b)
	{
		++bs_;
	}

	int get_b() { return bs_; }

	volatile int bs_ = 0;
};

class machine : public rave2::machine<machine, as, bs> 
{
public:
	void react(char ch)
	{
		if (ch == 'a')
		{
			send(event_a());
		}
		else
		{
			send(event_b());
		}
	}
};

} // namespace m9

