#pragma once

#include <functional>

namespace m4
{
template <typename machine> class as;
template <typename machine> class bs;

template <typename T>
class state
{
public:
	using type = T;
};

template <typename machine>
class as : public state<as<machine>>
{
public:

	void react(char ch)
	{
		if (ch == 'a')
		{
			++as_;
		}
		else
		{
			auto base = static_cast<machine*>(this);

			base->transition(state<bs<machine>>());
			base->send(ch);
		}
	}

	int get_a() { return as_; }

private:
	volatile int as_ = 0;
};

template <typename machine>
class bs : public state<bs<machine>>
{
public:
	void react(char ch)
	{
		if (ch == 'b')
		{
			++bs_;
		}
		else
		{
			auto base = static_cast<machine*>(this);

			base->transition(state<as<machine>>());
			base->send(ch);
		}
	}

	int get_b() { return bs_; }

private:
	volatile int bs_ = 0;
};

class machine : public as<machine>, public bs<machine>
{
public:
	machine()
	{
		state_ = [=](char input){this->as::react(input);};
	}

	void react(char ch)
	{
		state_(ch);
	}

	void send(char ch)
	{
		react(ch);
	}

	template <typename state>
	void transition(state)
	{
		using state_type = typename state::type;
		state_ = [=](char input){this->state_type::react(input);};
	}

private:
	std::function<void(char)> state_;
};

} // namspace m4

