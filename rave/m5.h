#pragma once 

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

namespace m5
{
namespace msm = boost::msm;
namespace mpl = boost::mpl;

struct event_a { };

struct event_b { };

class machine_def : public msm::front::state_machine_def<machine_def>
{
public:
	template <class Event, class FSM>
	void on_entry(const Event&, FSM&) 
	{
	}

	template <class Event, class FSM>
	void on_exit(const Event&, FSM&) 
	{
	}

	struct as : public msm::front::state<>
	{
		template <class Event, class FSM>
		void on_entry(const Event&, FSM&) {}

		template <class Event, class FSM>
		void on_exit(const Event&, FSM&) {}
	};

	struct bs : public msm::front::state<>
	{
		template <class Event, class FSM>
		void on_entry(const Event&, FSM&) {}

		template <class Event, class FSM>
		void on_exit(const Event&, FSM&) {}
	};

	typedef as initial_state;

	void process_as(const event_a&)
	{
		++as_;
	}

	void process_bs(const event_b&)
	{
		++bs_;
	}

	struct transition_table: mpl::vector<
		a_row<as, event_b, bs, &machine_def::process_bs>,
		a_row<as, event_a, as, &machine_def::process_as>,
		a_row<bs, event_b, bs, &machine_def::process_bs>,
		a_row<bs, event_a, as, &machine_def::process_as>
		> {};

	template <class FSM, class Event>
	void no_transition(const Event& e, FSM&, int state)
	{
	}

private:
	volatile int as_ = 0;
	volatile int bs_ = 0;
};


class machine 
{
public:
	machine()
	{
		m_.start();
	}

	void react(char ch)
	{
		if (ch == 'a')
		{
			m_.process_event(event_a());
		}
		else 
		{
			m_.process_event(event_b());
		}
	}

private:
	msm::back::state_machine<machine_def> m_;
};

} // namespace m5

