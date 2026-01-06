#pragma once
#include <stdexcept>
#include "cardinal.hpp"
#include "term.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"


class ordinal
{
public:
    using cantor_form = array_sequence<term>;

private:
    cantor_form form;
    int numerical_part = 0;

public:
    ordinal();
    ordinal(int number);
    ordinal(const cantor_form& other);
    ordinal(const ordinal& other);
    ~ordinal() = default;

    ordinal operator+(const ordinal& other);
    ordinal operator+(int num);
    ordinal operator-(const ordinal& other);
    ordinal operator++(int);
    ordinal make_from(int num);
    ordinal& operator=(int num);
    ordinal& operator+=(int num);
    ordinal& operator=(const ordinal& other);
    ordinal& operator+=(const ordinal& other);
    ordinal& operator-=(const ordinal& other);
    ordinal& operator++();

    bool operator==(const ordinal& other);
    bool operator==(int num);
    bool operator!=(const ordinal& other);
    bool operator!=(int num);
    bool operator<(const ordinal& other);
    bool operator<(int num);
    bool operator>(const ordinal& other);
    bool operator>(int num);
    bool operator<=(const ordinal& other);
    bool operator<=(int num);
    bool operator>=(const ordinal& other);
    bool operator>=(int num);
    bool is_finite();

    const cantor_form& get_form() const;

    int get_omega();
    const int get_omega() const;
    int get_number();
    const int get_number() const;

    void set_omega(int omega);
    void increase_omega();
    void set_number(int number);
    void increase_number();

private:
    cantor_form& get_form();
};
