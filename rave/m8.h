#pragma once

namespace m8
{

struct machine;

void as(machine* m, char ch);
void bs(machine* m, char ch);

struct machine
{
	void react(char ch)
	{
		state_(this, ch);
	}

	volatile int as_ = 0;
	volatile int bs_ = 0;
	void (*state_)(machine*, char) = as;
};

void as(machine* m, char ch)
{
	if (ch == 'a')
	{
	}
	else
	{
		m->state_ = bs;
		m->react(ch);
	}
}

void bs(machine* m, char ch)
{
	if (ch == 'b')
	{
	}
	else
	{
		m->state_ = as;
		m->react(ch);
	}

}

} // namespace m8

