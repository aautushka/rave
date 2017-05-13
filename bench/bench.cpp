#include "benchmark/benchmark_api.h"

#include "rave/m1.h"
#include "rave/m2.h"
#include "rave/m3.h"
#include "rave/m4.h"
#include "rave/m5.h"
#include "rave/m6.h"
#include "rave/m7.h"

template <class T> void
state_machine(benchmark::State& state) 
{
	std::vector<char> v;
	for (int i = 0; i < 1024; ++i)
	{
		v.push_back(i % 2 ? 'a' : 'b');
	}

	T machine;

	while (state.KeepRunning())
	{
		for (size_t i = 0; i < v.size(); ++i)
		{
			machine.react(v[i]);
		}
	}
}

BENCHMARK_TEMPLATE(state_machine, m1::machine);
BENCHMARK_TEMPLATE(state_machine, m2::machine);
BENCHMARK_TEMPLATE(state_machine, m3::machine);
BENCHMARK_TEMPLATE(state_machine, m4::machine);
BENCHMARK_TEMPLATE(state_machine, m5::machine);
BENCHMARK_TEMPLATE(state_machine, m6::machine);
BENCHMARK_TEMPLATE(state_machine, m7::machine);

BENCHMARK_MAIN();
