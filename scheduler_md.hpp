#ifndef SCHEDULER_MD_HPP
#define SCHEDULER_MD_HPP
#include <atomic>
#include <time.h>
#include <vector>
#include <map>
#include <memory>
#include <tuple>
#include "lib/ddlock.hpp"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class tournament {
public:
  tournament();
  tournament(int start_bet, int end_bet);
  ~tournament() {}

  void add_bet_credit(int bet_credit);
  int get_bet_credit() { return bet_credits_; }

  std::tuple<std::string, std::string> get_user_info(int uid);
  void set_user_info(int uid, std::tuple<std::string, std::string> user_info);

  int get_prize_pool();
  void reset();

private:
  int start_bet_;
  int end_bet_;
  std::atomic<int> bet_credits_;
  std::map<int, std::string> names_;
  std::map<int, std::tuple<std::string, std::string>> user_infos_;
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

  boost::asio::io_service io_service_;
};

#endif
