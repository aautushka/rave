#pragma once

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>

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
};

struct active : sc::simple_state<active, machine, as> 
{
	volatile int as_ = 0;
	volatile int bs_ = 0;
};

struct as : sc::simple_state<as, active> 
{
	using reactions = sc::transition<event_b, bs>;

	~as()
	{
		++context<active>().as_;
	}
};

struct bs : sc::simple_state<bs, active>
{
	using reactions = sc::transition<event_a, as>;

	~bs()
	{
		++context<active>().bs_;
	}
};


} // namespace m6

