#include <atomic>
#include <iostream>
class spin_mutex
{
public:
  void lock()
  {
    std::cout << "spin lock start" << std::endl;
    while(lck.test_and_set(std::memory_order_acquire))
      {}
  }
 
  void unlock()
  {
    lck.clear(std::memory_order_release);
    std::cout << "spin lock end" << std::endl;
  }
 
private:
  std::atomic_flag lck = ATOMIC_FLAG_INIT;
};
