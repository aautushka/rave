#include "benchmark/benchmark_api.h"
#include "rave/rave.h"

class machine1
{
public:
	enum states { A, B};

	void react(char ch)
	{
		switch (state_)
		{
			case A: 
				if (ch == 'a')
				{
					++as_;
				}
				else 
				{
					state_ = B;
					react(ch);
				}
				break;
			case B:
				if (ch == 'b')
				{
					++bs_;
				}
				else
				{
					state_ = A;
					react(ch);
				}
		};
	}

private:
	int state_ = A;
	volatile int as_ = 0;
	volatile int bs_ = 0;

};


namespace v2
{
template <typename machine>
struct bs;

template <typename machine>
struct as
{
	void react(char ch)
	{
		if (ch == 'a')
		{
			++as_;
		}
		else
		{
			rave::transition<bs>(this);
			rave::send(this, ch);
		}
	}

	volatile int as_ = 0;
};

template <typename machine>
struct bs
{
	void react(char ch)
	{
		if (ch == 'b')
		{
			++bs_;
		}
		else
		{
			rave::transition<as>(this);
			rave::send(this, ch);
		}
	}

	volatile int bs_ = 0;
};
} // namespace v2

class machine2 : public rave::machine<machine2, v2::as, v2::bs> {};


namespace v3
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
} // namespace v3

using machine3 = v3::machine;

namespace v4
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
	}

private:
	volatile int as_ = 0;
};

template <typename machine>
class bs : public state<bs<machine>>
{
public:
	void react(char ch)
	{
		//http* base = static_cast<http*>(this);

		//base->transition(state<body<http>>());
		//base->post(input + strlen(input) - non_header_size);
	}

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

} // namspace v4

using machine4 = v4::machine;

static void machine1_bench(benchmark::State& state) 
{
	std::vector<char> v;
	for (int i = 0; i < 1024; ++i)
	{
		v.push_back(i % 2 ? 'a' : 'b');
	}

	machine1 m1;

	while (state.KeepRunning())
	{
		for (size_t i = 0; i < v.size(); ++i)
		{
			m1.react(v[i]);
		}
	}
}

static void machine2_bench(benchmark::State& state) 
{
	std::vector<char> v;
	for (int i = 0; i < 1024; ++i)
	{
		v.push_back(i % 2 ? 'a' : 'b');
	}

	machine2 m2;

	while (state.KeepRunning())
	{
		for (size_t i = 0; i < v.size(); ++i)
		{
			m2.react(v[i]);
		}
	}
}

static void machine3_bench(benchmark::State& state) 
{
	std::vector<char> v;
	for (int i = 0; i < 1024; ++i)
	{
		v.push_back(i % 2 ? 'a' : 'b');
	}

	machine3 m3;

	while (state.KeepRunning())
	{
		for (size_t i = 0; i < v.size(); ++i)
		{
			m3.react(v[i]);
		}
	}
}


static void machine4_bench(benchmark::State& state) 
{
	std::vector<char> v;
	for (int i = 0; i < 1024; ++i)
	{
		v.push_back(i % 2 ? 'a' : 'b');
	}

	machine4 m4;

	while (state.KeepRunning())
	{
		for (size_t i = 0; i < v.size(); ++i)
		{
			m4.react(v[i]);
		}
	}
}

BENCHMARK(machine1_bench);
BENCHMARK(machine2_bench);
BENCHMARK(machine3_bench);
BENCHMARK(machine4_bench);

BENCHMARK_MAIN();
