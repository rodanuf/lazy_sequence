#pragma once
#include "ordinal.hpp"


class counter // переименовать, так как это счётчик, а не итератор, счётчик вынести
{
private:
    ordinal cur_idx;

public:
    counter();
    counter(int index);
    counter(const ordinal &other);
    counter(const counter &other);
    ~counter() = default;

    counter operator++(int);
    counter &operator++();

    counter &increment_idx();
    counter &set_idx(int omega, int num);
    counter &set_idx(const ordinal &idx);
    counter &set_idx(int num);

    ordinal &get_index(); // не нужно
    const ordinal &get_index() const;

    bool has_omega();

    int get_num_part();
    int get_omega_part();

    void skip(int start, int end);
    void skip(ordinal start, ordinal end);
};