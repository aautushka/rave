#pragma once 

#include "rave/rave2.h"

namespace m9
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
			rave2::transition<bs>(this);
			rave2::send(this, ch);
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
			rave2::transition<as>(this);
			rave2::send(this, ch);
		}
	}

	volatile int bs_ = 0;
};

class machine : public rave2::machine<machine, as, bs> {};

} // namespace m9

