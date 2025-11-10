#include "lazy_sequence.hpp"

template <typename T>
lazy_sequence<T>::default_generator::default_generator(lazy_sequence<T>* seq, int arity, std::function<T,(T)> ungenerator)
{

}