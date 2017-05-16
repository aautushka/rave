#pragma once
#include <fsmlite/fsm.hpp>

namespace m10
{

class machine : public fsmlite::fsm<machine>
{
	friend class fsm;
public:
	enum states {STATE_A, STATE_B};
	machine() : fsm(STATE_A) {}

	struct event_a {};
	struct event_b {};

	void react(char ch)
	{
		if (ch == 'a')
		{
			process_event(event_a());
		}
		else
		{
			process_event(event_b());
		}
	}

	void process_a(const event_a&);

	void process_b(const event_b&);

	using transition_table = table<
		mem_fn_row<STATE_A, event_a, STATE_A, &machine::process_a>,
		mem_fn_row<STATE_A, event_b, STATE_B, &machine::process_b>,
		mem_fn_row<STATE_B, event_a, STATE_A, &machine::process_a>,
		mem_fn_row<STATE_B, event_b, STATE_B, &machine::process_b>>;


	volatile int as_ = 0;
	volatile int bs_ = 0;
};

void machine::process_a(const event_a&)
{
	++as_;
}

void machine::process_b(const event_b&)
{
	++bs_;
}

} // namespace m10


