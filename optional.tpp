#include "optional.hpp"

template <typename T>
optional<T>::optional() : has_value(false) {}

template <typename T>
optional<T>::optional(const T& value)
{
    has_value = true;
    storage.value = value;
}

template <typename T>
optional<T>::optional(T&& value)
{
    has_value = true;
    storage.value = std::move(value);
}

template <typename T>
optional<T>::optional(const optional<T>& other)
{
    has_value = other.has_value;
    storage = other.storage;
}

template <typename T>
optional<T>::optional(optional<T>&& other)
{
    has_value = other.has_value;
    if (other.has_value)
    {
        storage = std::move(other.storage);
        other.reset();
    }
}

template <typename T>
optional<T>::~optional()
{
    if (has_value)
    {
        storage.value.~T();
    }
}

template <typename T>
optional<T>& optional<T>::operator=(const T& value)
{
    if (value)
    {
        has_value = true;
    }
    else
    {
        has_value = false;
    }
    
    storage.value = value;
}

template <typename T>
optional<T>& optional<T>::operator=(T&& value)
{
    if (value)
    {
        has_value = true;
    }
    else
    {
        has_value = false;
    }

    storage.value = std::move(value);
}

template <typename T>
optional<T>& optional<T>::operator=(const optional<T>& other)
{
    has_value = other.has_value;
    storage.value = other.storage.value;
}

template <typename T>
optional<T>& optional<T>::operator=(optional<T>&& other)
{
    if (other.has_value)
    {
        has_value = other.has_value;
        storage.value = other.storage.value;
        other.reset();
    }
    else
    {
        reset();
    }
}


template <typename T>
optional<T>& optional<T>::reset()
{
    has_value = false;
    storage.value.~T();
}

template <typename T>
const T& optional<T>::get_value() const
{
    return storage.value;
}

template <typename T>
const T& optional<T>::operator*() const
{
    return storage.value;
}

template <typename T>
bool optional<T>::is_sated()
{
    return has_value;
}

template <typename T>
optional<T>::operator bool()
{
    return has_value;
}