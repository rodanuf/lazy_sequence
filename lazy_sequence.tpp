#include "lazy_sequence.hpp"

template <typename T>
lazy_sequence<T>::unary_generator::unary_generator(lazy_sequence<T>* seq, std::function<T(T)> unfuncton) : owner(seq), unary_function(unfunction) {}

template <typename T>
T& lazy_sequence<T>::unary_generator::get_next()
{
    return unary_function(owner->get_last());
}

template <typename T>
bool lazy_sequence<T>::unary_generator::has_next()
{
    return unary_function(owner->get_last());
}


template <typename T>
lazy_sequence<T>::binary_generator::binary_generator(lazy_sequence<T>* seq, std::function<T(T,T)> binfunction) : owner(seq), binary_function(binfunction) {}

template <typename T>
lazy_sequence::default_generator::default_generator(lazy_sequence<T>* seq, int arity, std::function<T,(T,T)> generator) : owner(seq), atiry(arity), generator(generator)
{
    unary_generator = nullptr;
    binary_generator = nullptr;
}
