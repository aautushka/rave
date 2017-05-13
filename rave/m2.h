#pragma once 

#include "rave/rave.h"

namespace m2
{
template <typename machine>
struct bs;

template <typename machine>
struct as
{
	void react(char ch)
	{
		if (ch == 'a')
		{
			++as_;
		}
		else
		{
			rave::transition<bs>(this);
			rave::send(this, ch);
		}
	}

	volatile int as_ = 0;
};

template <typename machine>
struct bs
{
	void react(char ch)
	{
		if (ch == 'b')
		{
			++bs_;
		}
		else
		{
			rave::transition<as>(this);
			rave::send(this, ch);
		}
	}

	volatile int bs_ = 0;
};

class machine : public rave::machine<machine, m2::as, m2::bs> {};

} // namespace m2

