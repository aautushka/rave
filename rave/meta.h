#pragma once
#include <functional>
#include <type_traits>
#include <list>

namespace meta
{

template <typename ... Ts> class type_list;

template <typename T, typename ... Ts>
class type_list<T, Ts ...> : public T, public type_list<Ts ...>
{
public:
	using head = T;
	using tail = type_list<Ts ...>;
};

template <>
struct type_list<> 
{ 
};

template <int N, typename List>
struct get_type
{
	using type = typename get_type<N - 1, typename List::tail>::type;
};

template <typename List>
struct get_type<0, List>
{
	using type = typename List::head;	
};

template <typename List>
struct get_size
{
	enum {value = 1 + get_size<typename List::tail>::value };
};

template <>
struct get_size<type_list<>>
{
	enum {value = 0};
};


template <typename T, typename List>
struct get_index
{
	enum { value = std::is_same<T, typename List::head>::value ? 0 : 1 + get_index<T, typename List::tail>::value };
};

template <typename T>
struct get_index<T, type_list<>>
{
	enum { value = 0 };
};
} // namespace meta
