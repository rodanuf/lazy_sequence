#include "counter.hpp"

counter::counter()
{
    *cur_idx = 0;
}

counter::counter(int index)
{
    this->set_idx(index);
}

counter::counter(const ordinal &other)
{
    this->set_idx(other);
}

counter counter::operator++(int)
{
    counter tmp(this->get_index());
    (*cur_idx)++;
    return tmp;
}

counter &counter::operator++()
{
    (*cur_idx)++;
    return *this;
}

counter &counter::increment_idx()
{
    (*cur_idx)++;
    return *this;
}

counter &counter::set_idx(int omega, int num)
{
    (*cur_idx) = ordinal(term(omega, num));
    return *this;
}

counter &counter::set_idx(const ordinal &idx)
{
    *cur_idx = idx;
    return *this;
}

counter &counter::set_idx(int num)
{
    if (num < 0)
    {
        throw std::invalid_argument("Index is negative");
    }
    *cur_idx = num;
    return *this;
}

const ordinal &counter::get_index() const
{
    return *cur_idx;
}

bool counter::has_omega()
{
    return (*cur_idx).is_finite();
}

int counter::get_num_part()
{
    return (*cur_idx).get_numerical_part();
}
