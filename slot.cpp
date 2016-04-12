#include "slot.hpp"
#include <iostream>
#include <set>
#include "json11.hpp"

slot::slot() {
  std::cout << "ddslot ctor called" << std::endl;
  timestamp_ = 0;
  //update_timestamp();
}

slot::~slot() {
  std::cout << "ddslot dtor called" << std::endl;
}

size_t slot::get_now() {
  using namespace std::chrono;
  typedef std::chrono::steady_clock clk;
  unsigned long long milliseconds_since_epoch=
    std::chrono::duration_cast<std::chrono::milliseconds>(clk::now().time_since_epoch()).count();

  return milliseconds_since_epoch;
}

void slot::set_reel(int reel_count, reel_symbols reel) {
  reels_[reel_count] = reel;
}

// 페이테이블
paytable_classic::paytable_classic() {

}

paytable_classic::~paytable_classic() {

}

bool paytable_classic::init() {
  auto r = true;
  r &= add_paytable(ddslot_classic::symbols::Wild, 5, 5000);
  r &= add_paytable(ddslot_classic::symbols::Wild, 4,  500);
  r &= add_paytable(ddslot_classic::symbols::Wild, 3,   50);
  r &= add_paytable(ddslot_classic::symbols::Wild, 2,    5);

  r &= add_paytable(ddslot_classic::symbols::Seven3, 5, 1000);
  r &= add_paytable(ddslot_classic::symbols::Seven3, 4,  200);
  r &= add_paytable(ddslot_classic::symbols::Seven3, 3,   40);
  r &= add_paytable(ddslot_classic::symbols::Seven3, 2,    3);

  r &= add_paytable(ddslot_classic::symbols::Seven, 5,  500);
  r &= add_paytable(ddslot_classic::symbols::Seven, 4,  150);
  r &= add_paytable(ddslot_classic::symbols::Seven, 3,   30);
  r &= add_paytable(ddslot_classic::symbols::Seven, 2,    2);

  r &= add_paytable(ddslot_classic::symbols::Bell, 5,  300);
  r &= add_paytable(ddslot_classic::symbols::Bell, 4,  150);
  r &= add_paytable(ddslot_classic::symbols::Bell, 3,   25);
  r &= add_paytable(ddslot_classic::symbols::Bell, 2,    2);

  r &= add_paytable(ddslot_classic::symbols::Bar3, 5,  200);
  r &= add_paytable(ddslot_classic::symbols::Bar3, 4,   75);
  r &= add_paytable(ddslot_classic::symbols::Bar3, 3,   20);

  r &= add_paytable(ddslot_classic::symbols::Bar2, 5,  200);
  r &= add_paytable(ddslot_classic::symbols::Bar2, 4,   75);
  r &= add_paytable(ddslot_classic::symbols::Bar2, 3,   20);

  r &= add_paytable(ddslot_classic::symbols::Bar, 5,  100);
  r &= add_paytable(ddslot_classic::symbols::Bar, 4,   50);
  r &= add_paytable(ddslot_classic::symbols::Bar, 3,   15);

  r &= add_paytable(ddslot_classic::symbols::Mangosteen, 5,  100);
  r &= add_paytable(ddslot_classic::symbols::Mangosteen, 4,   50);
  r &= add_paytable(ddslot_classic::symbols::Mangosteen, 3,   15);

  r &= add_paytable(ddslot_classic::symbols::Watermelon, 5,   50);
  r &= add_paytable(ddslot_classic::symbols::Watermelon, 4,   25);
  r &= add_paytable(ddslot_classic::symbols::Watermelon, 3,   10);

  r &= add_paytable(ddslot_classic::symbols::Cherry, 5,   50);
  r &= add_paytable(ddslot_classic::symbols::Cherry, 4,   25);
  r &= add_paytable(ddslot_classic::symbols::Cherry, 3,   10);

  r &= add_paytable(ddslot_classic::symbols::Any, 5,   100);
  r &= add_paytable(ddslot_classic::symbols::Any, 4,   25);
  r &= add_paytable(ddslot_classic::symbols::Any, 3,   10);

  return r;
}

bool paytable_classic::add_paytable(int symbol, int sequence, int credit) {


 if(paytable_.find(symbol) != paytable_.end()) {
    auto& v = paytable_[symbol];    
    if(v.find(sequence) != v.end()) return false;
    v[sequence] = credit;
  } else {
    paytable_[symbol].insert(std::make_pair(sequence, credit));
  }

  return true;
}

int paytable_classic::get_credit(int symbol, int sequence) {
  auto& symbol_paytable = paytable_[symbol];
  if(symbol_paytable.find(sequence) == symbol_paytable.end()) return 0;
  return symbol_paytable[sequence];
}

// 페이라인 정보
payline_classic::payline_classic() {

}

payline_classic::~payline_classic() {
  
}

bool payline_classic::init() {

  auto r = true;
  r &= add_payline(0,  {  1,  4,  7, 10, 13 }); //  1
  r &= add_payline(1,  {  0,  3,  6,  9, 12 }); //  2
  r &= add_payline(2,  {  2,  5,  8, 11, 14 }); //  3
  r &= add_payline(3,  {  0,  4,  8, 10, 12 }); //  4
  r &= add_payline(4,  {  2,  4,  6, 10, 14 }); //  5

  r &= add_payline(5,  {  0,  3,  7,  9, 12 }); //  6
  r &= add_payline(6,  {  2,  5,  7, 11, 14 }); //  7
  r &= add_payline(7,  {  1,  5,  8, 11, 13 }); //  8
  r &= add_payline(8,  {  1,  3,  6,  9, 13 }); //  9
  r &= add_payline(9,  {  0,  4,  7, 10, 12 }); // 10

  r &= add_payline(10, {  2,  4,  7, 10, 14 }); // 11
  r &= add_payline(11, {  0,  4,  6, 10, 12 }); // 12
  r &= add_payline(12, {  2,  4,  8, 10, 14 }); // 13
  r &= add_payline(13, {  1,  3,  7,  9, 13 }); // 14
  r &= add_payline(14, {  1,  5,  7, 11, 13 }); // 15

  r &= add_payline(15, {  1,  4,  6, 10, 13 }); // 16
  r &= add_payline(16, {  1,  4,  8, 10, 13 }); // 17
  r &= add_payline(17, {  0,  5,  6, 11, 12 }); // 18
  r &= add_payline(18, {  2,  3,  8,  9, 14 }); // 19
  r &= add_payline(19, {  1,  3,  8,  9, 13 }); // 20

  r &= add_payline(20, {  1,  5,  6, 11, 13 }); // 21
  r &= add_payline(21, {  0,  3,  8,  9, 12 }); // 22
  r &= add_payline(22, {  2,  5,  6, 11, 14 }); // 23
  r &= add_payline(23, {  0,  5,  8, 11, 12 }); // 24
  r &= add_payline(24, {  2,  3,  6,  9, 14 }); // 25

  return r;
}

bool payline_classic::add_payline(int number, std::vector<int> payline) {
  auto it = paylines_.find(number);
  if(it != paylines_.end()) return false;
  paylines_[number] = payline;

  return true;
}

payline_classic::payline_result payline_classic::check_payline(int number, const std::vector<int>& out_symbols, bool is_freespin_on, int in_wild_symbol) {
  payline_classic::payline_result result; 
  //result.number = number + 1;
  result.number = number;

  payline& payline_info = paylines_[number];
  
  int symbols[5] = { out_symbols[payline_info[0]], out_symbols[payline_info[1]], out_symbols[payline_info[2]], out_symbols[payline_info[3]], out_symbols[payline_info[4]]};


  // 와일드 체크
  if(is_freespin_on) {
    std::cout << "is_freespin_on" << std::endl;
    // 5개 전부다 와일드
    if(symbols[0] == in_wild_symbol && symbols[1] == in_wild_symbol && symbols[2] == in_wild_symbol && symbols[3] == in_wild_symbol && symbols[4] == in_wild_symbol) {
      result.symbol = in_wild_symbol;
      result.sequence = 5;
      return result;
    }

    if(symbols[0] == in_wild_symbol && symbols[1] == ddslot_classic::symbols::Wild && symbols[2] == in_wild_symbol && symbols[3] == in_wild_symbol && symbols[4] == in_wild_symbol) {
      result.symbol = in_wild_symbol;
      result.sequence = 5;
      return result;
    }

    if(symbols[0] == in_wild_symbol && symbols[1] == in_wild_symbol && symbols[2] == in_wild_symbol && symbols[3] == ddslot_classic::symbols::Wild && symbols[4] == in_wild_symbol) {
      result.symbol = in_wild_symbol;
      result.sequence = 5;
      return result;
    }


    if(symbols[0] == in_wild_symbol && symbols[1] == ddslot_classic::symbols::Wild && symbols[2] == in_wild_symbol && symbols[3] == ddslot_classic::symbols::Wild && symbols[4] == in_wild_symbol) {
      result.symbol = in_wild_symbol;
      result.sequence = 5;
      return result;
    }


    // 와일드중에서 심볼 찾기
    for(auto i=0; i<5; i++) {
      if(symbols[i] != in_wild_symbol && symbols[i] != ddslot_classic::symbols::Wild) {
	result.symbol = symbols[i];
	break;
      }
    }

    // 심볼에서 몇개가 연결되는지 찾기
    for(auto i=1; i<5; i++) {
      if(symbols[i] != result.symbol && symbols[i] != in_wild_symbol && symbols[i] != ddslot_classic::symbols::Wild) {
	break;
      }
      result.sequence++;
    }

    /*
    if(result.symbol == -1) {
      result.symbol = symbols[0];
    }
    */
      // anybar 체크해줘야함
      const int max_reel_count = 5;
      auto any_sequence = 0;
      for(auto i=0; i<max_reel_count; i++) {
	if(symbols[i] == ddslot_classic::symbols::Bar3 || 
	   symbols[i] == ddslot_classic::symbols::Bar2 ||
	   symbols[i] == ddslot_classic::symbols::Bar  ||
	   symbols[i] == ddslot_classic::symbols::Wild || 
	   symbols[i] == in_wild_symbol) {
	  any_sequence++;
	} else {
	  break;
	}
      }

      if(any_sequence >= 3) {
	if(result.sequence < any_sequence) {
	  std::cout << "any 걸림" << std::endl;
	  std::cout << "any sequence: " << any_sequence << std::endl;
	  result.symbol = ddslot_classic::symbols::Any;
	  result.sequence = any_sequence;
	}
      }

  } else {
    result.symbol = symbols[0];
    
    bool is_seqence = true;
    if(symbols[0] == symbols[1] || symbols[1] == ddslot_classic::symbols::Wild) {
      if(is_seqence) result.sequence++;
    } else {
      is_seqence = false;
    }

    if(symbols[0] == symbols[2]) {
      if(is_seqence) result.sequence++;
    } else {
      is_seqence = false;
    }

    if(symbols[0] == symbols[3] || symbols[3] == ddslot_classic::symbols::Wild) {
      if(is_seqence) result.sequence++;
    } else {
      is_seqence = false;
    }

    if(symbols[0] == symbols[4]) {
      if(is_seqence) result.sequence++;
    } else {
      is_seqence = false;
    }
  }


  // anybar 체크해줘야함
  const int max_reel_count = 5;
  auto any_sequence = 0;
  for(auto i=0; i<max_reel_count; i++) {
    if(symbols[i] == ddslot_classic::symbols::Bar3 || 
       symbols[i] == ddslot_classic::symbols::Bar2 ||
       symbols[i] == ddslot_classic::symbols::Bar  ||
       symbols[i] == ddslot_classic::symbols::Wild) {
      any_sequence++;
    } else {
      break;
    }
  }

  if(any_sequence >= 3) {
    if(result.sequence < any_sequence) {
      std::cout << "any 걸림" << std::endl;
      std::cout << "any sequence: " << any_sequence << std::endl;
      result.symbol = ddslot_classic::symbols::Any;
      result.sequence = any_sequence;
    }
  }

  return result;
}

// 클래식 슬롯
ddslot_classic::ddslot_classic(int id)  {
  id_ = id;
  type_ = classic;
  current_freespin_count_ = 0;
  max_freespin_count_ = 0;
  std::cout << "ddslot_classic ctor called" << std::endl;
}

ddslot_classic::~ddslot_classic() {
  std::cout << "ddslot_classic dtor called" << std::endl;
}

bool ddslot_classic::init() {
  
  set_reel(0, { Scatter, Cherry, Seven, Bar3, Watermelon, Bar2, Mangosteen, Cherry, Bar3, Bar, Bell, Watermelon, Bar2, Cherry, Seven3, Bell, Bar, Mangosteen, Bar2, Scatter, Watermelon, Cherry, Seven, Mangosteen, Bar2, Scatter, Bar, Watermelon, Bar3, Bell, Seven3, Bar });

  set_reel(1, { Cherry,Bell, Seven3, Bar3, Mangosteen, Cherry, Seven, Bar, Watermelon, Seven3, Bar3, Cherry, Seven, Wild, Bar, Bar2, Mangosteen, Watermelon, Bar3, Bell, Scatter, Cherry, Bar, Mangosteen, Watermelon, Bar2, Bell, Cherry, Seven3, Seven, Mangosteen, Watermelon });

  set_reel(2, { Seven, Bar, Bar2, Scatter, Mangosteen, Cherry, Bar, Seven, Bar3, Watermelon, Seven3, Bell, Bar, Cherry, Mangosteen, Bar3, Bar2, Watermelon, Cherry, Bell, Seven, Bar3, Watermelon, Mangosteen, Bar2, Scatter, Bell, Watermelon,Bar, Mangosteen, Cherry, Seven3 });

  set_reel(3, { Seven, Mangosteen, Wild, Scatter, Bar, Watermelon, Mangosteen, Bar3, Seven3 ,Bar2, Watermelon, Cherry, Bar3, Mangosteen, Bar, Seven, Cherry, Watermelon ,Bell, Bar3, Mangosteen, Bar2, Bar, Bell ,Watermelon, Cherry, Bar2, Seven ,Bar3, Seven3, Cherry, Watermelon });

  set_reel(4,  { Watermelon, Scatter, Seven3, Seven, Mangosteen, Bar3, Bar, Watermelon, Bell, Mangosteen ,Bar3, Seven, Bar, Seven3, Cherry, Bar2, Bar3, Seven, Scatter, Bar, Bell, Cherry, Bar2, Seven, Watermelon, Bar, Seven3, Cherry, Bar3, Bar2, Mangosteen, Bell });

  bool r = true;
  r &= payline_.init();
  r &= paytable_.init();
  
  return r;
}

std::string ddslot_classic::spin(int bet) {
  update_timestamp();

  auto stop_points = random_generator_.get_random_number<int, 5>(0, 14);
  auto out_symbols = set_out_reels(stop_points);

  //std::string debug_symbols = "";

  std::string reel_top = "";
  std::string reel_mid = "";
  std::string reel_bottom = "";

  for(unsigned i=0; i<out_symbols.size(); i++) {
    /*
    debug_symbols += std::to_string(out_symbols[i]) + ", ";
    if(i == 2 || i == 5 || i == 8 || i == 11) {
      debug_symbols += " | ";
    }
    */
    if(i == 0 || i== 3 || i == 6 || i == 9 || i == 12) {
      reel_top += std::to_string(out_symbols[i]) + "  |  ";
    } else if(i == 1 || i== 4 || i == 7 || i == 10 || i == 13) {
      reel_mid += std::to_string(out_symbols[i]) + "  |  ";
    } else if(i == 2 || i== 5 || i == 8 || i == 11 || i == 14) {
      reel_bottom += std::to_string(out_symbols[i]) + "  |  ";
    }
  }

  std::cout << "-------------------------" << std::endl;
  std::cout << reel_top << std::endl;
  std::cout << reel_mid << std::endl;
  std::cout << reel_bottom << std::endl;
  std::cout << "-------------------------" << std::endl;

  //std::cout << debug_symbols << std::endl;

  bool is_freespin = false;
  if(current_freespin_count_ < max_freespin_count_) {
    is_freespin = true;
    std::cout << "프리스핀중- current spin: " << current_freespin_count_ << "/ max spin: " << max_freespin_count_ << std::endl;
    current_freespin_count_++;
  }

  std::vector<payline_classic::payline_result> payline_results;
  for(unsigned i=0; i<payline_.get_payline_count(); i++) {
    if(is_freespin) {
      payline_results.push_back(payline_.check_payline(i, out_symbols, is_freespin, 5));
    } else {
      payline_results.push_back(payline_.check_payline(i, out_symbols));
    }
  }

  auto win_credit = 0;

  std::vector<int> hit_numbers;
  std::vector<int> hit_sequences;
  std::vector<int> hit_win_credits;

  for(auto& result : payline_results) {

    result.win_credit = paytable_.get_credit(result.symbol, result.sequence);

    if(result.win_credit > 0) {
      std::cout << "========================================" << std::endl;
      std::cout << "히트된 심볼: " << result.symbol << std::endl;
      std::cout << "페이라인 번호: " << result.number << std::endl;
      std::cout << "연속된갯수: " << result.sequence << std::endl;
      std::cout << "========================================" << std::endl;
      hit_numbers.push_back(result.number);
      hit_sequences.push_back(result.sequence);
      hit_win_credits.push_back(result.win_credit);

      win_credit += result.win_credit;
    }


    if(result.symbol == ddslot_classic::symbols::Scatter) {
      if(current_freespin_count_ == 0 && result.sequence >= 3) {
	std::cout << "@@@@@@@@ 프린스핀 당첨 @@@@@@@@@" << std::endl;
	current_freespin_count_ = 1;
      }
      if(result.sequence == 3) {
	max_freespin_count_ += 10;
      } else if(result.sequence == 4) {
	max_freespin_count_ += 15;
      } else if(result.sequence == 5) {
	max_freespin_count_ += 20;
      }
    }

    //result.win_money = paytable_.get_win_money(심볼 번호, seq);
  }

  if(current_freespin_count_ >= max_freespin_count_) {
    current_freespin_count_ = 0;
    max_freespin_count_ = 0;
  }

  std::vector<int> out_symbols2 = out_symbols;
  out_symbols2[0] =  out_symbols[2];    out_symbols2[2] =  out_symbols[2-2];
  out_symbols2[3] =  out_symbols[3+2];  out_symbols2[5] =  out_symbols[5-2];
  out_symbols2[6] =  out_symbols[6+2];  out_symbols2[8] =  out_symbols[8-2];
  out_symbols2[9] =  out_symbols[9+2];  out_symbols2[11] = out_symbols[11-2];
  out_symbols2[12] = out_symbols[12+2]; out_symbols2[14] = out_symbols[14-2];
  

  json11::Json res = json11::Json::object {
      { "type", "spin_res" },
      { "ec", 0 },
      { "credit", 50000 },
      { "win_credit", win_credit },

      { "symbols", out_symbols2 },

      { "hit_count", static_cast<int>(hit_numbers.size()) },
      { "hit_numbers", hit_numbers },
      { "hit_sequences", hit_sequences },
      { "hit_win_credits", hit_win_credits },

      { "current_freespin_count", current_freespin_count_ },
      { "max_freespin_count", max_freespin_count_ },
      { "wild_symbol", -1 },
    };

  return res.dump();
}


bool ddslot_classic::reset() {
  current_freespin_count_ = 0;
  max_freespin_count_ = 0;
  return true;
}

bool ddslot_classic::is_seat() {

  std::cout << "슬롯 번호: " << id_ << std::endl;

  auto now = get_now();

  if(now - get_timestamp() < 1000 * 60) {
    return false;
  }

  std::cout << "선택 가능한 슬롯: " << id_ << std::endl;

  return true;
}

std::vector<int> ddslot_classic::set_out_reels(std::vector<int>& stop_points) {
  std::vector<int> out_symbols;
  auto i=0;
  for(auto stop_point : stop_points) {

    auto& reel = reels_[i];

    if(stop_point == 0) {
      out_symbols.push_back(reel[reel.size()-1]);
      out_symbols.push_back(reel[stop_point]);
      out_symbols.push_back(reel[stop_point+1]);
    } else if(stop_point == reel.size()-1) {
      out_symbols.push_back(reel[stop_point-1]);
      out_symbols.push_back(reel[stop_point]);
      out_symbols.push_back(reel[0]);
    } else {
      out_symbols.push_back(reel[stop_point-1]);
      out_symbols.push_back(reel[stop_point]);
      out_symbols.push_back(reel[stop_point+1]);
    }
    i++;
  }
  return out_symbols;
}
