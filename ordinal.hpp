#pragma once
#include <stdexcept>
#include "cardinality.hpp"


class ordinal
{
private:
    int omega;
    int number;
    bool is_omega_square = false;

public:
    ordinal();
    ordinal(int omega, int number);
    ordinal(const ordinal& other);
    ordinal(const cardinal& length);
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

    int get_omega();
    const int get_omega() const;
    int get_number();
    const int get_number() const;

    void set_omega(int omega);
    void increase_omega();
    void set_number(int number);
    void increase_number();
};
