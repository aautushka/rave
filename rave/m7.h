#pragma once

namespace m7
{

template <int ... I>
struct int_list;

template <int I, int ... Is>
struct int_list<I, Is ...> 
{
	enum {head = I};
	using tail = int_list<Is ...>;
};

template <>
struct int_list<>
{
};

enum states {A, B};

class machine
{
public:
	template <int N>
	void process_state(char ch);

	void transition(int state)
	{
		state_ = state;
	}

	void send(char ch)
	{
		react(ch);
	}

	void react(char ch);

private:
	volatile int as_ = 0;
	volatile int bs_ = 0;
	int state_ = A;
};

template <>
void machine::process_state<A>(char ch)
{
	if (ch == 'a')
	{
		++as_;
	}
	else
	{
		transition(B);
		send(ch);
	}
}

template <>
void machine::process_state<B>(char ch)
{

	if (ch == 'b')
	{
		++bs_;
	}
	else
	{
		transition(A);
		send(ch);
	}
}

template <typename States>
void call_state(int state, machine& m, char ch)
{
	if (state == States::head)
	{
		m.process_state<States::head>(ch);
	}
	else
	{
		call_state<typename States::tail>(state, m, ch);
	}
}

template <>
void call_state<int_list<>>(int state, machine& m, char ch)
{
}

void machine::react(char ch)
{
	call_state<int_list<A, B>>(state_, *this, ch);
}

} // namespace m7

