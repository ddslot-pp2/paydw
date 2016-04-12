#ifndef SCHEDULER_MD_HPP
#define SCHEDULER_MD_HPP
#include <atomic>
#include <time.h>
#include <vector>
#include <map>
#include <memory>
#include "lib/ddlock.hpp"

class tournament {
public:
  tournament();
  ~tournament() {}

  void add_win_credit(int credit);
  int get_win_credit() { return win_credit_; }

  std::string get_name(int uid);
  void set_name(int uid, std::string name);

private:
  std::atomic<int> win_credit_;
  std::map<int, std::string> names_;
  spin_mutex m;
};

enum tournament_type { start, end };

using tournament_ptr = std::shared_ptr<tournament>;
using tournament_weak_ptr = std::weak_ptr<tournament>;

class scheduler_md {
public:
  scheduler_md();
  ~scheduler_md();

  static scheduler_md& get() {
    static scheduler_md obj;
    return obj;
  }

  void init();
  void run();

  int get_timestamp();

  tournament_ptr get_tournament(int zone);
  void reset_tournament();

  bool get_tournament_on() { return is_tournament_on_; }
  
  std::atomic<bool> is_run_;
  std::atomic<bool> is_tournament_on_;
  int remaining_time_;
  tournament_type type_;
  int tournament_count_;

  static const int max_tournament_zone = 3;
  std::vector<tournament_ptr> tournaments_;
};

#endif
