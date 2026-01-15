#pragma once

class term
{
private:
    int coefficient;
    int exponent;
public:
    term();
    term(int coefficient, int exponent);
    term(const term& other);
    ~term() = default;

    term operator++(int);

    term& operator=(const term& other);
    term& operator+=(const term& other);
    term& operator+=(int num);
    term& set_coefficient(int coefficient);
    term& set_exponent(int exponent);

    bool operator==(const term& other) const;
    bool operator!=(const term& other) const;
    bool operator<(const term& other);
    bool operator>(const term& other);
    bool operator<=(const term& other);
    bool operator>=(const term& other);

    int& get_coefficient();
    int& get_exponent();
    const int& get_coefficient() const; 
    const int& get_exponent() const;
};