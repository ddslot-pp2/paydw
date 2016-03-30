#ifndef RANDOM_GENERATOR_HPP
#define RANDOM_GENERATOR_HPP
#include <random>
#include <vector>

class random_generator {

public:
  template <typename T>
  T generate_random(T start, T end) {
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dist(start, end);
    return dist(gen);
  }

  double generate_random(double start, double end);

  template <typename T, std::size_t Size=5> 
  std::vector<T> get_random_number(T start, T end);

};

template <typename T, std::size_t Size> 
std::vector<T> random_generator::get_random_number(T start, T end) {
  std::vector<T> v;

  for(unsigned i=0; i<Size; i++) {
    v.push_back(generate_random<T>(start, end));
  }

  return v;
}


//#include "random_generator.cpp"
/*
class tmp {
public:
  template <typename T> 
  T sum(T lhs, T rhs) {
    std::cout << "XXXXXXXXXX" <<std::endl;
    return lhs+rhs;
  }
  
  double sum(double lhs, double rhs);

};


template <>
double tmp::sum<double>(double lhs, double rhs) {
  std::cout << "double double" <<std::endl;
  return lhs+rhs;
}
*/

#endif
