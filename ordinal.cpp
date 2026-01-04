#include "ordinal.hpp"

ordinal::ordinal() : omega(0), number(0) {}
ordinal::ordinal(int omega, int number) : omega(omega), number(number)
{
    if (omega < 0 || number < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
}

ordinal::ordinal(const ordinal& other) : omega(other.omega), number(other.number) {}

ordinal::ordinal(const cardinal& length)
{
    omega = length.get_aleph_idx();
    number =  length.get_finite_length();
}

ordinal ordinal::operator+(const ordinal& other)
{
    if (this->is_finite())
    {
        return ordinal(omega, number + other.number);
    }
    return ordinal(omega + other.omega, other.number);
}

ordinal ordinal::operator+(int num)
{
    return ordinal(omega, number + num);
}

ordinal ordinal::operator-(const ordinal& other)
{
    return ordinal(omega - other.omega, number - other.number);
}

ordinal ordinal::operator++(int)
{
    return ordinal(omega, number++);
}

ordinal ordinal::make_from(int num)
{
    return ordinal(0, num);
}

ordinal& ordinal::operator=(int num)
{
    number = num;
    return *this;
}

ordinal& ordinal::operator+=(int num)
{
    number += num;
    return *this;
}

ordinal& ordinal::operator=(const ordinal& other)
{
    omega = other.omega;
    number = other.number;
    return *this;
}

ordinal& ordinal::operator+=(const ordinal& other)
{
    omega += other.omega;
    number += other.number;
    return *this;
}

ordinal& ordinal::operator-=(const ordinal& other)
{
    if (*this < other)
    {
        throw std::invalid_argument("Invalid arguments");
    }
    omega -= other.omega;
    number -= other.number;
    return *this;
}

ordinal& ordinal::operator++()
{
    number++;
    return *this;
}

bool ordinal::operator==(const ordinal& other)
{
    return omega == other.omega && number == other.number;
}

bool ordinal::operator==(int num)
{
    if (omega == 0)
    {
        return number == num;
    }
    return false;
}

bool ordinal::operator!=(const ordinal& other)
{
    return omega != other.omega || number != other.number;
}

bool ordinal::operator!=(int num)
{
    return !(*this == num);
}

bool ordinal::operator<(const ordinal& other)
{
    if (omega < other.omega)
    {
        return true;
    }
    else if (omega == other.omega)
    {
        return number < other.number;
    }
    return false;
}

bool ordinal::operator<(int num)
{
    if (omega == 0)
    {
        return number < num;
    }
    return false;
}

bool ordinal::operator>(const ordinal& other)
{
    return !(*this < other) && *this != other;
}

bool ordinal::operator>(int num)
{
    return !(*this < num) && *this != num;
}

bool ordinal::operator<=(const ordinal& other)
{
    return !(*this > other);
}

bool ordinal::operator<=(int num)
{
    return !(*this > num);
}

bool ordinal::operator>=(const ordinal& other)
{
    return !(*this < other);
}

bool ordinal::operator>=(int num)
{
    return !(*this < num);
}

bool ordinal::is_finite()
{
    return number && omega;
}

int ordinal::get_omega()
{
    return omega;
}

const int ordinal::get_omega() const
{
    return omega;
}

int ordinal::get_number()
{
    return number;
}

const int ordinal::get_number() const
{
    return number;
}

void ordinal::set_omega(int omega)
{
    if (omega < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
    this->omega = omega;
}

void ordinal::increase_omega()
{
    omega++;
}

void ordinal::set_number(int number)
{
    if (number < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
    this->number = number;
}

void ordinal::increase_number()
{
    number++;
}