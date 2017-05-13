#pragma once 

#include "rave/rave2.h"

namespace m9
{
struct event_a{};
struct event_b{};

template <typename machine>
struct bs;

template <typename machine>
struct as
{
	void react(event_a)
	{
		++as_;
	}
	
	void react(event_b b)
	{
		rave2::transition<bs>(this);
		rave2::send(this, b);
	}

	volatile int as_ = 0;
};

template <typename machine>
struct bs
{
	void react(event_a a)
	{
		rave2::transition<as>(this);
		rave2::send(this, a);
	}

	void react(event_b)
	{
		++bs_;
	}

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

