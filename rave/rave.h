#pragma once 
#include "meta.h"

namespace rave
{

using event_t = char;

template <template <typename> typename new_state, typename machine, template <typename> typename old_state>
void transition(old_state<machine>* state)
{
	auto base = static_cast<machine*>(state);
	base->template transition<new_state>();
}

template <typename machine, template <typename> typename state>
void post(state<machine>* s, event_t event)
{
	auto base = static_cast<machine*>(s);
	base->post(event);
}

template <typename machine, template <typename> typename state>
void send(state<machine>* s, event_t event)
{
	auto base = static_cast<machine*>(s);
	base->send(event);
}

template <int N, typename T>
struct dispatcher
{
	void dispatch(int state, event_t event);
};

template <typename T>
struct dispatcher<0, T>
{
	static_assert(meta::get_size<T>::value == 0, "");

	template <typename Event>	
	void dispatch(int state, Event event)
	{
	};
};

template <typename T>
struct dispatcher<1, T> : public T
{
	static_assert(meta::get_size<T>::value == 1, "");
	using dispatcher_type = dispatcher<1, T>;

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			this->state0::react(event);
			static_cast<state0*>(this)->react(event);
			break;
		};
	};
};

template <typename T>
struct dispatcher<2, T> : public T
{
	static_assert(meta::get_size<T>::value == 2, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			this->state0::react(event);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			this->state1::react(event);
			break;
		};
	};
};
template <typename T>
struct dispatcher<3, T> : public T
{
	static_assert(meta::get_size<T>::value == 3, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			this->state0::react(event);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			this->state1::react(event);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			this->state2::react(event);
			break;
		};
	}
};

template <typename T, template <class> class ... Ts>
class machine : public dispatcher<meta::get_size<meta::type_list<Ts<T> ...>>::value, meta::type_list<Ts<T> ...>>
{
public:
	using base_type = meta::type_list<Ts<T> ...>;
	using dispatcher_type = dispatcher<meta::get_size<meta::type_list<Ts<T> ...>>::value, meta::type_list<Ts<T> ...>>;

	machine()
	{
	}

	void post(event_t event)
	{
		send(event);
	}

	void send(event_t event)
	{
		react(event);
	}

	template <template <class> class State>
	void transition()
	{
		state_ = meta::get_index<State<T>, base_type>::value;
	}

	void react(event_t event)
	{
		dispatcher_type::dispatch(state_, event);
	}

private:

	int state_ = 0;
};

} // namespace rave 

