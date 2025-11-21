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
    unary_function = unfunc;
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
    generator_storage = *((owner->buffer.get())->get_subsequence(last_idx - 1, last_idx));
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
    return binary_function(generator_storage.get_first(), generator_storage.get_last());
}

template <typename T>
lazy_sequence<T>::nary_generator::nary_generator(lazy_sequence<T>* owner, int ar, std::function<T(sequence<T>*)> seqfunc)
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
T& lazy_sequence<T>::nary_generator::get_next()
{
    T& result = sequence_function(&generator_storage);
    generator_storage.append_element(result);
    generator_storage.remove_at(0);
    return result;
}

template <typename T>
bool lazy_sequence<T>::nary_generator::has_next()
{
    return this->get_next();
}

template <typename T>
lazy_sequence<T>::skip_generator::skip_generator(lazy_sequence<T>* owner, lazy_sequence<T>* parent, int start_skip, int end_skip)
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
void lazy_sequence<T>::skip_generator::skip()
{
    if (start_idx == end_idx)
    {
        return;
    }
    for (int i = start_idx; i <= end_idx; i++)
    {
        (*parent).generator_->get_next();
    }
    start_idx = 0;
    end_idx = 0;
}

template <typename T>
void lazy_sequence<T>::skip_generator::skip(int new_start, int new_end)
{
    this->start_idx = new_start;
    this->end_idx = new_end;
    skip();
}

template <typename T>
T& lazy_sequence<T>::skip_generator::get_next()
{
    skip();
    return (*parent).generator_->get_next();
}

template <typename T>
bool lazy_sequence<T>::skip_generator::has_next()
{
    if ( start_idx == end_idx)
    {
        return (*parent).generator_->has_next();
    }
}

template <typename T>
lazy_sequence<T>::insert_generator::insert_generator(shared_ptr<lazy_sequence<T>> parent, shared_ptr<lazy_sequence<T>> other)
{
    if (*parent == nullptr || *other == nullptr)
    this->parent = parent;
    this->other = other;
}

template <typename T>
lazy_sequence<T>::insert_generator::insert_generator(shared_ptr<lazy_sequence<T>> parent, T& elem)
{
    this->parent = parent;
    element = elem;
}

template <typename T>
T& lazy_sequence<T>::insert_generator::get_element()
{
    return element;
}

template <typename T>
T& lazy_sequence<T>::insert_generator::get_other_next()
{
    return (*other).generator_->get_next();
}

template <typename T>
T& lazy_sequence<T>::insert_generator::get_next()
{
    return (*parent).generator_.get_next();
}

template <typename T>
bool lazy_sequence<T>::insert_generator::has_next()
{
    return (*parent).generator_.has_next();
}

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
    if ((*iterator).get_index >= index)
    {
        (*buffer).get(index);
    }
    else
    {
        while ((*iterator).get_index != index - 1)
        {
            this->generator_->get_next();
            (*iterator)++;
        }
    }
}

//TODO
template <typename T>
T& lazy_sequence<T>::reduce(std::function<T(T,T)> func)
{}//

template <typename T>
lazy_sequence<T>* lazy_sequence<T>::get_subsequence(int startidx, int endidx)
{

}