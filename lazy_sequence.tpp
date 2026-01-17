#include "lazy_sequence.hpp"
#include <stdexcept>

template <typename T>
counter lazy_sequence<T>::begin() const
{
    return counter();
}

template <typename T>
counter lazy_sequence<T>::end() const
{
    return counter(this->get_length() - 1);
}

template <typename T>
lazy_sequence<T>::lazy_sequence() : memoized(nullptr), gen(nullptr), length() {}

template <typename T>
lazy_sequence<T>::lazy_sequence(T* items, int size)
{
    if (items == nullptr)
    {
        throw std::invalid_argument("Array is null");
    }
    array_sequence<T>* seq = new array_sequence<T>(items, size);
    this->set_memoized(seq);
    this->set_generator(nullptr);
    this->set_length(cardinal(0, size));
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const std::initializer_list<T>& list, generator<T>* gen)
{
    auto seq = new array_sequence<T>(list);
    this->set_memoized(shared_ptr<array_sequence<T>> (seq));
    if (!gen)
    {
        this->set_generator(nullptr);
    }
    else
    {
        this->set_generator(gen);
    }
    this->set_length(cardinal(0, seq->get_length()));
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, generator<T>* gen)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }

    if (!gen)
    {
        this->set_generator(nullptr);
    }
    else
    {
        this->set_generator(gen);
    }
    auto ar_seq = new array_sequence<T>(*seq);
    this->set_memoized(shared_ptr<array_sequence<T>> (ar_seq));
    this->set_generator(gen);
    this->set_length(cardinal(0, seq->get_length()));
}

template <typename T>
lazy_sequence<T>::~lazy_sequence()
{
    delete gen;
}

template <typename T>
const T& lazy_sequence<T>::get_first() const
{
    if (!memoized)
    {
        throw std::out_of_range("Memoized is null");
    }
    return (*memoized).get_first();
}

template <typename T>
const T& lazy_sequence<T>::get_next() const
{
    if (gen == nullptr)
    {
        throw std::logic_error("Finite sequence has not generator");
    }
    T next_val = gen->get_next();
    memoized->append_element(next_val);
    return memoized->get_last();
}

template <typename T>
const T& lazy_sequence<T>::get_last_memoized() const
{
    if (!memoized)
    {
        throw std::out_of_range("Memoized is null");
    }
    return (*memoized).get_last();
}

template <typename T>
const T& lazy_sequence<T>::get(const ordinal& index) const
{
    auto it = begin();
    if (index.is_finite())
    {
        if (index.get_numerical_part() < memoized->get_length())
        {
            return memoized->get(index.get_numerical_part());
        }
        if (index.get_numerical_part() == memoized->get_length())
        {
            memoized->append_element((*this).get_next());
            return memoized->get(index.get_numerical_part());
        }
        it.set_idx((*memoized).get_length());
        while (it.get_num_part() <= index.get_numerical_part())
        {
            ++it;
            (*memoized).insert_element((*this).get_next(), it.get_index().get_numerical_part());
        }
    }
    if (!(index.is_finite()))
    {
        if (index.get_leading_term().get_coefficient() > length.get_aleph_idx() - 1)
        {
            throw std::invalid_argument("Bad index");
        
        }
        while (it.get_index() != index.get_numerical_part())
        {
            ++it;
            if (it.get_index() == index.get_numerical_part())
            {
                return this->gen->get_other_next();
            }
            this->gen->get_other_next();
        }
    }
    return (*memoized).get(index.get_numerical_part());
}

template <typename T>
const T& lazy_sequence<T>::get(int index) const
{
    return get(ordinal(index));
}

template <typename T>
const T lazy_sequence<T>::reduce(std::function<T(T,T)> func, const T& initial) const
{
    monad_adapter<array_sequence<T>> monada(*(memoized->clone()));
    return monada.reduce(func, initial);
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(int startidx, int endidx)
{
    return get_subsequence(ordinal(startidx), ordinal(endidx));
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(const ordinal& start_idx, const ordinal& end_idx)
{
    if (!memoized)
    {
        throw std::invalid_argument("No memoized");
    }
    ordinal start = start_idx;
    ordinal end = end_idx;
    if (!((end - start).is_finite()))
    {
        throw std::invalid_argument("Bad index");
    }
    array_sequence<T>* other = new array_sequence<T>();
    while (start != end)
    {
        other->append_element(this->get(start));
        start++;
    }   
    lazy_sequence<T>* result = new lazy_sequence(other, nullptr);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::concat(const lazy_sequence<T>& other)
{
    lazy_sequence<T>* result = new lazy_sequence();
    result->set_generator(new concat_generator(*this, other));
    return result;
}

template <typename T>
template <typename T2>
lazy_sequence<T2>* lazy_sequence<T>::map(std::function<T2(T)> func)
{
    lazy_sequence<T2> *result = new lazy_sequence<T2>();
    auto gen = new map_generator(*this, func);
    result->set_generator(gen);
    shared_ptr<array_sequence<T2>> result_memoized = shared_ptr<array_sequence<T2>>(new array_sequence<T2>());
    for (int i = 0; i < memoized->get_length(); i++)
    {
        result_memoized->append_element(func(memoized->get(i)));
    }
    result->set_memoized(result_memoized);
    result->set_length(this->length);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::where(std::function<bool(T)> func)
{
    lazy_sequence<T>* result = new lazy_sequence<T>();
    result->set_generator(new filter_generator(*this, func));
    shared_ptr<array_sequence<T>> result_memoized = shared_ptr<array_sequence<T>>(new array_sequence<T>());
    for (int i = 0; i < memoized->get_length(); i++)
    {
        if (func(memoized->get(i)))
        {
            result_memoized->append_element(memoized->get(i));
        }
    }
    result->set_memoized(result_memoized);
    result->set_length(this->length);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::set(const T& item, const ordinal& index)
{
    lazy_sequence<T>* result = new lazy_sequence();
    if (this->gen == nullptr)
    {
        result->set_generator(nullptr);
        shared_ptr<array_sequence<T>> result_memoized = shared_ptr<array_sequence<T>>(new array_sequence<T>());
        for (int i = 0; i < memoized->get_length(); i++)
        {
            if (i == index.get_numerical_part())
            {
                result_memoized->append_element(item);
            }
            else
            {
                result_memoized->append_element(memoized->get(i));
            }
        }
        result->set_memoized(shared_ptr<array_sequence<T>>(result_memoized));
        result->set_length(this->length);
        return result;
    }
    auto gen = new pull_generator(*this, item, index, ordinal(0));
    result->set_generator(gen);
    result->set_memoized(shared_ptr<array_sequence<T>>(new array_sequence<T>(*this->memoized)));
    result->set_length(this->length);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::set(const T& item, int index)
{
    return set(item, ordinal(index));
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::set_first(const T& item)
{
    return set(item, ordinal(0));
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::set_last_memoized(const T& item)
{
    return set(item, ordinal(this->get_length().get_finite_length() - 1));
}

template <typename T>
generator<T>* lazy_sequence<T>::get_generator()
{
    return this->gen;
}

template <typename T>
const generator<T>* lazy_sequence<T>::get_generator() const
{
    return this->gen;
}

template <typename T>
shared_ptr<array_sequence<T>> lazy_sequence<T>::get_memoized()
{
    return this->memoized;
}

template <typename T>
shared_ptr<array_sequence<T>> lazy_sequence<T>::get_memoized() const
{
    return this->memoized;
}

template <typename T>
cardinal lazy_sequence<T>::get_length()
{
    return this->length;
}

template <typename T>
const cardinal lazy_sequence<T>::get_length() const
{
    return this->length;
}

template <typename T>
lazy_sequence<T>& lazy_sequence<T>::set_generator(generator<T>* gen)
{
    this->gen = gen;
    return *this;
}

template <typename T>
lazy_sequence<T>& lazy_sequence<T>::set_memoized(shared_ptr<array_sequence<T>> memoized)
{
    this->memoized = memoized;
    return *this;
}

template <typename T>
lazy_sequence<T>& lazy_sequence<T>::set_length(cardinal length)
{
    this->length = length;
    return *this;
}

