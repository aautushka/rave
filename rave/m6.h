#pragma once

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

namespace m6
{
namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct event_a : sc::event<event_a>{};
struct event_b : sc::event<event_b>{};

struct as;
struct bs;
struct active;

struct machine : sc::state_machine<machine, active> 
{
	machine()
	{
		initiate();
	}

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

	int get_a();
	int get_b();

	volatile int as_ = 0;
	volatile int bs_ = 0;
};

struct active : sc::simple_state<active, machine, as> 
{
	volatile int as_ = 0;
	volatile int bs_ = 0;
};

struct as : sc::simple_state<as, active> 
{
	using reactions = mpl::list<
		sc::custom_reaction<event_a>,
		sc::custom_reaction<event_b>>;

	sc::result react(const event_a&)
	{
		++context<active>().as_;
		return transit<as>();
	}

	sc::result react(const event_b&)
	{
		++context<active>().bs_;
		return transit<bs>();
	}
};

struct bs : sc::simple_state<bs, active>
{
	using reactions = mpl::list<
		sc::custom_reaction<event_a>,
		sc::custom_reaction<event_b>>;

	sc::result react(const event_a&)
	{
		++context<active>().as_;
		return transit<as>();
	}

	sc::result react(const event_b&)
	{
		++context<active>().bs_;
		return transit<bs>();
	}
};

int machine::get_a()
{
	return state_cast<const active&>().as_;
}

int machine::get_b()
{
	return state_cast<const active&>().bs_;
}


} // namespace m6

