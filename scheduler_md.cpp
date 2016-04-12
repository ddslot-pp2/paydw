#include "scheduler_md.hpp"
#include <thread>
#include <iostream>
#include <chrono>
#include "db_md.hpp"

tournament::tournament() : win_credit_(0) {

}

void tournament::add_win_credit(int win_credit) {
  // 10% 적립
  win_credit_ += win_credit;
}

std::string tournament::get_name(int uid) {
  m.lock();
  if(names_.find(uid) != names_.end()) {
    m.unlock();
    return names_[uid];
  }
  m.unlock();
  return "";
}

void tournament::set_name(int uid, std::string name) {
  m.lock();
  names_[uid] = name;
  //if(names_.find(uid) != names_.end()) names_[uid] = name;
  m.unlock();
}

scheduler_md::scheduler_md() : is_run_(true), is_tournament_on_(false), remaining_time_(0) {
  type_ = tournament_type::end;
}

scheduler_md::~scheduler_md() {

}

void scheduler_md::init() {
  tournament_count_ = get_timestamp() / 330;
  std::cout << "토너먼트 " << tournament_count_  << "번쨰" << std::endl;
  remaining_time_ = 330 - get_timestamp() % 330;
  if(remaining_time_ > 30) {
    is_tournament_on_ = true;
    type_ = tournament_type::start;
  }

  for(unsigned i=0; i<max_tournament_zone; i++) {
    tournaments_.push_back(std::make_shared<tournament>());
  }
}

void scheduler_md::run() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  while(is_run_) {
    if(type_ == tournament_type::start) {
      std::cout << "이벤트 진행중: " << remaining_time_ << std::endl;
    } else {
      std::cout << "대기 시간: " << remaining_time_ << std::endl;
    }

    remaining_time_--;

    if(remaining_time_ > 0 && remaining_time_ <= 30) {
      is_tournament_on_ = false;
      type_ = tournament_type::end;
    } else if(remaining_time_ < 0 ) {
      // 토너먼트 테이블 정리해주기
      
      std::string query = "call reset_tournaments();";
      auto r = db_md::get().execute(query);
      if(!r) {
	std::cout << "sql query result is false" << std::endl;
      } else {
	std::cout << "토너먼트 리셋" << std::endl;
      }

      //reset_tournament();
      tournament_count_ = get_timestamp() / 330;
      //std::cout << "토너먼트 " << tournament_count_  << "번쨰" << std::endl;
      is_tournament_on_ = true;
      remaining_time_ = 330;
      type_ = tournament_type::start;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int scheduler_md::get_timestamp() {
  return time(0);
}

tournament_ptr scheduler_md::get_tournament(int zone) {

  if(static_cast<int>(tournaments_.size()) > zone && zone >= 0) { 
    return tournaments_[zone];
  }
  
  return nullptr;
}

void scheduler_md::reset_tournament() {
  std::string query = "call reset_tournament()";
  auto r = db_md::get().execute(query);
  if(!r) {
    
  }
}
