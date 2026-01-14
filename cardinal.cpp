#include "cardinal.hpp"

cardinal::cardinal() : aleph_idx(0), finite_length(0) {}

cardinal::cardinal(int aleph_idx, int finite_length = 0)
{
    if (aleph_idx < 0 || finite_length < 0)
    {
        throw std::invalid_argument("Invalid argument");        
    }
}


cardinal cardinal::operator+(const cardinal& other)
{
    return cardinal(aleph_idx + other.aleph_idx, finite_length > other.finite_length ? finite_length : other.finite_length);
}

cardinal& cardinal::operator+=(const cardinal& other)
{
    aleph_idx += other.aleph_idx;
    finite_length = finite_length > other.finite_length ? finite_length : other.finite_length;
    return *this;
}

int cardinal::get_aleph_idx() 
{
    return aleph_idx;
}

const int cardinal::get_aleph_idx() const
{
    return aleph_idx;
}

int cardinal::get_finite_length() 
{
    return finite_length;
}

const int cardinal::get_finite_length() const
{
    return finite_length;
}