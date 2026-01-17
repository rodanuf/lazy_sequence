#pragma once
#include "ordinal.hpp"
#include "pointers/uniq_ptr.hpp"

class counter
{
private:
    uniq_ptr<ordinal> cur_idx;

public:
    counter();
    counter(int index);
    counter(const ordinal &other);
    ~counter() = default;

    counter &operator++();

    counter &increment_idx();
    counter &set_idx(int omega, int num);
    counter &set_idx(const ordinal &idx);
    counter &set_idx(int num);

    const ordinal &get_index() const;

    bool has_omega();

    int get_num_part();

};