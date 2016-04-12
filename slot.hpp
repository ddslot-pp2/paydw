#ifndef SLOT_HPP
#define SLOT_HPP
#include <string>
#include <iostream>
#include <stdint.h> 
#include <chrono>
#include <map>
#include <memory>
#include "lib/random_generator.hpp"
using reel_symbols = std::vector<int>;

enum slot_type { none, classic };

class slot {
public:
  virtual std::string spin(int bet) = 0;
  virtual bool reset() = 0 ;

  int get_id() { return id_; }
  void update_timestamp() { timestamp_ = get_now(); }
  size_t get_timestamp() { return timestamp_; }
  size_t get_now();
  slot_type get_type() { return type_; }


  virtual bool init() = 0;
  virtual bool is_seat() = 0;
  void set_reel(int reel_count, reel_symbols reel);

protected:
  slot();
  virtual ~slot();

  size_t timestamp_;
  int id_;
  int64_t uid_;
  slot_type type_;
  random_generator random_generator_;
  std::map<int, reel_symbols> reels_;
  //reel_symbols out_reels_;
};

class paytable_classic {
public:
  paytable_classic();
  ~paytable_classic();
  bool init();

  bool add_paytable(int symbol, int sequence, int credit);
  int get_credit(int symbol, int sequence);

private:
  std::map<int, std::map<int, int>> paytable_;
};

class payline_classic {
public:

  struct payline_result {
    int number; // hit number
    int sequence; 
    int symbol;
    int win_credit;
    payline_result() {
      number = -1; sequence = 1; symbol = -1; win_credit = 0;
    }
    ~payline_result() {}
  };

  payline_classic();
  ~payline_classic();

  bool init();
  bool add_payline(int, std::vector<int> payline);
  
  payline_result check_payline(int number, const std::vector<int>& out_symbols, bool is_freespin_on = false, int wild_symbol=-1);
  //std::unique_ptr<paytable_classic> paytable_;

  unsigned get_payline_count() { return paylines_.size(); }

private:
  using payline = std::vector<int>;
  std::map<int, payline> paylines_;
};

class ddslot_classic : public slot {
public:

  enum symbols { Wild=0, Seven3, Seven, Bell, Bar3, Bar2, Bar, Mangosteen, Watermelon, Cherry, Scatter, Any };

  ddslot_classic(int id);
  virtual ~ddslot_classic();

  bool init();
  std::string spin(int bet);
  bool reset();
  bool is_seat();

  std::vector<int> set_out_reels(std::vector<int>& stop_points);


  int current_freespin_count_;
  int max_freespin_count_;
  payline_classic payline_;
  paytable_classic paytable_;
};

#endif

// 심볼 번호
// 0   3   6    9   12
// 1   4   7   10   13
// 2   5   8   11   14
