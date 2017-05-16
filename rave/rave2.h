#pragma once
#include "meta.h"
#include <iostream>

namespace rave2
{

template <class T>
auto call_on_exit(T& t, int) -> decltype(t.on_exit(), void())
{
	t.on_exit();
}

template <class T>
void call_on_exit(T& t, long)
{
}

template <typename  T, typename U, typename Event>
inline auto call_react(U* u, Event event, int) -> decltype(u->T::react(event), void())
{
	u->T::react(std::move(event));
}

template <typename  T, typename U, typename Event>
inline void call_react(U*, Event, long)
{
}

template <class T>
auto call_on_entry(T& t, int) -> decltype(t.on_entry(), void())
{
	t.on_entry();
}

template <class T>
void call_on_entry(T&, long)
{
}

template <template <typename> typename new_state, typename machine, template <typename> typename old_state>
void transit(old_state<machine>* state)
{
	call_on_exit(*state, 0);
	auto base = static_cast<machine*>(state);
	base->template transit<new_state>();
	call_on_entry(static_cast<new_state<machine>&>(*base), 0);
}

template <typename machine, template <typename> typename state, typename Event>
void post(state<machine>* s, Event event)
{
	auto base = static_cast<machine*>(s);
	base->post(std::move(event));
}

template <typename machine, template <typename> typename state, typename Event>
void send(state<machine>* s, Event event)
{
	auto base = static_cast<machine*>(s);
	base->send(std::move(event));
}

template <int N, typename T>
struct dispatcher
{
	template <typename Event>
	void dispatch(int state, Event event);
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
			call_react<state0>(this, event, 0);
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
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
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
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<4, T> : public T
{
	static_assert(meta::get_size<T>::value == 4, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		};
	}
};


template <typename T>
struct dispatcher<5, T> : public T
{
	static_assert(meta::get_size<T>::value == 5, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<6, T> : public T
{
	static_assert(meta::get_size<T>::value == 6, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<7, T> : public T
{
	static_assert(meta::get_size<T>::value == 7, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<8, T> : public T
{
	static_assert(meta::get_size<T>::value == 7, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<9, T> : public T
{
	static_assert(meta::get_size<T>::value == 9, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		};
	}
};


template <typename T>
struct dispatcher<10, T> : public T
{
	static_assert(meta::get_size<T>::value == 10, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		case 9:
			using state9 = typename meta::get_type<9, T>::type;
			call_react<state9>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<11, T> : public T
{
	static_assert(meta::get_size<T>::value == 11, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		case 9:
			using state9 = typename meta::get_type<9, T>::type;
			call_react<state9>(this, event, 0);
			break;
		case 10:
			using state10 = typename meta::get_type<10, T>::type;
			call_react<state10>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<12, T> : public T
{
	static_assert(meta::get_size<T>::value == 12, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		case 9:
			using state9 = typename meta::get_type<9, T>::type;
			call_react<state9>(this, event, 0);
			break;
		case 10:
			using state10 = typename meta::get_type<10, T>::type;
			call_react<state10>(this, event, 0);
			break;
		case 11:
			using state11 = typename meta::get_type<11, T>::type;
			call_react<state11>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<13, T> : public T
{
	static_assert(meta::get_size<T>::value == 13, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		case 9:
			using state9 = typename meta::get_type<9, T>::type;
			call_react<state9>(this, event, 0);
			break;
		case 10:
			using state10 = typename meta::get_type<10, T>::type;
			call_react<state10>(this, event, 0);
			break;
		case 11:
			using state11 = typename meta::get_type<11, T>::type;
			call_react<state11>(this, event, 0);
			break;
		case 12:
			using state12 = typename meta::get_type<12, T>::type;
			call_react<state12>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<14, T> : public T
{
	static_assert(meta::get_size<T>::value == 14, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		case 9:
			using state9 = typename meta::get_type<9, T>::type;
			call_react<state9>(this, event, 0);
			break;
		case 10:
			using state10 = typename meta::get_type<10, T>::type;
			call_react<state10>(this, event, 0);
			break;
		case 11:
			using state11 = typename meta::get_type<11, T>::type;
			call_react<state11>(this, event, 0);
			break;
		case 12:
			using state12 = typename meta::get_type<12, T>::type;
			call_react<state12>(this, event, 0);
			break;
		case 13:
			using state13 = typename meta::get_type<13, T>::type;
			call_react<state13>(this, event, 0);
			break;
		};
	}
};

template <typename T>
struct dispatcher<15, T> : public T
{
	static_assert(meta::get_size<T>::value == 15, "");

	template <typename Event>
	void dispatch(int state, Event event)
	{
		switch (state)
		{
		case 0:
			using state0 = typename meta::get_type<0, T>::type;
			call_react<state0>(this, event, 0);
			break;
		case 1:
			using state1 = typename meta::get_type<1, T>::type;
			call_react<state1>(this, event, 0);
			break;
		case 2:
			using state2 = typename meta::get_type<2, T>::type;
			call_react<state2>(this, event, 0);
			break;
		case 3:
			using state3 = typename meta::get_type<3, T>::type;
			call_react<state3>(this, event, 0);
			break;
		case 4:
			using state4 = typename meta::get_type<4, T>::type;
			call_react<state4>(this, event, 0);
			break;
		case 5:
			using state5 = typename meta::get_type<5, T>::type;
			call_react<state5>(this, event, 0);
			break;
		case 6:
			using state6 = typename meta::get_type<6, T>::type;
			call_react<state6>(this, event, 0);
			break;
		case 7:
			using state7 = typename meta::get_type<7, T>::type;
			call_react<state7>(this, event, 0);
			break;
		case 8:
			using state8 = typename meta::get_type<8, T>::type;
			call_react<state8>(this, event, 0);
			break;
		case 9:
			using state9 = typename meta::get_type<9, T>::type;
			call_react<state9>(this, event, 0);
			break;
		case 10:
			using state10 = typename meta::get_type<10, T>::type;
			call_react<state10>(this, event, 0);
			break;
		case 11:
			using state11 = typename meta::get_type<11, T>::type;
			call_react<state11>(this, event, 0);
			break;
		case 12:
			using state12 = typename meta::get_type<12, T>::type;
			call_react<state12>(this, event, 0);
			break;
		case 13:
			using state13 = typename meta::get_type<13, T>::type;
			call_react<state13>(this, event, 0);
			break;
		case 14:
			using state14 = typename meta::get_type<14, T>::type;
			call_react<state14>(this, event, 0);
			break;
		};
	}
};

struct state
{
	template <typename Event> void react(Event) {}
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

	template <typename Event>
	void post(Event event)
	{
		send(std::move(event));
	}

	template <typename Event>
	void send(Event event)
	{
		react(std::move(event));
	}

	template <template <class> class State>
	void transit()
	{
		state_ = meta::get_index<State<T>, base_type>::value;
	}

	template <typename Event>
	void react(Event event)
	{
		dispatcher_type::dispatch(state_, event);
	}

private:

	int state_ = 0;
};

} // namespace rave 

