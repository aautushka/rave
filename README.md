# rave
The goal is to implement a simple yet fast C++ state-machine. Writing boilerplate code is no fun, yet using the all-powerful Boost libraries (boost.statechart and boost.msm) may be an overkill in most applications. I usually resign to using the C-style switch-based state-machine, because it's fast, easy to debug, understand and reason about. 

This is the benchmarking results (using google benchmark library).
![Screenshot](bench.png)

Now, what are those m1...m9 mean. 

#m1
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
