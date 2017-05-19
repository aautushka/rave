# rave
The goal is to implement a simple yet fast C++ state-machine. Writing boilerplate code is no fun, yet using the all-powerful Boost libraries (boost.statechart and boost.msm) may be an overkill in most applications. I usually resign to using the C-style switch-based state-machine, because it's fast, easy to debug, understand and reason about. 

This is the benchmarking results (using google benchmark library). Reproduced on ArchLinux + GCC 6.2 (make sure it's a Release build: cmake -DCMAKE_BUILD_TYPE=Release ..)

![Screenshot](bench.png)

Now, what do these m1 ... m9 mean. 

## m1
The most primitive state machine imaginagle, just one stupid switch
```c++
switch (state)
{
case STATE_A:
    process_state_a(event); break;
case STATE_B:
    process_state_b(event); break;
};
```

## m2
My own state-machine generalization, I was trying to emulate the m1 machine with modern C++ adding some templates to the mix
```c++
template <class machine> struct state_a
{
   void react(event e)
   {
        process_state_a(event);
   }
};
template <class machine> struct state_b
{
    void react(event e)
    {
        process_state_b(event);
    }
};

class machine: public rave::machine<machine, state_a, state_b> {};
```

## m3
A C++ classic: using vtable dispatching and iterface classes
```c++
struct state
{
    virtual void react(event e) = 0;
};

struct state_a : public state {};
struct state_b : public state {};

struct machine
{
    state* current_state_;
    
    void react(event e)
    {
        current_state_->react(e);
    }
};
```

## m4
Another attempt in emulating the switch-based machine, but with std::function as a primary dispatching mechanism
```c++
```

## m5
A Boost.MSM statemachine. I feel the comparison would be incomplete without including this glorious piece of engineering in this review. Given all the power provided by MSM it works surprisingly fast. This is by far the best general-purpose state-machine written in C++ I've ever seen. 
```c++
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>

struct event_a() {};
struct event_b() {};

class def : public msm::front::state_machine_def<def> 
{
    ...
};

using machine = boost::msm::back::state_machine<def>;

```

## m6
A Boost.Statechart machine. By far the slowest.
```c++
namespace sc = boost::statechart;

struct machine : sc::state_machine<machine, active> {};

struct active : sc::simple_state<active, machine, state_a>{};

struct state_a : sc::simple_state<state_a, active> {};

struct state_b : sc::simple_state<state_b, active> {};

```

## m7
A simple C++ state-machine inspided by Gerhard Reitmayr's https://github.com/GerhardR/fsm. It uses a neat metaprogramming trick to dispach an event to appropriate handler defined but a template function specialization.
```c++
struct machine
{
    template <int State> void react(event);
};

template<> void machine::react<STATE_A>(event);
template<> void machine::react<STATE_A>(event);
```

## m8
This is another C state-machine which uses a function pointer for tracking states and dispatching events. Essensially it's the same technique used in C++ vtables, bur rather implemented in the most straightforward way without the C++ machinery.
```c++
struct machine
{
    void (state*)(event);
};

void process_state_a(event);
void process_state_b(event);
```

## m9
The second take on m2 machine, slightly improved (now the machine can dispatch events of different types, instead of single event type in m2.
```c++
struct event_a{};
struct event_b{};

template <class machine>
struct state_a
{
    void react(state_a);
    void react(state_b);
};

template <class machine>
struct state_b
{
    void react(state_a);
    void react(state_b);
};

struct machine : public rave2::machine<machine, state_a, state_b>
{
};
```
# Conclusions
* To this day, the good ol' C remains the fastest solution, nothing can really beat it. And nothing would. In the end we are all trying hard to bring the C performance back to C++
* With C++ templates and metaprogramming techniques modern compilers do wonders: we can hardly outdo C, but we can get close. Besides the code gets more object-oriented, tremendously helping in the long run
* The classic C++ vtable solution remains viable enough, it's reasonably fast and reasonably nice. But I love templates better. One major downside is the tons of boilerplate code.
* Boost MSM is great, but it's a little heavy-weight for my purposes.
* Boost Statechart does not look great performance-wise. I used to use it a lot in production code because of its expressive power, but one needs to be aware of its performance
* Don't know why, but std::function is so slow. I will investigate this one more.




