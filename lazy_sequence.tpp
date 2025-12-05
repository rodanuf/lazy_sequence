#include "lazy_sequence.hpp"
#include <stdexcept>

template <typename T>
lazy_sequence<T>::ls_iterator::ls_iterator()
{
    cur_idx = 0;
}

template <typename T>
lazy_sequence<T>::ls_iterator::ls_iterator(int index)
{
    cur_idx = index;
}

template <typename T>
lazy_sequence<T>::ls_iterator::ls_iterator(const ls_iterator& other)
{
    this->cur_idx = other.cur_idx;
}

template <typename T>
typename lazy_sequence<T>::ls_iterator& lazy_sequence<T>::ls_iterator::operator++()
{
    cur_idx++;
    return *this;
}

template <typename T>
typename lazy_sequence<T>::ls_iterator lazy_sequence<T>::ls_iterator::operator++(int)
{
    ls_iterator tmp(*this);
    this->cur_idx++;
    return tmp;
}

template <typename T>
int lazy_sequence<T>::ls_iterator::get_index()
{
    return cur_idx;
}

template <typename T>
void lazy_sequence<T>::ls_iterator::increment_idx()
{
    cur_idx++;
}


template <typename T>
lazy_sequence<T>::lazy_sequence() : buffer(nullptr), generator_(nullptr) {}

template <typename T>
lazy_sequence<T>::lazy_sequence(T* items, int size)
{
    array_sequence<T> seq(items, size);
    buffer(&seq);
    generator_ = nullptr;
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }

    buffer(seq);
    generator_ = nullptr;
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, std::function<T(T)> other_generator)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }

    buffer(seq);
    generator_(unary_generator(this, other_generator));
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, std::function<T(T,T)> other_generator)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }

    buffer(seq);
    generator_(binary_generator(this, other_generator));
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq, int arity, std::function<T(sequence<T>*)> other_generator)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }

    buffer(seq);
    generator_(nary_generator(this, arity, other_generator));
}


template <typename T>
lazy_sequence<T>::lazy_sequence(const lazy_sequence<T>& other)
{
    sequence<T>* seq = other.buffer->clone();
    this->buffer(seq);
    this->generator_ = other.generator_->copy();
}

template <typename T>
lazy_sequence<T>::~lazy_sequence()
{
    buffer.~uniq_ptr();
    delete generator_;
    iterator.~shared_ptr();
}

template <typename T>
T& lazy_sequence<T>::get_first()
{
    (*buffer).get_first();
}

template <typename T>
T& lazy_sequence<T>::get_last()
{
    (*buffer).get_last();
}

template <typename T>
T& lazy_sequence<T>::get(int index)
{
    if (index < 0)
    {
        throw std::invalid_argument("Bad index");
    }
    if (index > (*iterator).get_index())
    {
        while ((*iterator).get_index != index)
        {
            (*iterator)++;
            (*buffer).insert_element(this->generator_->get_next(), (*iterator).get_index());
        }
    }
    return (*buffer).get_element(index);
}

template <typename T>
T& lazy_sequence<T>::reduce(std::function<T(T,T)> func)
{
    monad_adapter<sequence<T>> monada((*buffer).clone());
    new result = monada.reduce(func);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(int startidx, int endidx)
{
    if (startidx > endidx || startidx < 0 || endidx < 0)
    {
        throw std::invalid_argument("Bad index");
    }
    while ((*iterator).get_index != startidx - 1)
    {
        this->generator_->get_next();
        (*iterator)++;
    }
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::concat(lazy_sequence<T>* other)
{
    if (other == nullptr)
    {
        throw std::invalid_argument("Other sequence is nullptr");
    }
    while (other->generator_->has_next())
    {
        this->generator_->get_next();
        (*iterator)++;
    }
}

template <typename T>
lazy_sequence<T>