#include "slot_md.hpp"

bool slot_md::init() {

  for(auto i=0; i<10; i++) {
    auto slot_ptr = std::make_shared<ddslot_classic>(i);
    slot_ptr->init();
    slots_[i] = slot_ptr;
  }

  return true;
}

slot_ptr slot_md::get_slot(int id) {
  if(id > slots_.size()) return nullptr;
  return slots_[id];
}

int slot_md::get_slot_id(slot_type type) {
  std::lock_guard<std::mutex> guard(m);

  for(auto& slot : slots_) {
    if(slot.second->get_type() == type && slot.second->is_seat()) {
      std::cout << "slot id: " << slot.second->get_id() << std::endl;
      slot.second->update_timestamp();
      return slot.second->get_id();
    }
  }

  return -1;
}
