#include "generators.hpp"

template <typename T>
unary_generator<T>::unary_generator(const lazy_sequence<T>& owner, std::function<T(T)> unfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }
    generator_storage = (owner.memoized.get()).get_last();
    unary_function = unfunc;
}

template <typename T>
T& unary_generator<T>::get_other_next()
{
    return this->get_next();
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
optional<T> unary_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}





template <typename T>
binary_generator<T>::binary_generator(const lazy_sequence<T>& owner, std::function<T(T, T)> binfunc)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }
    int last_idx = (owner.memoized.get()).get_length() - 1;
    generator_storage = *((owner.memoized.get()).get_subsequence(last_idx - 1, last_idx));
    binary_function = binfunc;
}

template <typename T>
T& binary_generator<T>::get_other_next()
{
    return this->get_next();
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
optional<T> binary_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}




template <typename T>
nary_generator<T>::nary_generator(const lazy_sequence<T>& owner, int ar, std::function<T(sequence<T> *)> seqfunc)
    : arity(ar)
{
    if (owner == nullptr)
    {
        throw std::invalid_argument("Owner is nullptr");
    }

    int last_idx = (owner.memoized.get()).get_length() - 1;
    generator_storage = uniq_ptr<array_sequence<T>>(new array_sequence<T>(*((owner->memoized.get())->get_subsequence(last_idx - arity, last_idx))));
    sequence_function = seqfunc;
}

template <typename T>
T& nary_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
T& nary_generator<T>::get_next()
{
    T result = sequence_function(generator_storage.get());
    generator_storage->append_element(result);
    generator_storage->remove_at(0);
    return generator_storage->get_last();
}

template <typename T>
bool nary_generator<T>::has_next()
{
    return this->get_next();
}

template <typename T>
optional<T> nary_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}




template <typename T>
concat_generator<T>::concat_generator(const lazy_sequence<T>& parent, const lazy_sequence<T>& other)
{
    if (*parent == nullptr || *other == nullptr)
    {
        throw std::invalid_argument("Parent or other is nullptr");
    }
    this->parent = shared_ptr(&parent);
    this->other = shared_ptr(&other);
}

template <typename T>
T& concat_generator<T>::get_other_next()
{
    return (*other).gen->get_next();
}

template <typename T>
T& concat_generator<T>::get_next()
{
    return (*parent).gen.get_next();
}

template <typename T>
bool concat_generator<T>::has_next()
{
    return (*parent).gen.has_next();
}

template <typename T>
optional<T> concat_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}




template <typename T, typename T2>
map_generator<T, T2>::map_generator(const lazy_sequence<T>& parent, std::function<T2(T)> func)
{
    this->parent = shared_ptr(&parent);
    this->map_function = func;
}

template <typename T, typename T2>
T2& map_generator<T, T2>::get_other_next()
{
    return map_function((*parent).gen.get_next());
}


template <typename T, typename T2>
T2& map_generator<T, T2>::get_next()
{
    return map_function((*parent).gen.get_next());
}

template <typename T, typename T2>
bool map_generator<T, T2>::has_next()
{
    return (*parent).gen.has_next();
}

template <typename T, typename T2>
optional<T2> map_generator<T, T2>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}




template <typename T>
filter_generator<T>::filter_generator(const lazy_sequence<T>& parent, std::function<bool(T)> filter_function)
{
    this->parent = shared_ptr(&parent);
    this->filter_function = filter_function;
}

template <typename T>
T& filter_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
T& filter_generator<T>::get_next()
{
    T &result = (*parent).gen.get_next();
    while (!filter_function(result))
    {
        result = (*parent).gen.get_next();
    }
    return result;
}

template <typename T>
bool filter_generator<T>::has_next()
{
    T &result = (*parent).gen.get_next();
    if (!filter_function(result))
    {
        return false;
    }
    return true;
}


template <typename T>
optional<T> filter_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}



template <typename T>
pull_generator<T>::pull_generator(const lazy_sequence<T>& parent, const T& item, const ordinal& index, const ordinal& start_idx)
{
    this->parent = shared_ptr(&parent);
    element = item;
    index = index;
    index_initialize = start_idx;
}

template <typename T>
T& pull_generator<T>::get_next()
{
    if (index == index_initialize)
    {
        return element;
    }
    else
    {
        return (*parent).gen.get_next();
    }
}

template <typename T>
bool pull_generator<T>::has_next()
{
    if (index == index_initialize)
    {
        return true;
    }
    else
    {
        return (*parent).gen.has_next();
    }
}

template <typename T>
optional<T> pull_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional();
    }
}