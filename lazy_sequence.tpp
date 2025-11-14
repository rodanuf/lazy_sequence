#include "lazy_sequence.hpp"
#include <stdexcept>

template <typename T>
lazy_sequence<T>::unary_generator::unary_generator(lazy_sequence<T>* owner, std::function<T(T)> unfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }
    generator_storage = (owner->buffer.get())->get_last();
    unary_function = unfunc
}

template <typename T>
T& lazy_sequence<T>::unary_generator::get_next()
{
    generator_storage = unary_function(generator_storage);
    return generator_storage;
}

template <typename T>
bool lazy_sequence<T>::unary_generator::has_next()
{
    return unary_function(generator_storage);
}


template <typename T>
lazy_sequence<T>::binary_generator::binary_generator(lazy_sequence<T>* owner, std::function<T(T,T)> binfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }
    int last_idx = (owner->buffer.get())->get_length() - 1;
    generator_storage = *((owner->buffer.get())->get_subsequence(last_idx - 1, last_idx))
    binary_function = binfunc;
}

template <typename T>
T& lazy_sequence<T>::binary_generator::get_next()
{
    T& result = binary_function(generator_storage.get_first(), generator_storage.get_last());
    generator_storage.remove_at(0);
    generator_storage.append_element(result);
    return result;
}

template <typename T>
bool lazy_sequence<T>::binary_generator::has_next()
{
    return binary_function(generator_storage.get_first(), generator_storage.get_last())
}

template <typename T>
lazy_sequence<T>::sequence_generator::sequence_generator(lazy_sequence<T>* owner, int ar, std::function<T(sequence<T>*)> seqfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }

    int last_idx = (owner->buffer.get())->get_length() - 1;
    generator_storage = *((owner->buffer.get())->get_subsequence(last_idx - arity, last_idx));
    sequence_function = seqfunc;
}

template <typename T>
T& lazy_sequence<T>::sequence_generator::get_next()
{
    T& result = sequence_function(&generator_storage);
    generator_storage.append_element(result);
    generator_storage.remove_at(0);
    return result;
}

template <typename T>
bool lazy_sequence<T>::sequence_generator::has_next()
{
    return this->get_next();
}

template <typename T>
lazy_sequence<T>::skip_generator::skip_generator(lazy_sequence<T>* owner, lazy_sequence<T>* parent, int start_skip, int end_skip)
: owner__(owner), parent__(parent), start_idx(start_skip), end_idx(end_skip)
{
    if ((end_idx > start_idx) || (end_idx < 0 || start_idx))
    {
        throw std::out_of_range("Bad index");
    }

    if (parent == nullptr || owner == nullptr)
    {
        throw std::invalid_argument("Parent sequence or onwer sequence is nullptr");
    }
}

template <typename T>
