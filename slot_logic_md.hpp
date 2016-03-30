#include <vector>
#include <random>
#include <iostream>
#include <map>
#include <memory>
#include <atomic>

// ```````````````````````````````````````````
// 랜던 번호 생성
// ...........................................
class random_generator {

public:
  unsigned get_random_number(int start, int end);
};

// ```````````````````````````````````````````
// payinfo
// ...........................................
class pay_info {
  using payline_container = std::vector<int>;  
  std::map<int, payline_container> paylines_;

  struct payline_result {
    int symbol;
    int seq_count;
    int scatter_count;
    payline_result(int symbol, int seq_count, int scatter_count) : symbol(symbol), seq_count(seq_count), scatter_count(scatter_count) {}
    ~payline_result() {}
  };

  payline_result check_payline(int number, const std::vector<int>& out_symbols);

public:

  void add_payline(int number, payline_container payline);
  
  std::vector<payline_result> get_payline_result(const std::vector<int>& out_symbols);

  pay_info();
  ~pay_info();

};

// ```````````````````````````````````````````
// paytable
// ...........................................
class paytable_info {
  // symbol,       seq: 5, credit
  std::map<int, std::map<int, int>> paytable_;

public:
  void add_paytable_symbol(int symbol, int seq, int credit);
  int get_symbol_credit(int symbol, int seq);
};

// ```````````````````````````````````````````
// spin_result 스핀 결과값
// ...........................................
struct payline_info {
  int number;
  int credit;
  int seq_count;
};

struct spin_result { 
  // payline number, credit
  //std::vector<std::tuple<int, int>> payline_result;
  std::vector<payline_info> payline_result;
  // freespin 결과값, freespin count
  int freespin_count;

  int win_credit;

  std::vector<int> reel_result;
  bool result;

  spin_result() {
    freespin_count = 0;
    win_credit = 0;
    result = true;
  }
  ~spin_result() {}
};

struct symbol_result {
  int reel_id;
  std::vector<int> symbols;
};

// ```````````````````````````````````````````
// slot
// ...........................................
using reel_symbols = std::vector<int>;

class slot {

protected:
  int max_row_;
  int max_col_;
  int max_reel_;

  std::map<int, reel_symbols> reels_;
  random_generator random_generator_;

  symbol_result get_symbols_in_reel(int reel_count);

  std::vector<int> out_symbols_;
  std::atomic<bool> is_spin;

public:
  slot(int max_row, int max_col);
  virtual ~slot();

  virtual spin_result spin() = 0;

  void set_reel(int reel_count, reel_symbols reel);
};

// ```````````````````````````````````````````
// dd_slot은 slot을 상속받아서 다른부분들으 채워넣음 심볼 이라든지 페이라인 같은거
// ...........................................
class dd_slot : public slot {

  pay_info pay_info_;
  paytable_info paytable_info_;

public:
  dd_slot(int max_row, int max_col);
  virtual ~dd_slot();

  bool init();
  spin_result spin();

  enum symbols { Wild=0, Seven3, Seven, Bell, Bar3, Bar2, Bar, Mangosteen, Watermelon, Cherry, Scatter };
};


// ```````````````````````````````````````````
// 슬롯 로직의 총괄관리
// ...........................................
class slot_logic_md {
  //std::map<int, std::shared_ptr<dd_slot>> dd_slots_;

public:
  slot_logic_md();
  ~slot_logic_md();

  bool init();

  static slot_logic_md& get() {
    static slot_logic_md obj;
    return obj;
  }

  std::map<int, std::shared_ptr<dd_slot>> dd_slots_;
};

/*
Atkins			->	Wild
-------------------------------------------------
Steak			->	Seven3;    3 Sevens(777)
-------------------------------------------------
Ham			-> 	Seven;     Seven(7)
-------------------------------------------------
Buffalo Wings		->	Bell
-------------------------------------------------
Sausage			->	Bar3
-------------------------------------------------
Eggs			->	Bar2
-------------------------------------------------
Butter			->	Bar
-------------------------------------------------
Cheese			->	Mangosteen
-------------------------------------------------
Bacon			->	Watermelon	
-------------------------------------------------
Mayonnaise		->	Cherry
-------------------------------------------------
Scale			->	Scatter
-------------------------------------------------
*/
