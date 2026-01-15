#include "ordinal.hpp"
#include <algorithm>

ordinal::ordinal() : form(), numerical_part(0) {}
ordinal::ordinal(int number) : form(), numerical_part(0)
{
    if (number < 0)
    {
        throw std::invalid_argument("Invalid arguments");
    }

    form.append_element(term(number, 0));
    numerical_part = number;
}

ordinal::ordinal(const std::initializer_list<term>& list) : form(list), numerical_part(0)
{
    this->normalize();
    if (form.get_last().get_exponent() == 0)
    {
        numerical_part = form.get_last().get_coefficient();
    }
}

ordinal::ordinal(const cantor_form& other)
{
    form = other;
    numerical_part = form.get_last().get_coefficient();
}

ordinal::ordinal(const term& other)
{
    form.append_element(other);
    numerical_part = 0;
    if (this->is_finite())
    {
        numerical_part = other.get_coefficient();
    }
}

ordinal::ordinal(const ordinal& other) : form(other.form), numerical_part(other.numerical_part) {}

ordinal ordinal::operator+(const ordinal& other)
{
    if (*this < other && !(other.is_finite()))
    {
        return ordinal(other);
    }
    cantor_form buffer_form = this->get_form();
    cantor_form other_buffer_form = other.get_form();
    int index = buffer_form.get_length() - other_buffer_form.get_length();
    int last_idx = (*this).get_form().get_length() - 1;
    for (int i = 0; i < other.get_form().get_length(); i++)
    {
        if ((*this).get_form().get(last_idx - i).get_exponent() < other_buffer_form.get(i).get_exponent())
        {
            buffer_form.remove_at(last_idx - i);
        }
    }
    if (buffer_form.get_last().get_exponent() == other_buffer_form.get_first().get_exponent())
    {
        buffer_form.get_last().set_coefficient((*this).get_form().get(index).get_coefficient() + other.get_leading_term().get_coefficient());
        other_buffer_form.remove_at(0);
    }
    buffer_form.concat(other_buffer_form);
    return ordinal(buffer_form); 
}

ordinal ordinal::operator+(int num)
{
    ordinal result = *this + ordinal(num);
    return result;
}

ordinal ordinal::operator-(const ordinal& other)
{
    if (*this < other)
    {
        throw std::invalid_argument("Right operand must be less than left operand");
    }
    if (other.get_leading_term().get_exponent() < (*this).get_leading_term().get_exponent())
    {
        return ordinal(this->get_form());
    }

    cantor_form buffer_form = this->get_form();
    cantor_form other_buffer_form = other.get_form();
    for (int i = buffer_form.get_length() - 1; i > 0; i--)
    {
        if (buffer_form.get(i).get_exponent() == other_buffer_form.get(i).get_exponent())
        {
            buffer_form.get(i).set_coefficient((*this).get_form().get(i).get_coefficient() - other_buffer_form.get(i).get_coefficient());
        }
    }

    ordinal difference = ordinal(buffer_form);
    if (difference.is_finite())
    {
        difference.get_form().get(buffer_form.get_length() - 1).set_coefficient(buffer_form.get_last().get_coefficient() - other_buffer_form.get_last().get_coefficient()); 
    }
    difference.set_num(buffer_form.get_last().get_coefficient() - other_buffer_form.get_last().get_coefficient());
    if (difference.get_form().get_length() > 1)
    {
        difference.normalize();
    }
    return difference;
}

ordinal ordinal::operator-(int num)
{
    ordinal result = *this - ordinal(num);
    return result;
}

ordinal ordinal::operator*(const ordinal& other)
{
    cantor_form buffer_form = (*this).get_form();
    cantor_form other_buffer_form = other.get_form();
    ordinal result;

    for (int i = 0; i < buffer_form.get_length(); i++)
    {
        for (int j = 0; j < other_buffer_form.get_length(); j++)
        {
            int coeff = buffer_form[i].get_coefficient() * other_buffer_form[j].get_coefficient();
            int exp;
            exp = buffer_form[i].get_exponent() + other_buffer_form[j].get_exponent();

            bool has_same_exponent = false;
            for (int k = 0; k < result.get_form().get_length(); k++)
            {
                if (result[k].get_exponent() == exp)
                {
                    result[k].get_coefficient() += coeff;
                    has_same_exponent = true;
                }
            }

            if (!has_same_exponent)
            {
                result.get_form().append_element(term(coeff, exp));
            }
        }
    }

    return result;
}

ordinal ordinal::operator*(int num)
{
    return ordinal((*this) * ordinal(num));
}

ordinal ordinal::operator++(int)
{
    ordinal result(*this);
    ++(*this);
    return result;
}

ordinal &ordinal::operator=(int num)
{
    this->get_form().clear();
    this->get_form().append_element(term(num, 0));
    this->numerical_part = num;
    return *this;
}

ordinal &ordinal::operator+=(int num)
{
    this->get_form().get_last() += num;
    this->numerical_part += num;
    return *this;
}

ordinal &ordinal::operator=(const ordinal &other)
{
    this->get_form() = other.get_form();
    this->numerical_part = other.numerical_part;
    return *this;
}

ordinal &ordinal::operator+=(const ordinal &other)
{
    if (*this < other)
    {
        *this = other;
        return *this;
    }
    cantor_form buffer_form = this->get_form();
    cantor_form other_buffer_form = other.get_form();
    int index = buffer_form.get_length() - other_buffer_form.get_length();
    int last_idx = buffer_form.get_length() - 1;
    for (int i = 0; i < other.get_form().get_length(); i++)
    {
        if ((buffer_form).get(last_idx - i).get_exponent() < other_buffer_form.get(i).get_exponent())
        {
            (*this).get_form().remove_at(last_idx - i);
        }
    }
    if ((*this).get_form().get_last().get_exponent() == other_buffer_form.get_first().get_exponent())
    {
        (*this).get_form().get_last().set_coefficient(buffer_form.get(index).get_coefficient() + other.get_leading_term().get_coefficient());
        other_buffer_form.remove_at(0);
    }
    (*this).get_form().concat(other_buffer_form);
    if ((*this).get_form().get_last().get_exponent() == 0)
    {
        numerical_part = (*this).get_form().get_last().get_coefficient();
    }
    return *this;
}

ordinal &ordinal::operator-=(const ordinal &other)
{
    if (*this < other)
    {
        throw std::invalid_argument("Right operand must be less than left operand");
    }
    if (other.get_leading_term().get_exponent() < (*this).get_leading_term().get_exponent())
    {
        return *this;
    }

    cantor_form buffer_form = this->get_form();
    cantor_form other_buffer_form = other.get_form();
    for (int i = buffer_form.get_length() - 1; i > 0; i--)
    {
        buffer_form.get(i).set_coefficient((*this).get_form().get(i).get_coefficient() - other_buffer_form.get(i).get_coefficient());
    }

    ordinal result = ordinal(buffer_form);
    if (result.is_finite())
    {
        result.get_form().get(buffer_form.get_length() - 1).set_coefficient(buffer_form.get_last().get_coefficient() - other_buffer_form.get_last().get_coefficient());
    }
    result.set_num(buffer_form.get_last().get_coefficient() - other_buffer_form.get_last().get_coefficient());
    if (result.get_form().get_length() > 1)
    {
        result.normalize();
    }

    *this = result;
    return *this;
}

ordinal &ordinal::operator++()
{
    this->get_form().get_last()++;
    numerical_part++;
    return *this;
}

ordinal &ordinal::add_term(const term &t)
{
    (*this).get_form().append_element(t);
    (*this).normalize();
    return *this;
}

ordinal &ordinal::normalize()
{
    int n = form.get_length();
    if (n <= 1)
        return *this;

    auto sift_down = [this](int start, int heap_size)
    {
        int parent = start;
        while (true)
        {
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;
            int largest = parent;

            if (left < heap_size && form.get(left) > form.get(largest))
                largest = left;
            if (right < heap_size && form.get(right) > form.get(largest))
                largest = right;

            if (largest == parent)
                break;

            term tmp = form[parent];
            form[parent] = form[largest];
            form[largest] = tmp;

            parent = largest;
        }
    };

    for (int i = n / 2 - 1; i >= 0; --i)
    {
        sift_down(i, n);
    }

    for (int i = n - 1; i > 0; --i)
    {
        term tmp = form[0];
        form[0] = form[i];
        form[i] = tmp;

        sift_down(0, i);
    }

    for (int i = 0; i < n / 2; ++i)
    {
        term tmp = form[i];
        form[i] = form[n - 1 - i];
        form[n - 1 - i] = tmp;
    }

    for (int i = form.get_length() - 1; i >= 0; i--)
    {
        if (form.get(i).get_coefficient() == 0)
        {
            form.remove_at(i);
        }
    }

    cantor_form merged;

    int i = 0;
    while (i < n)
    {
        int current_exp = form.get(i).get_exponent();
        int total_coeff = form.get(i).get_coefficient();

        while (i + 1 < n && form.get(i + 1).get_exponent() == current_exp)
        {
            ++i;
            total_coeff += form.get(i).get_coefficient();
        }
        merged.append_element(term(total_coeff, current_exp));

        ++i;
    }

    form = std::move(merged);
    return *this;
}

bool ordinal::operator==(const ordinal &other) const
{
    if ((*this).get_form().get_length() != other.get_form().get_length())
    {
        return false;
    }

    for (int i = 0; i < (*this).get_form().get_length(); i++)
    {
        if ((*this).get_form().get(i) != other.get_form().get(i))
        {
            return false;
        }
    }

    return true;
}

bool ordinal::operator==(int num) const
{
    if ((*this).is_finite())
    {
return numerical_part == num;
    }
    return false;
}

bool ordinal::operator!=(const ordinal &other)
{
    return !(*this == other);
}

bool ordinal::operator!=(int num)
{
    return !(*this == num);
}

bool ordinal::operator<(const ordinal &other)
{
    if ((*this).get_leading_term() < other.get_leading_term())
    {
        return true;
    }
    else if ((*this) == other)
    {
        return false;
    }

    for (int i = 0; i < ((*this).get_form().get_length() < other.get_form().get_length() ? (*this).get_form().get_length() : other.get_form().get_length()); i++)
    {
        if ((*this).get_form().get(i) > other.get_form().get(i))
        {
            return false;
        }
    }

    return true;
}

bool ordinal::operator<(int num)
{
    if ((*this).is_finite())
    {
        return (*this).get_form().get_last() < term(num, 0);
    }
    return false;
}

bool ordinal::operator>(const ordinal &other)
{
    return !(*this < other) && *this != other;
}

bool ordinal::operator>(int num)
{
    return !(*this < num) && *this != num;
}

bool ordinal::operator<=(const ordinal &other)
{
    return !(*this > other);
}

bool ordinal::operator<=(int num)
{
    return !(*this > num);
}

bool ordinal::operator>=(const ordinal &other)
{
    return !(*this < other);
}

bool ordinal::operator>=(int num)
{
    return !(*this < num);
}

bool ordinal::is_finite() const
{
    for (int i = 0; i < form.get_length(); i++)
    {
        if (form.get(i).get_exponent() != 0)
        {
            return false;
        }
    }

    return true;
}

const ordinal::cantor_form &ordinal::get_form() const
{
    return form;
}

const term &ordinal::operator[](int index) const
{
    return form[index];
}

const term &ordinal::get_leading_term() const
{
    return form.get(0);
}

const term &ordinal::get_term(int index) const
{
    return form.get(index);
}

int ordinal::find_index(const term &t)
{
    int index = 0;
    while (form.get(index) > t)
    {
        index++;
    }

    return index;
}

int ordinal::get_numerical_part() const
{
    return numerical_part;
}

ordinal::cantor_form &ordinal::get_form()
{
    return form;
}

term &ordinal::operator[](int index)
{
    return form[index];
}

term &ordinal::get_leading_term()
{
    return form.get(0);
}

term &ordinal::get_term(int index)
{
    return form.get(index);
}

ordinal& ordinal::set_num(int num)
{
    numerical_part = num;
    return *this;
}