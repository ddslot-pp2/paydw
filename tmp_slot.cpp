

/*
#include "slot.hpp"
#include <iostream>

template <typename Result>
slot<Result>::slot() {
  std::cout << "slot ctor called" << std::endl;
}

template <typename Result>
slot<Result>::~slot() {
  std::cout << "slot dtor called" << std::endl;
}

template <typename Result>
ddslot_classic<Result>::ddslot_classic(slot_type type) : slot<Result>() {
  slot<Result>::id_ = 10;
  std::cout << "classic slot ctor called" << std::endl;
}

template <typename Result>
ddslot_classic<Result>::~ddslot_classic() {
  std::cout << "classic slot dtor called" << std::endl;
}

template <typename Result>
Result ddslot_classic<Result>::spin() {
  Result rtn;
  return rtn;
}

template <typename Result>
ddslot_triple<Result>::ddslot_triple(slot_type type) : slot<Result>() {
  slot<Result>::id_ = 10;
}

template <typename Result>
ddslot_triple<Result>::~ddslot_triple() {

}

template <typename Result>
Result ddslot_triple<Result>::spin() {
  Result rtn;
  return rtn;
}
*/
