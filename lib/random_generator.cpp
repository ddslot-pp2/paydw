#include "random_generator.hpp"

/*
template <typename T>
T random_generator::generate_random<T>(T start, T end) {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dist(start, end);
  return dist(gen);
}  
*/

template <>
double random_generator::generate_random<double>(double start, double end) {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<> dist(start, end);
  return dist(gen);
}
