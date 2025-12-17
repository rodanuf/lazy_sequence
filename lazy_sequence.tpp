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
ordinary& lazy_sequence<T>::ls_iterator::get_index()
{
    return cur_idx;
}

template <typename T>
const ordinary& lazy_sequence<T>::ls_iterator::get_index() const
{
    return cur_idx;
}

template <typename T>
bool lazy_sequence<T>::ls_iterator::has_omega()
{
    return cur_idx.get_omega();
}

template <typename T>
int lazy_sequence<T>::ls_iterator::get_num_part()
{
    return cur_idx.get_number();
}

template <typename T>
int lazy_sequence<T>::ls_iterator::get_omega_part()
{
    return cur_idx.get_omega();
}

template <typename T>
void lazy_sequence<T>::ls_iterator::increment_idx()
{
    cur_idx++;
}

template <typename T>
typename lazy_sequence<T>::ls_iterator lazy_sequence<T>::begin()
{
    return ls_iterator();
}

template <typename T>
typename lazy_sequence<T>::ls_iterator lazy_sequence<T>::end()
{
    return ls_iterator(this->length);
}

template <typename T>
lazy_sequence<T>::lazy_sequence() : buffer(nullptr), generator_(nullptr), length() {}

template <typename T>
lazy_sequence<T>::lazy_sequence(T* items, int size)
{
    if (items == nullptr)
    {
        throw std::invalid_argument("Array is null");
    }
    array_sequence<T>* seq = new array_sequence<T>(items, size);
    buffer(seq);
    generator_ = nullptr;
    length(0, size);
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const std::initializer_list<T>& list)
{
    array_sequence<T>* seq = new array_sequence<T>(list);
    buffer(seq);
    generator_ = nullptr;
    length(0, seq->get_length());
}

template <typename T>
lazy_sequence<T>::lazy_sequence(sequence<T>* seq)
{
    if (seq == nullptr)
    {
        throw std::invalid_argument("Seq is null");
    }
    array_sequence<T>* ar_seq = new array_sequence<T>(*seq);
    buffer(ar_seq);
    generator_ = nullptr;
    length(0, seq->get_length())
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
    length(1, 0);
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
    length(1, 0);
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
    length(1, 0);
}


template <typename T>
lazy_sequence<T>::lazy_sequence(const lazy_sequence<T>& other)
{
    buffer = other.buffer;
    this->generator_ = other.generator_->copy();
    length(other.length());
}

template <typename T>
lazy_sequence<T>::lazy_sequence(const lazy_sequence<T>& seq_one, const lazy_sequence<T>& seq_two)
{
    if (seq_one == nullptr || seq_two == nullptr)
    {
        throw std::invalid_argument("One of seqeunces is null");
    }
    buffer = seq_one.buffer;
    generator<T>* generator_ = new concat_generator(seq_one, seq_two);
    length = seq_one->length + seq_two->length;
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
    return get(ordinary(index));
}

template <typename T>
T& lazy_sequence<T>::get(const ordinary& index)
{
    auto it = begin();
    if (index.is_finite())
    {
        it.set_idx(buffer.get_length());
        while (it != index)
        {
            it++;
            (*buffer).insert_element(this->generator_->get_next(), (*iterator).get_index());
        }
    }
    if (!(index.is_finite()))
    {
        if (index.get_omega() != length.get_aleph_idx() - 1)
        {
            throw std::invalid_argument("Bad index");
        }
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
    return (*buffer).get_element(index);
}

template <typename T>
T& lazy_sequence<T>::reduce(std::function<T(T,T)> func)
{
    monad_adapter<sequence<T>> monada((*buffer).clone());
    auto result = monada.reduce(func);
    return result;
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(int startidx, int endidx)
{
    return get_subsequence(ordinary(startidx), ordinary(endidx));
}

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(ordinary& start_idx, ordinary& end_idx)
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
