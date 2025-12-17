#pragma once
#include <stdexcept>
#include "cardinality.hpp"


class ordinary
{
private:
    int omega;
    int number;
    bool is_omega_square = false;

public:
    ordinary();
    ordinary(int omega, int number);
    ordinary(const ordinary& other);
    ordinary(const cardinality& length);
    ~ordinary() = default;

    ordinary operator+(const ordinary& other);
    ordinary operator+(int num);
    ordinary operator-(const ordinary& other);
    ordinary operator++(int);
    ordinary make_from(int num);
    ordinary& operator=(int num);
    ordinary& operator+=(int num);
    ordinary& operator=(const ordinary& other);
    ordinary& operator+=(const ordinary& other);
    ordinary& operator-=(const ordinary& other);
    ordinary& operator++();

    bool operator==(const ordinary& other);
    bool operator==(int num);
    bool operator!=(const ordinary& other);
    bool operator!=(int num);
    bool operator<(const ordinary& other);
    bool operator<(int num);
    bool operator>(const ordinary& other);
    bool operator>(int num);
    bool operator<=(const ordinary& other);
    bool operator<=(int num);
    bool operator>=(const ordinary& other);
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
