#include "slot_logic_md.hpp"
#include <cassert>
// ```````````````````````````````````````````
// 랜던 번호 1개식 생성 생성
// ...........................................
unsigned random_generator::get_random_number(int start, int end) {
  std::random_device rd;  
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dist(start, end);
  return dist(gen);
}

// ```````````````````````````````````````````
// 페이 정보
// ...........................................
pay_info::pay_info() {

};

pay_info::~pay_info() {

};

void pay_info::add_payline(int number, payline_container payline) {
  paylines_[number] = payline;
}

pay_info::payline_result pay_info::check_payline(int number, const std::vector<int>& out_symbols) {
  
  std::vector<int> symbols;

  for(unsigned i=0; i<out_symbols.size(); i++) {
    const auto& payline = paylines_[number];
    for(auto pos : payline) {
      symbols.push_back(out_symbols[pos]);
    }
  }

  // 와일드 심벌로 시작하거나 다른 심벌로 시작했는데 와일드 같이 있는경우 고려
  auto seq_count = 1;
  auto symbol = symbols[0];
  
  if(symbol != dd_slot::symbols::Scatter) {
    for(unsigned i=1; i<symbols.size(); i++) {
      if(symbols[i] != symbol) {
	if(symbols[i] != dd_slot::symbols::Wild) {
	  break;
	}
      }
      seq_count++;
    }
  }

    
  auto scatter_count = 0;
  for(unsigned i=0; i<symbols.size(); i++) {
    if(symbols[i] == dd_slot::symbols::Scatter) {
      scatter_count++;
    }
  }
  
  
  return payline_result(symbol, seq_count, scatter_count);
}

std::vector<pay_info::payline_result> pay_info::get_payline_result(const std::vector<int>& out_symbols) {
  std::vector<payline_result> payline_results;

  for(unsigned i=0; i<paylines_.size(); i++) {
    payline_results.push_back(check_payline(i, out_symbols));
  }
  return payline_results;
}

// ```````````````````````````````````````````
// paytable
// ...........................................
void paytable_info::add_paytable_symbol(int symbol, int seq, int credit) {

  if(paytable_.find(symbol) != paytable_.end()) {
    auto& v = paytable_[symbol];
    v[seq] = credit;
  } else {
    paytable_[symbol].insert(std::make_pair(seq, credit));
  }

}

int paytable_info::get_symbol_credit(int symbol, int seq) {
  auto credit = 0;
  auto& v = paytable_[symbol];
  // v = map<int int> ; seq, credit

  if(v.find(seq) != v.end()) {
    credit = v[seq];
    std::cout << "페이라인 맞음" << std::endl;
    std::cout << "seq: " << seq << std::endl;   
    std::cout << "credit: " << credit << std::endl;
    std::cout << "symbol: " << symbol << std::endl;
    std::cout << "----------------" << std::endl;
  } else {
    std::cout << "페이라인 안맞음" << std::endl;
  }


  return credit;
}

// ```````````````````````````````````````````
// 슬롯
// ...........................................
slot::slot(int max_row, int max_col) : max_row_(max_row), max_col_(max_col), is_spin(false) {
  max_reel_ = max_col_;
}

slot::~slot() {

}

void slot::set_reel(int reel_count, reel_symbols reel) {

#ifdef NDEBUG

#else 
  assert(!(max_reel_-1 < reel_count));
#endif

  reels_[reel_count] = reel;
}

symbol_result slot::get_symbols_in_reel(int reel_count) {
  std::vector<int> out_symbols; out_symbols.clear();
  const std::vector<int>& reel = reels_[reel_count];
  
  symbol_result sr;

  auto rv = random_generator_.get_random_number(0, reel.size()-1);
  //std::cout << "rv: " << rv << std::endl;

  if(rv == 0) {
    out_symbols.push_back(reel[reel.size()-1]);
    out_symbols.push_back(reel[rv]);
    out_symbols.push_back(reel[rv+1]);
  } else if(rv == reel.size()-1) {
    out_symbols.push_back(reel[rv-1]);
    out_symbols.push_back(reel[rv]);
    out_symbols.push_back(reel[0]);
  } else {
    out_symbols.push_back(reel[rv-1]);
    out_symbols.push_back(reel[rv]);
    out_symbols.push_back(reel[rv+1]);
  }

  sr.reel_id = rv;
  sr.symbols = out_symbols;
  return sr;
}

// ```````````````````````````````````````````
// dd 슬롯
// ...........................................
dd_slot::dd_slot(int max_row, int max_col) : slot(max_row, max_col) {
  std::cout << "dd_slot ctor" << std::endl;
}

dd_slot::~dd_slot() {

}

bool dd_slot::init() {

  // 슬롯에 맞게 릴지정
  set_reel(0, { Scatter, Cherry, Seven, Bar3, Watermelon, Bar2, Mangosteen, Cherry, Bar3, Bar, Bell, Watermelon, Bar2, Cherry, Seven3, Bell, Bar, Mangosteen, Bar2, Wild, Watermelon, Cherry, Seven, Mangosteen, Bar2, Scatter, Bar, Watermelon, Bar3, Bell, Seven3, Bar });

  set_reel(1, { Cherry,Bell, Seven3, Bar3, Mangosteen, Cherry, Seven, Bar, Watermelon, Seven3, Bar3, Cherry, Seven, Wild, Bar, Bar2, Mangosteen, Watermelon, Bar3, Bell, Scatter, Cherry, Bar, Mangosteen, Watermelon, Bar2, Bell, Cherry, Seven3, Seven, Mangosteen, Watermelon });

  set_reel(2, { Seven, Bar, Bar2, Scatter, Mangosteen, Cherry, Bar, Seven, Bar3, Watermelon, Seven3, Bell, Bar, Cherry, Mangosteen, Bar3, Bar2, Watermelon, Cherry, Bell, Seven, Bar3, Watermelon, Mangosteen, Bar2, Wild, Bell, Watermelon,Bar, Mangosteen, Cherry, Seven3 });

  set_reel(3, { Seven, Mangosteen, Wild, Scatter, Bar, Watermelon, Mangosteen, Bar3, Seven3 ,Bar2, Watermelon, Cherry, Bar3, Mangosteen, Bar, Seven, Cherry, Watermelon ,Bell, Bar3, Mangosteen, Bar2, Bar, Bell ,Watermelon, Cherry, Bar2, Seven ,Bar3, Seven3, Cherry, Watermelon });

  set_reel(4, { Watermelon, Scatter, Seven3, Seven, Mangosteen, Bar3, Bar, Watermelon, Bell, Mangosteen ,Bar3, Seven, Bar, Seven3, Cherry, Bar2, Bar3, Seven, Wild, Bar, Bell, Cherry, Bar2, Seven, Watermelon, Bar, Seven3, Cherry, Bar3, Bar2, Mangosteen, Bell });

  // 페이라인 지정
  pay_info_.add_payline(0,  {  1,  4,  7, 10, 13 }); //  1
  pay_info_.add_payline(1,  {  0,  3,  6,  9, 12 }); //  2
  pay_info_.add_payline(2,  {  2,  5,  8, 11, 14 }); //  3
  pay_info_.add_payline(3,  {  0,  4,  8, 10, 12 }); //  4
  pay_info_.add_payline(4,  {  2,  4,  6, 10, 14 }); //  5

  pay_info_.add_payline(5,  {  1,  3,  6,  9, 13 }); //  6
  pay_info_.add_payline(6,  {  1,  5,  8, 11, 13 }); //  7
  pay_info_.add_payline(7,  {  0,  3,  7, 11, 14 }); //  8
  pay_info_.add_payline(8,  {  2,  5,  7,  9, 12 }); //  9
  pay_info_.add_payline(9,  {  1,  3,  7, 11, 14 }); // 10

  pay_info_.add_payline(10, {  1,  5,  7,  9, 13 }); // 11
  pay_info_.add_payline(11, {  0,  4,  7, 10, 12 }); // 12
  pay_info_.add_payline(12, {  2,  4,  7, 10, 14 }); // 13
  pay_info_.add_payline(13, {  0,  4,  6, 10, 12 }); // 14
  pay_info_.add_payline(14, {  2,  4,  8, 10, 14 }); // 15

  pay_info_.add_payline(15, {  1,  4,  6, 10, 13 }); // 16
  pay_info_.add_payline(16, {  1,  4,  8, 10, 13 }); // 17
  pay_info_.add_payline(17, {  0,  3,  8,  9, 12 }); // 18
  pay_info_.add_payline(18, {  2,  5,  6, 11, 14 }); // 19
  pay_info_.add_payline(19, {  0,  5,  8, 11, 12 }); // 20
  
  // 페이테이블 생성
  paytable_info_.add_paytable_symbol(Wild, 5, 5000);
  paytable_info_.add_paytable_symbol(Wild, 4,  500);
  paytable_info_.add_paytable_symbol(Wild, 3,   50);
  paytable_info_.add_paytable_symbol(Wild, 2,    5);
  /*
  paytable_info_.add_paytable_symbol(Scatter, 5, 100);
  paytable_info_.add_paytable_symbol(Scatter, 4,  25);
  paytable_info_.add_paytable_symbol(Scatter, 3,   5);
*/
  paytable_info_.add_paytable_symbol(Seven3, 5, 1000);
  paytable_info_.add_paytable_symbol(Seven3, 4,  200);
  paytable_info_.add_paytable_symbol(Seven3, 3,   40);
  paytable_info_.add_paytable_symbol(Seven3, 2,    3);

  paytable_info_.add_paytable_symbol(Seven, 5, 500);
  paytable_info_.add_paytable_symbol(Seven, 4, 150);
  paytable_info_.add_paytable_symbol(Seven, 3,  30);
  paytable_info_.add_paytable_symbol(Seven, 2,   2);
  
  paytable_info_.add_paytable_symbol(Bell, 5, 300);
  paytable_info_.add_paytable_symbol(Bell, 4, 100);
  paytable_info_.add_paytable_symbol(Bell, 3,  25);
  paytable_info_.add_paytable_symbol(Bell, 2,   2);

  paytable_info_.add_paytable_symbol(Bar3, 5, 200);
  paytable_info_.add_paytable_symbol(Bar3, 4,  75);
  paytable_info_.add_paytable_symbol(Bar3, 3,  20);

  paytable_info_.add_paytable_symbol(Bar2, 5, 200);
  paytable_info_.add_paytable_symbol(Bar2, 4,  75);
  paytable_info_.add_paytable_symbol(Bar2, 3,  20);

  paytable_info_.add_paytable_symbol(Bar, 5, 100);
  paytable_info_.add_paytable_symbol(Bar, 4,  50);
  paytable_info_.add_paytable_symbol(Bar, 3,  15);

  paytable_info_.add_paytable_symbol(Mangosteen, 5, 100);
  paytable_info_.add_paytable_symbol(Mangosteen, 4,  50);
  paytable_info_.add_paytable_symbol(Mangosteen, 3,  15);

  paytable_info_.add_paytable_symbol(Watermelon, 5, 50);
  paytable_info_.add_paytable_symbol(Watermelon, 4, 25);
  paytable_info_.add_paytable_symbol(Watermelon, 3, 10);

  paytable_info_.add_paytable_symbol(Cherry, 5, 50);
  paytable_info_.add_paytable_symbol(Cherry, 4, 25);
  paytable_info_.add_paytable_symbol(Cherry, 3, 10);

  return true;
}

spin_result dd_slot::spin() {

  spin_result sr;

  if(is_spin) { 
    sr.result = false;
    return sr;
  }

  is_spin = true;

  out_symbols_.clear();

  for(auto i=0; i<max_reel_; i++) {
    auto symbol_result = get_symbols_in_reel(i);
    sr.reel_result.push_back(symbol_result.reel_id + 1);
    out_symbols_.insert(out_symbols_.end(), symbol_result.symbols.begin(), symbol_result.symbols.end());
  }

  auto results = pay_info_.get_payline_result(out_symbols_);
  auto payline = 1;
  auto win_credit = 0;

  for(auto& result : results) {
    std::cout << "payline: " << payline << std::endl;
    auto credit = paytable_info_.get_symbol_credit(result.symbol, result.seq_count);
    if(credit > 0) {
      payline_info tmp;
      //sr.payline_result.push_back(std::make_tuple(payline, credit));
      tmp.number = payline;
      tmp.seq_count = result.seq_count;
      tmp.credit = credit;
      sr.payline_result.push_back(tmp);
    }

    win_credit += credit;

    if(result.scatter_count >= 3) {
      if(result.seq_count == 3) {
	std::cout << "scatter 3" << std::endl;
	win_credit += 5;
	sr.freespin_count = 10;
      } else if(result.seq_count == 4) {
	std::cout << "scatter 4" << std::endl;
	win_credit += 25;
	sr.freespin_count = 15;
      } else if(result.seq_count == 5) {
	std::cout << "scatter 5" << std::endl;
	win_credit += 100;
	sr.freespin_count = 20;
      }
    }
    payline++;
  }

  sr.win_credit = win_credit;

  is_spin = false;

  std::cout << "is_spin: " << is_spin << std::endl;

  return sr;
}

// ```````````````````````````````````````````
// 슬롯 로직의 총괄관리
// ...........................................
slot_logic_md::slot_logic_md() {

}

slot_logic_md::~slot_logic_md() {

}

bool slot_logic_md::init() {

  for(auto i=0; i<50; i++) {
    dd_slots_[i] = std::make_shared<dd_slot>(3, 5);
    dd_slots_[i]->init();
  }


  return true;
}
