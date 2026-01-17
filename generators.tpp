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
const T& unary_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& unary_generator<T>::get_next()
{
    generator_storage = unary_function(generator_storage);
    return generator_storage;
}

template <typename T>
bool unary_generator<T>::has_next() const
{
    return true;
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
        return optional<T>();
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
const T& binary_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& binary_generator<T>::get_next()
{
    T &result = binary_function(generator_storage.get_first(), generator_storage.get_last());
    generator_storage.remove_at(0);
    generator_storage.append_element(result);
    return result;
}

template <typename T>
bool binary_generator<T>::has_next() const
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
        return optional<T>();
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
    generator_storage = uniq_ptr<array_sequence<T>>(new array_sequence<T>(*((owner.memoized->get())->get_subsequence(last_idx - arity, last_idx))));
    sequence_function = seqfunc;
}

template <typename T>
const T& nary_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& nary_generator<T>::get_next()
{
    T result = sequence_function(generator_storage.get());
    generator_storage->append_element(result);
    generator_storage->remove_at(0);
    return generator_storage->get_last();
}

template <typename T>
bool nary_generator<T>::has_next() const
{
    return true;
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
        return optional<T>();
    }
}




template <typename T>
concat_generator<T>::concat_generator(const lazy_sequence<T>& parent, const lazy_sequence<T>& other)
{
    this->parent = &parent;
    this->other = &other;
}

template <typename T>
const T& concat_generator<T>::get_other_next()
{
    return other->get_next();
}

template <typename T>
const T& concat_generator<T>::get_next()
{
    return parent->get_next();
}

template <typename T>
bool concat_generator<T>::has_next() const
{
    return parent->get_generator()->has_next();
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
        return optional<T>();
    }
}




template <typename T, typename T2>
map_generator<T, T2>::map_generator(const lazy_sequence<T>& parent, std::function<T2(T)> func)
{
    this->parent = &parent;
    this->map_function = func;
}

template <typename T, typename T2>
const T2& map_generator<T, T2>::get_other_next()
{
    T next = parent->get_next();
    return map_function(next);
}


template <typename T, typename T2>
const T2& map_generator<T, T2>::get_next()
{
    return map_function(parent->get_next());
}

template <typename T, typename T2>
bool map_generator<T, T2>::has_next() const
{
    return parent->get_generator()->has_next();
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
        return optional<T2>();
    }
}




template <typename T>
filter_generator<T>::filter_generator(const lazy_sequence<T>& parent, std::function<bool(T)> filter_function)
{
    this->parent = &parent;
    this->filter_function = filter_function;
}

template <typename T>
const T& filter_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& filter_generator<T>::get_next()
{
    T result = parent->get_next();
    while (!filter_function(result))
    {
        result = parent->get_next();
    }
    return filter_function(result);
}

template <typename T>
bool filter_generator<T>::has_next() const
{
    T result = parent->get_next();
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
        return optional<T>();
    }
}



template <typename T>
pull_generator<T>::pull_generator(const lazy_sequence<T>& parent, const T& item, const ordinal& index, const ordinal& start_idx)
{
    this->parent = &parent;
    element = item;
    this->index = index;
    index_initialize = start_idx;
}

template <typename T>
const T& pull_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& pull_generator<T>::get_next()
{
    if (index == index_initialize)
    {
        return element;
    }
    else
    {
        index++;
        return parent->get_next();
    }
}

template <typename T>
bool pull_generator<T>::has_next() const
{
    if (index == index_initialize)
    {
        return true;
    }
    else
    {
        return parent->get_generator()->has_next();
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
        return optional<T>();
    }
}




template <typename T>
increment_generator<T>::increment_generator(const T& start, const T& increment) : current(start), increment_by(increment), result(0) {}

template <typename T>
const T& increment_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& increment_generator<T>::get_next()
{
    result = current;
    current = current + increment_by;
    return result;
}

template <typename T>
bool increment_generator<T>::has_next() const
{
    return true;
}

template <typename T>
optional<T> increment_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional<T>();
    }
}




template <typename T>
fibonacci_generator<T>::fibonacci_generator(const T& first_element, const T& second_element)
{
    first = first_element;
    second = second_element;
    last_value = 0;
}

template <typename T>
const T& fibonacci_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& fibonacci_generator<T>::get_next()
{
    last_value = first;
    first = second;
    second = last_value + second;
    return last_value;
}

template <typename T>
bool fibonacci_generator<T>::has_next() const
{
    return true;
}

template <typename T>
optional<T> fibonacci_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional<T>();
    }
}




template <typename T>
power_generator<T>::power_generator(const T& base)
{
    this->base = base;
    current = 1;
    count = 0;
    result = 0;
}

template <typename T>
const T& power_generator<T>::get_other_next()
{
    return this->get_next();
}

template <typename T>
const T& power_generator<T>::get_next()
{
    if (count == 0)
    {
        result = current;
        count++;
        return result;
    }
    result = current;
    current *= base;
    count++;
    return result;
}

template <typename T>
bool power_generator<T>::has_next() const
{
    return true;
}

template <typename T>
optional<T> power_generator<T>::try_get_next()
{
    if (this->has_next())
    {
        return optional(this->get_next());
    }
    else
    {
        return optional<T>();
    }
}