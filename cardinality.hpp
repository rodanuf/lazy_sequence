#pragma once

class cardinality
{
private:
    int aleph_idx;
    int finite_length;
    
public:
    cardinality();
    cardinality(int aleph_idx, int finite_length = 0);
    ~cardinality() = default;

    cardinality operator+(const cardinality& other);

    int get_aleph_idx();
    const int get_aleph_idx() const;
    int get_finite_length();
    const int get_finite_length() const;
};