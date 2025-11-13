#include "lazy_sequence.hpp"
#include <stdexcept>

template <typename T>
lazy_sequence<T>::unary_generator::unary_generator(lazy_sequence<T>* seq, std::function<T(T)> unfunc) : owner(seq), generator_storage((*(*seq).buffer).clone()), unary_function(unfunc) {}

template <typename T>
T& lazy_sequence<T>::unary_generator::get_next()
{
    (*generator_storage).append_element(unary_function((*owner).get_last()));
    (*generator_storage).remove_at(0);
    return (*generator_storage).get_last();
}

template <typename T>
bool lazy_sequence<T>::unary_generator::has_next()
{
    return unary_function((*owner).get_last());
}


template <typename T>
lazy_sequence<T>::binary_generator::binary_generator(lazy_sequence<T>* seq, std::function<T(T,T)> binfunc) : owner(seq), binary_function(binfunc) {}

template <typename T>
T& lazy_sequence<T>::binary_generator::get_next()
{
    return binary_function((*owner).get((*owner).get_length() - 1), (*owner).get_last())
}

template <typename T>
bool lazy_sequence<T>::binary_generator::has_next()
{
    return binary_function((*owner).get((*owner).get_length() - 1), (*owner).get_last())
}

template <typename T>
lazy_sequence<T>::sequence_generator::sequence_generator(lazy_sequence<T>* seq, int ar, std::function<T(sequence<T>*)> seqfunc) : owner(seq), arity(ar), sequence_function(seqfunc) {}

template <typename T>
T& lazy_sequence<T>::sequence_generator::get_next()
{
    int last_idx = (*owner).get_length() - 1;
    int start_arity = last_idx - arity;
    return sequence_function((*owner).get_subsequence(start_arity, last_idx))
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
lazy_sequence<T>& lazy_sequence<T>::skip_generator::skip()
{

}
