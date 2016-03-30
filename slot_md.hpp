#ifndef SLOT_MD_HPP
#define SLOT_MD_HPP
#include <memory>
#include <map>
#include <mutex>
#include "slot.hpp"

using slot_ptr = std::shared_ptr<slot>;

class slot_md {
public:

  static slot_md& get() {
    static slot_md obj;
    return obj;
  }

  bool init();

  slot_ptr get_slot(int id);
  int get_slot_id(slot_type type);

private:
  
  std::map<int, slot_ptr> slots_;
  std::mutex m;
};

#endif
