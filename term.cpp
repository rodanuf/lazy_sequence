#include "term.hpp"
#include <stdexcept>

term::term() : coefficient(0), exponent(0) {}

term::term(int coefficient, int exponent)
{
    if (coefficient < 0 || exponent < 0 )
    {
        throw std::invalid_argument("Coefficient or exponent is negative");
    }
}

term::term(const term& other)
{
    coefficient = other.coefficient;
    exponent = other.exponent;
}

term term::operator++(int)
{
    term temp = *this;
    *this += 1;
    return temp;
}

term& term::operator=(const term& other)
{
    coefficient = other.coefficient;
    exponent = other.exponent;
    return *this;
}

term& term::operator+=(const term& other)
{
    if (exponent != other.exponent)
    {
        throw std::invalid_argument("Exponent parts are not equal");
    }

    coefficient += other.coefficient;
    return *this;
}

term& term::operator+=(int num)
{
    if (exponent != 0)
    {
        throw std::invalid_argument("Exponent must be zero");
    }

    coefficient += num;
    return *this;    
}

term& term::set_coefficient(int coefficient)
{
    this->coefficient = coefficient;
    return *this;
}

term& term::set_exponent(int exponent)
{
    this->exponent = exponent;
    return *this;
}

bool term::operator==(const term& other)
{
    if (coefficient == other.coefficient && exponent == other.exponent)
    {
        return true;
    }
    return false;
}

bool term::operator!=(const term& other)
{
    return !(*this == other);
}

bool term::operator<(const term& other)
{
    if (exponent < other.exponent || (exponent == other.exponent && coefficient < other.coefficient))
    {
        return true;
    }
    
    return false;
}

bool term::operator>(const term& other)
{
    if (exponent > other.exponent || (exponent == other.exponent && coefficient > other.coefficient))
    {
        return true;
    }

    return false;
}

bool term::operator<=(const term& other)
{
    return !(*this > other);
}

bool term::operator>=(const term& other)
{
    return !(*this < other);
}

int& term::get_coefficient()
{
    return coefficient;
}

int& term::get_exponent()
{
    return exponent;
}

const int& term::get_coefficient() const
{
    return coefficient;
}

const int& term::get_exponent() const
{
    return exponent;
}