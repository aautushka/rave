#pragma once

namespace m3
{

class machine;
class as;
class bs;

template <typename T> struct tag{};

class state
{
public:
	virtual ~state() {}
	virtual void react(machine* m, char ch) = 0;
};

class as : public state
{
public:
	virtual void react(machine* m, char ch);

private:
	volatile int as_ = 0;
};

class bs : public state
{
public:
	void react(machine* m, char ch);
private:
	volatile int bs_ = 0;
};

class machine
{
public:
	void react(char ch)
	{
		state_->react(this, ch);
	}

	void send(char ch)
	{
		react(ch);
	}

	void transition(tag<as>)
	{
		state_ = &as_;
	}

	void transition(tag<bs>)
	{
		state_ = &bs_;
	}

private:
	as as_;
	bs bs_;
	state* state_ = &as_;
};

void as::react(machine* m, char ch)
{
	if (ch == 'a')
	{
		++as_;
	}
	else
	{
		m->transition(tag<bs>());
		m->send(ch);
	}
}
void bs::react(machine* m, char ch)
{
	if (ch == 'b')
	{
		++bs_;
	}
	else
	{
		m->transition(tag<as>());
		m->send(ch);
	}
}

} // namespace m3

