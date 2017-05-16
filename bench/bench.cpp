#include "benchmark/benchmark_api.h"

#include "rave/m0.h"
#include "rave/m1.h"
#include "rave/m2.h"
#include "rave/m3.h"
#include "rave/m4.h"
#include "rave/m5.h"
#include "rave/m6.h"
#include "rave/m7.h"
#include "rave/m8.h"
#include "rave/m9.h"
#include "rave/m10.h"

#include <random>
#include <iostream>

std::vector<char> generate_input_seq()
{
	std::random_device rnd;
	std::mt19937 gen(rnd());
	std::uniform_int_distribution<> dis('a', 'b');
	std::vector<char> ret;

	for (size_t i = 0; i < 1024; ++i)
	{
		ret.push_back(dis(gen));
	}

	return std::move(ret);
}

std::vector<char> input_seq = generate_input_seq();

template <class T> void
state_machine(benchmark::State& state) 
{
	std::vector<char>& v = input_seq;

	T machine;

	while (state.KeepRunning())
	{
		for (size_t i = 0; i < v.size(); ++i)
		{
			machine.react(v[i]);
		}
	}
}

BENCHMARK_TEMPLATE(state_machine, m0::machine);
BENCHMARK_TEMPLATE(state_machine, m1::machine);
BENCHMARK_TEMPLATE(state_machine, m2::machine);
BENCHMARK_TEMPLATE(state_machine, m3::machine);
BENCHMARK_TEMPLATE(state_machine, m4::machine);
BENCHMARK_TEMPLATE(state_machine, m5::machine);
BENCHMARK_TEMPLATE(state_machine, m6::machine);
BENCHMARK_TEMPLATE(state_machine, m7::machine);
BENCHMARK_TEMPLATE(state_machine, m8::machine);
BENCHMARK_TEMPLATE(state_machine, m9::machine);
BENCHMARK_TEMPLATE(state_machine, m10::machine);

BENCHMARK_MAIN();
