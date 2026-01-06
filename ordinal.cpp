#include "ordinal.hpp"

ordinal::ordinal() : form(), numerical_part(0) {}
ordinal::ordinal(int number)
{
    if (number < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }

    form.append_element(term(number, 0));
    numerical_part = number;
}

ordinal::ordinal(const cantor_form& other)
{
    form = other;
    numerical_part = form.get_last().get_coefficient();
}

ordinal::ordinal(const ordinal& other) : form(other.form), numerical_part(other.numerical_part) {}

ordinal ordinal::operator+(const ordinal& other)
{
    if (other > *this)
    {
        return ordinal(other);
    }
    cantor_form buffer_form = this->get_form();
    cantor_form other_buffer_form = other.get_form();
    for (int i = 0; i < buffer_form.get_length(); i++)
    {
        if (other_buffer_form.get(i).get_exponent() == buffer_form.get(buffer_form.get_length() - i).get_exponent())
        {
            buffer_form.get(buffer_form.get_length() - i).set_coefficient(other_buffer_form.get(i).get_coefficient() + this->get_form().get(i).get_coefficient());
        }
        if (other_buffer_form.get(i).get_exponent() > buffer_form.get(buffer_form.get_length() - i).get_exponent())
        {
            buffer_form.insert_element(other_buffer_form.get(i));
        }
    }
    return ordinal(buffer_form); 
}

ordinal ordinal::operator+(int num)
{
    cantor_form buffer_form = this->get_form();
    buffer_form.get_last() += num;
    return ordinal(buffer_form);
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