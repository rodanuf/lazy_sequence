#include "counter.hpp"

counter::counter()
{
    cur_idx = 0;
}

counter::counter(int index)
{
    this->set_idx(index);
}

counter::counter(const ordinal &other)
{
    this->set_idx(other);
}

counter::counter(const counter &other)
{
    this->cur_idx = other.cur_idx;
}

counter counter::operator++(int)
{
    counter tmp(*this);
    this->cur_idx++;
    return tmp;
}

counter &counter::operator++()
{
    cur_idx++;
    return *this;
}

counter &counter::increment_idx()
{
    cur_idx++;
    return *this;
}

counter &counter::set_idx(int omega, int num)
{
    cur_idx.set_omega(omega);
    cur_idx.set_number(num);
    return *this;
}

counter &counter::set_idx(const ordinal &idx)
{
    cur_idx = idx;
    return *this;
}

counter &counter::set_idx(int num)
{
    if (num < 0)
    {
        throw std::invalid_argument("Index is negative");
    }
    cur_idx = num;
    return *this;
}

ordinal &counter::get_index()
{
    return cur_idx;
}

const ordinal &counter::get_index() const
{
    return cur_idx;
}

bool counter::has_omega()
{
    return cur_idx.get_omega();
}

int counter::get_num_part()
{
    return cur_idx.get_number();
}

int counter::get_omega_part()
{
    return cur_idx.get_omega();
}
