#include "ordinary.hpp"

ordinary::ordinary() : omega(0), number(0) {}
ordinary::ordinary(int omega, int number) : omega(omega), number(number)
{
    if (omega < 0 || number < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
}

ordinary::ordinary(const ordinary &other) : omega(other.omega), number(other.number) {}

ordinary ordinary::operator+(const ordinary &other)
{
    if (is_finite())
    {
        return ordinary(omega, number + other.number);
    }
    return ordinary(omega + other.omega, other.number);
}

ordinary ordinary::operator+(int num)
{
    return ordinary(omega, number + num);
}

ordinary ordinary::operator-(const ordinary &other)
{
    return ordinary(omega - other.omega, number - other.number);
}

ordinary ordinary::operator++(int)
{
    return ordinary(omega, number++);
}

ordinary ordinary::make_from(int num)
{
    return ordinary(0, num);
}

ordinary& ordinary::operator+=(int num)
{
    number += num;
    return *this;
}

ordinary& ordinary::operator+=(const ordinary& other)
{
    omega += other.omega;
    number += other.number;
    return *this;
}

ordinary& ordinary::operator-=(const ordinary& other)
{
    if (*this < other)
    {
        throw std::invalid_argument("Invalid arguments");
    }
    omega -= other.omega;
    number -= other.number;
    return *this;
}

ordinary& ordinary::operator++()
{
    number++;
    return *this;
}

bool ordinary::operator==(const ordinary &other)
{
    return omega == other.omega && number == other.number;
}

bool ordinary::operator==(int num)
{
    if (omega == 0)
    {
        return number == num;
    }
    return false;
}

bool ordinary::operator!=(const ordinary &other)
{
    return omega != other.omega || number != other.number;
}

bool ordinary::operator!=(int num)
{
    return !(*this == num);
}

bool ordinary::operator<(const ordinary &other)
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

bool ordinary::operator<(int num)
{
    if (omega == 0)
    {
        return number < num;
    }
    return false;
}

bool ordinary::operator>(const ordinary &other)
{
    return !(*this < other) && *this != other;
}

bool ordinary::operator>(int num)
{
    return !(*this < num) && *this != num;
}

bool ordinary::operator<=(const ordinary &other)
{
    return !(*this > other);
}

bool ordinary::operator<=(int num)
{
    return !(*this > num);
}

bool ordinary::operator>=(const ordinary &other)
{
    return !(*this < other);
}

bool ordinary::operator>=(int num)
{
    return !(*this < num);
}

int ordinary::get_omega()
{
    return omega;
}

const int ordinary::get_omega() const
{
    return omega;
}

int ordinary::get_number()
{
    return number;
}

const int ordinary::get_number() const
{
    return number;
}

void ordinary::set_omega(int omega)
{
    if (omega < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
    this->omega = omega;
}

void ordinary::increase_omega()
{
    omega++;
}

void ordinary::set_number(int number)
{
    if (number < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }
    this->number = number;
}

void ordinary::increase_number()
{
    number++;
}