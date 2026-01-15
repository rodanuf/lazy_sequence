#pragma once
#include <stdexcept>
#include <initializer_list>
#include "cardinal.hpp"
#include "term.hpp"
#include "lab3_2ndsem/headers/array_sequence.hpp"


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
    ordinal(const std::initializer_list<term>& list);
    ordinal(const cantor_form& other);
    ordinal(const term& other);
    ordinal(const ordinal& other);
    ~ordinal() = default;

    ordinal operator+(const ordinal& other);
    ordinal operator+(int num);
    ordinal operator-(const ordinal& other);
    ordinal operator-(int num);
    ordinal operator*(const ordinal& other);
    ordinal operator*(int num);
    ordinal operator++(int);
    ordinal& operator=(int num);
    ordinal& operator+=(int num);
    ordinal& operator=(const ordinal& other);
    ordinal& operator+=(const ordinal& other);
    ordinal& operator-=(const ordinal& other);
    ordinal& operator++();

    ordinal& add_term(const term& t);
    ordinal& normalize();

    bool operator==(const ordinal& other) const;
    bool operator==(int num) const;
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
    bool is_finite() const;

    const cantor_form& get_form() const;

    const term& operator[](int index) const;
    const term& get_leading_term() const;
    const term& get_term(int index) const;

    term& operator[](int index);

    int find_index(const term& t);
    int get_numerical_part() const;

private:
    cantor_form& get_form();

    term& get_leading_term();
    term& get_term(int index);

    ordinal& set_num(int num);

};
