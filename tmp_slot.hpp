
/*
#ifndef SLOT_HPP
#define SLOT_HPP

#include <memory>
#include "lib/random_generator.hpp"

struct spin_result {
  spin_result() { result = 10; }
  int result;
};

struct spin_result2 {
  spin_result2() { result = 20; }
  int result;
};

enum slot_type { none, classic, triple };

template <typename Result>
class slot {
protected:

  int id_;
  slot_type type_;
  random_generator random_generator_;

public:
  slot();
  virtual ~slot();

  virtual Result spin() = 0;
};

template <typename Result>
class ddslot_classic : public slot<Result> {

public:
  ddslot_classic(slot_type type);
  virtual ~ddslot_classic();

  Result spin();
};

template <typename Result>
class ddslot_triple : public slot<Result> {
public:
  ddslot_triple(slot_type type);
  virtual ~ddslot_triple();

  Result spin();
};

#include "slot.cpp"

#endif

*/
