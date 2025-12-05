#pragma once

class cardinality
{
private:
    int alef_idx;
    int finite_length;
    
public:
    cardinality();
    cardinality(int alef_idx, int finite_length = 0);
    ~cardinality() = default;


};