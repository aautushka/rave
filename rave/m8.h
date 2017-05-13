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

	void transit(void (*new_state)(machine*, char))
	{
		state_ = new_state;
	}

	volatile int as_ = 0;
	volatile int bs_ = 0;
	void (*state_)(machine*, char) = as;
};

void as(machine* m, char ch)
{
	if (ch == 'a')
	{
		++m->as_;
	}
	else
	{
		m->transit(bs);
		m->react(ch);
	}
}

void bs(machine* m, char ch)
{
	if (ch == 'b')
	{
		++m->bs_;
	}
	else
	{
		m->transit(as);
		m->react(ch);
	}

}

} // namespace m8

