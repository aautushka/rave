/*
MIT License

Copyright (c) 2017 Anton Autushka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <typeinfo>

namespace haisu
{

template <typename T>
struct is_function_pointer : std::integral_constant<bool, 
    std::is_pointer<T>{} && std::is_function<typename std::remove_pointer_t<T>>{}>
{
};

template <typename T>
struct casts_to_function_pointer : std::integral_constant<bool,
    std::is_function<T>{} || is_function_pointer<T>{}>
{
};

// This is a generic function reference class inspired by LLVM's function_ref.
// There are some differences though:
// 1) haisu::function_ref stores function pointers directly, which allows object to be safely copied and stored
// 2) haisu::function_ref tries to convert a lambda (or any other function object for that matter) to a function pointer
// 3) more closely follows std::function interface (admittedly, not quite yet)

template <typename T>
class function_ref;

template <typename Ret, typename ... Arg>
class function_ref<Ret(Arg...)> final
{
public:
    function_ref(const function_ref&) = default;
    function_ref& operator =(const function_ref&) = default;

    function_ref() noexcept
    {
        assign(nullptr);
    }

    function_ref(std::nullptr_t) noexcept
    {
        assign(nullptr);
    }

    template <typename Callable, typename = std::enable_if_t<!std::is_same<std::decay_t<Callable>, function_ref>{}>>
    function_ref(Callable&& callable) noexcept
    {
        assign(std::move(callable));
    }

    template <typename R, typename ... A>
    function_ref(R (*func)(A...)) noexcept
    {
        assign(func);
    }

    function_ref(Ret(*func)(Arg...)) noexcept
    {
        assign(func);
    }

    template <typename Callable, typename = std::enable_if_t<!std::is_same<std::decay_t<Callable>, function_ref>{}>>
    function_ref& operator =(Callable&& callable) noexcept
    {
        return assign(std::move(callable));
    }

    template <typename Callable, typename = 
        std::enable_if_t<
            !casts_to_function_pointer<Callable>{} && 
            !std::is_constructible<Ret(*)(Arg...), Callable>{} && 
            !std::is_same<std::decay_t<Callable>, function_ref>{}
        >
    >
    function_ref& assign(Callable&& callable) noexcept
    {
        stored_func_ = std::addressof(callable);
        caller_ = call_stored_object<std::decay_t<Callable>>;
        safe_to_store_ = false;
        return *this;
    }

    template <typename R, typename ... A>
    function_ref& assign(R (*func)(A...)) noexcept
    {
        stored_func_ = reinterpret_cast<void*>(func);
        caller_ = call_stored_function<decltype(func)>;
        safe_to_store_ = true;
        return *this;
    }

    function_ref& assign(Ret(*func)(Arg...)) noexcept
    {
        stored_func_ = (void*)func;
        caller_ = call_stored_function<decltype(func)>;
        safe_to_store_ = true;
        return *this;
    }

    function_ref& assign(std::nullptr_t) noexcept
    {
        safe_to_store_ = false;
        caller_ = [](void*, Arg...) -> Ret {throw std::exception();};
        stored_func_ = nullptr;
        return *this;
    }


    template <typename ... T>
    Ret operator ()(T&& ... args) const
    {
        return caller_(stored_func_, std::forward<T>(args)...);
    }

    operator bool() const noexcept
    {
        return stored_func_ != nullptr;
    }

    bool safe_to_store() const noexcept
    {
        return safe_to_store_;
    }

    void swap(function_ref& rhs) noexcept
    {
        std::swap(stored_func_, rhs.stored_func_);
        std::swap(caller_, rhs.caller_);
        std::swap(safe_to_store_, rhs.safe_to_store_);
    }

private:
    template <typename Callable>
    static Ret call_stored_object(void* obj, Arg ... arg)
    {
        return (*reinterpret_cast<Callable*>(obj))(std::forward<Arg>(arg)...);
    }

    template <typename Func>
    static Ret call_stored_function(void* func, Arg ... arg)
    {
        return (reinterpret_cast<Func>(func))(std::forward<Arg>(arg)...);
    }

    void* stored_func_{nullptr};
    Ret (*caller_)(void* f, Arg...);
    bool safe_to_store_{true};
};

template <typename Ret, typename ... Args>
bool operator ==(const function_ref<Ret(Args...)>& f, std::nullptr_t) noexcept
{
    return !f;
}

template <typename Ret, typename ... Args>
bool operator ==(std::nullptr_t, const function_ref<Ret(Args...)>& f) noexcept
{
    return !f;
}

template <typename Ret, typename ... Args>
bool operator !=(const function_ref<Ret(Args...)>& f, std::nullptr_t) noexcept
{
    return f;
}

template <typename Ret, typename ... Args>
bool operator !=(std::nullptr_t, const function_ref<Ret(Args...)>& f) noexcept
{
    return f;
}

} // namespace haisu

namespace std
{
template <typename Ret, typename ... Args>
void swap(haisu::function_ref<Ret(Args...)>& lhs, haisu::function_ref<Ret(Args...)>& rhs)
{
    lhs.swap(rhs);
}
} // namespace std


