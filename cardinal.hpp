#pragma once
#include <stdexcept>

class cardinal
{
private:
    int aleph_idx;
    int finite_length;
    
public:
    cardinal();
    cardinal(int aleph_idx, int finite_length = 0);
    ~cardinal() = default;

    cardinal operator+(const cardinal& other);

    cardinal& operator+=(const cardinal& other);

    int get_aleph_idx();
    const int get_aleph_idx() const;
    int get_finite_length();
    const int get_finite_length() const;
};