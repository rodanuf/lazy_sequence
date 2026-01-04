#include "lazy_sequence.hpp"
#include <stdexcept>

template <typename T>
lazy_sequence<T>::lazy_sequence() : memoized(nullptr), generator_(nullptr), length() {}

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
    this->set_length(0, size);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const std::initializer_list<T>& list)
{
    array_sequence<T>* seq = new array_sequence<T>(list);
    this->set_memoized(seq);
    this->set_generator(nullptr);
    this->set_length(0, seq->get_length());
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const std::initializer_list<T>& list, std::function<T(T)> other_generator)
{
    if (list.size() == 0)
    {
        throw std::invalid_argument("List size is zero");
    }

    array_sequence<T>* seq = new array_sequence<T>(list);
    this->set_memoized(seq); // возможна утечка 
    this->set_generator(unary_generator(this, other_generator));
    this->set_length(1, 0);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const std::initializer_list<T>& list, std::function<T(T, T)> other_generator)
{
    if (list.size() < 2)
    {
        throw std::invalid_argument("List size is less than 2");
    }

    array_sequence<T>* seq = new array_sequence<T>(list);
    this->set_memoized(seq);
    this->set_generator(binary_generator(this, other_generator));
    this->set_length(1, 0);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const std::initializer_list<T>& list, int arity, std::function<T(sequence<T>*)> other_generator)
{
    if (list.size() < arity)
    {
        throw std::invalid_argument("List size is less than arity");
    }

    array_sequence<T>* seq = new array_sequence<T>(list);
    this->set_memoized(seq);
    this->set_generator(nary_generator(this, arity, other_generator));
    this->set_length(1, 0);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }

    array_sequence<T>* ar_seq = new array_sequence<T>(*seq);
    this->set_memoized(ar_seq);
    this->set_generator(nullptr);
    this->set_length(0, seq->get_length())
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, std::function<T(T)> other_generator)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }
    if (seq->get_length() == 0)
    {
        throw std::invalid_argument("Seq length is zero");
    }

    this->set_memoized(seq);
    this->set_generator(unary_generator(this, other_generator));
    this->set_length(1, 0);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, std::function<T(T,T)> other_generator)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }
    if (seq->get_length() < 2)
    {
        throw std::invalid_argument("Seq length is less than 2");
    }

    this->set_memoized(seq);
    this->set_generator(binary_generator(this, other_generator));
    this->set_length(1, 0);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, int arity, std::function<T(sequence<T>*)> other_generator)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }
    if (seq->get_length() < arity)
    {
        throw std::invalid_argument("Seq length is less than 2");
    }

    this->set_memoized(seq);
    this->set_generator(nary_generator(this, arity, other_generator));
    this->set_length(1, 0);
}


template <typename T>
lazy_sequence<T>::lazy_sequence(const lazy_sequence<T>& other)
{
    this->set_memoized(other.get_memoized());
    this->set_generator(other.get_generator());
    this->set_length(other.length());
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const lazy_sequence<T>& seq_one, const lazy_sequence<T>& seq_two)
{
    if (seq_one == nullptr || seq_two == nullptr)
    {
        throw std::invalid_argument("One of seqeunces is null");
    }
    this->set_memoized(seq_one.get_memoized());
    this->set_generator(new concatenation_generator(&seq_one, &seq_two, seq_one.get_generator(), seq_two.get_generator()));
    length = seq_one.length() + seq_two.length();
}

template <typename T>
lazy_sequence<T>::~lazy_sequence()
{
    memoized.~shared_ptr();
    delete generator_;
}

template <typename T>
T& lazy_sequence<T>::get_first()
{
    (*memoized).get_first();
}

template <typename T>
T& lazy_sequence<T>::get_last()
{
    (*memoized).get_last();
}

template <typename T>
T& lazy_sequence<T>::get(const ordinal& index)
{
    auto it = begin();
    if (index.is_finite())
    {
        it.set_idx(memoized.get_length());
        while (it != index)
        {
            it++;
            (*memoized).insert_element(this->generator_->get_next(), (*iterator).get_index());
        }
    }
    if (!(index.is_finite()))
    {
        if (index.get_omega() != length.get_aleph_idx() - 1)
        {
            throw std::invalid_argument("Bad index");
        }
        it.set_idx(memoized.get_length());
        while (it != index.get_number())
        {
            it++;
            if (it == index.get_number())
            {
                return this->generator_->get_next_other();
            }
            this->generator_->get_next_other();
        }
    }
    return (*memoized).get_element(index);
}

template <typename T>
T &lazy_sequence<T>::get(int index)
{
    return get(ordinal(index));
}

template <typename T>
T& lazy_sequence<T>::reduce(std::function<T(T,T)> func)
{
    monad_adapter<sequence<T>> monada((*memoized).clone());
    auto result = monada.reduce(func);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(int startidx, int endidx)
{
    return get_subsequence(ordinal(startidx), ordinal(endidx));
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(ordinal& start_idx, ordinal& end_idx)
{
    if (start_idx.get_omega() != start_idx.get_omega())
    {
        throw std::invalid_argument("Bad index");
    }
    array_sequence<T>* other = new array_sequence<T>();
    while (start_idx != end_idx)
    {
        other->append_element(this->get(start_idx));
        start_idx++;
    }   
    lazy_sequence<T>* result = new lazy_sequence(other, generator_);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::concat(lazy_sequence<T>* other)
{
    if (other == nullptr)
    {
        throw std::invalid_argument("Other sequence is nullptr");
    }
    lazy_sequence<T>* result = new lazy_sequence(this, other);
    return result;
}

template <typename T>
template <typename T2>
lazy_sequence<T>* lazy_sequence<T>::map(std::function<T2(T)> func)
{
    lazy_sequence<T>* result = new lazy_sequence();
    result = result->set_generator(new map_generator(func, this));
    result = result->set_memoized(this->memoized);
    result = result->set_length(this->length);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::where(std::function<bool(T)> func)
{
    lazy_sequence<T>* result = new lazy_sequence();
    result = result->set_generator(new filter_generator(func, this));
    result = result->set_memoized(this->memoized);
    result = result->set_length(this->length);
    return result;
}

template <typename T>
lazy_sequence<T>& lazy_sequence<T>::set(T& item, ordinal index)
{
    lazy_sequence<T>* result = new lazy_sequence();
    result = result->set_generator(new pull_generator(this, item, index));
    result = result->set_memoized(this->memoized);
    result = result->set_length(this->length);
    return result;
}

template <typename T>
lazy_sequence<T>& lazy_sequence<T>::set(T& item, int index)
{
    return set(item, ordinal(index));
}