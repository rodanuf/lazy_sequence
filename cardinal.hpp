#pragma once
#include <stdexcept>

class cardinal
{
private:
    int aleph_idx;
    int finite_length;
    
public:
    cardinal(int aleph_idx = 0, int finite_length = 0);
    cardinal(const cardinal& other) = default;
    ~cardinal() = default;

    cardinal operator+(const cardinal& other);
    cardinal operator-(int num);

    cardinal& operator+=(const cardinal& other);

    int get_aleph_idx();
    const int get_aleph_idx() const;
    int get_finite_length();
    const int get_finite_length() const;
};