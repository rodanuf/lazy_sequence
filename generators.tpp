#include "generators.hpp"

template <typename T>
unary_generator<T>::unary_generator(lazy_sequence<T> *owner, std::function<T(T)> unfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }
    generator_storage = (owner->memoized.get())->get_last();
    unary_function = unfunc;
}

template <typename T>
T& unary_generator<T>::get_next()
{
    generator_storage = unary_function(generator_storage);
    return generator_storage;
}

template <typename T>
bool unary_generator<T>::has_next()
{
    return unary_function(generator_storage);
}




template <typename T>
binary_generator<T>::binary_generator(lazy_sequence<T> *owner, std::function<T(T, T)> binfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }
    int last_idx = (owner->memoized.get())->get_length() - 1;
    generator_storage = *((owner->memoized.get())->get_subsequence(last_idx - 1, last_idx));
    binary_function = binfunc;
}

template <typename T>
T& binary_generator<T>::get_next()
{
    T &result = binary_function(generator_storage.get_first(), generator_storage.get_last());
    generator_storage.remove_at(0);
    generator_storage.append_element(result);
    return result;
}

template <typename T>
bool binary_generator<T>::has_next()
{
    return binary_function(generator_storage.get_first(), generator_storage.get_last());
}




template <typename T>
nary_generator<T>::nary_generator(lazy_sequence<T> *owner, int ar, std::function<T(sequence<T> *)> seqfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }

    int last_idx = (owner->memoized.get())->get_length() - 1;
    (*generator_storage) = *((owner->memoized.get())->get_subsequence(last_idx - arity, last_idx));
    sequence_function = seqfunc;
}

template <typename T>
T& nary_generator<T>::get_next()
{
    T &result = sequence_function(&generator_storage);
    generator_storage.append_element(result);
    generator_storage.remove_at(0);
    return result;
}

template <typename T>
bool nary_generator<T>::has_next()
{
    return this->get_next();
}




template <typename T>
skip_generator<T>::skip_generator(lazy_sequence<T> *owner, lazy_sequence<T> *parent, int start_skip, int end_skip)
    : parent(parent), start_idx(start_skip), end_idx(end_skip)
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
void skip_generator<T>::skip()
{
    if (start_idx == end_idx)
    {
        return;
    }
    for (int i = start_idx; i <= end_idx; i++)
    {
        (*parent).gen->get_next();
    }
    start_idx = 0;
    end_idx = 0;
}

template <typename T>
void skip_generator<T>::skip(int new_start, int new_end)
{
    this->start_idx = new_start;
    this->end_idx = new_end;
    skip();
}

template <typename T>
T& skip_generator<T>::get_next()
{
    skip();
    return (*parent).gen->get_next();
}

template <typename T>
bool skip_generator<T>::has_next()
{
    if (start_idx == end_idx)
    {
        return (*parent).gen->has_next();
    }
}




template <typename T>
insert_generator<T>::insert_generator(shared_ptr<lazy_sequence<T>> parent, shared_ptr<lazy_sequence<T>> other)
{
    if (*parent == nullptr || *other == nullptr)
    {
        throw std::invalid_argument("Parent or other is nullptr");
    }
    this->parent = parent;
    this->other = other;
}

template <typename T>
T& insert_generator<T>::get_other_next()
{
    return (*other).gen->get_next();
}

template <typename T>
T& insert_generator<T>::get_next()
{
    return (*parent).gen.get_next();
}

template <typename T>
bool insert_generator<T>::has_next()
{
    return (*parent).gen.has_next();
}




template <typename T>
filter_generator<T>::filter_generator(shared_ptr<lazy_sequence<T>> parent, std::function<bool(T)> filter_function)
{
    this->parent = parent;
    this->filter_function = filter_function;
}

