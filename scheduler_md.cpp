#include "scheduler_md.hpp"
#include <thread>
#include <iostream>
#include <chrono>
#include "db_md.hpp"
#include "worker_md.hpp"

tournament::tournament() : bet_credits_(0) {

}

tournament::tournament(int start_bet, int end_bet) : bet_credits_(0) {
  start_bet_ = start_bet;
  end_bet_ = end_bet;
}

void tournament::add_bet_credit(int bet_credit) {
  bet_credits_ += bet_credit;
}

std::tuple<std::string, std::string> tournament::get_user_info(int uid) {
  m.lock();
  if(user_infos_.find(uid) != user_infos_.end()) {
    m.unlock();
    return user_infos_[uid];
  }
  m.unlock();
  return std::tuple<std::string, std::string>("", "");
}

void tournament::set_user_info(int uid, std::tuple<std::string, std::string> user_info) {
  m.lock();
  user_infos_[uid] = user_info;
  m.unlock();
}

int tournament::get_prize_pool() {
  const auto update_interval = 10;
  const auto max_pool_area = 30;

  auto bet_pool = static_cast<int>(bet_credits_ / 5);

  auto remaining_time = 330 - (330 - scheduler_md::get().get_timestamp() % 330);

  auto pool_area = static_cast<int>(remaining_time / update_interval) + 1;
  if(pool_area > max_pool_area) pool_area = max_pool_area;

  //std::cout << "pool_area: " << pool_area << std::endl;

  auto avg_pool_prize = end_bet_ * 4;
  //std::cout << "before avg: " <<  avg_pool_prize << std::endl;
  avg_pool_prize = avg_pool_prize * (pool_area);

  //std::cout << "after avg: " <<  avg_pool_prize << std::endl;

  int prize_pool = static_cast<int>(avg_pool_prize) + bet_pool;
  //std::cout << "prize_pool: " << prize_pool << std::endl;
  return prize_pool;
}

void tournament::reset() {
  m.lock();
  user_infos_.clear();
  m.unlock();
  bet_credits_ = 0;
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

  /*
  for(unsigned i=0; i<max_tournament_zone; i++) {
    tournaments_.push_back(std::make_shared<tournament>());
  }
  */
  tournaments_.push_back(std::make_shared<tournament>(50, 500));
  tournaments_.push_back(std::make_shared<tournament>(500, 5000));
  tournaments_.push_back(std::make_shared<tournament>(5000, 1000000));
}

void scheduler_md::run() {

  while(is_run_) {
    // timestamp 기준으로 330기준으로 현재 대기인지 시작인지 구별해야함

    // 동기 타이머 써서 스레드를 블럭시킴
    auto remaining_time = 330 - get_timestamp() % 330;
    std::cout << "remaining_time: " << remaining_time << std::endl;

    if(remaining_time <= 30) {
      boost::asio::deadline_timer t(io_service_, boost::posix_time::seconds(remaining_time));
      std::cout << "토너먼트 대기 시작" << std::endl;
      is_tournament_on_ = false;
      // wait for remaining time
      t.wait();
      std::cout << "토너먼트 대기 종료" << std::endl;
    }

    is_tournament_on_ = false;
    reset_tournament();
    for(auto tournament : tournaments_) {
      tournament->reset();
    }
    is_tournament_on_ = true;

    remaining_time = 330 - get_timestamp() % 330;
    if(remaining_time-30 <= 0) remaining_time = 0;

    std::cout << "토너먼트 다시 시작" << std::endl;
    boost::asio::deadline_timer t(io_service_, boost::posix_time::seconds(remaining_time-30));
    t.wait();
    
    std::cout << "토너먼트 종료" << std::endl;
  }
}

/*
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
  
      std::cout << "start reset tournament called" << std::endl;
      worker_md::get().get_io_service().post(std::bind(&scheduler_md::reset_tournament, this));
      std::cout << "end reset tournament called" << std::endl;

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
*/

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
  is_tournament_on_ = false;
  std::cout << "reset tournament called" << std::endl;
  std::string query = "call reset_tournaments();";
  auto r = db_md::get().execute(query);
  if(!r) {
    std::cout << "sql query result is false" << std::endl;
  } else {
    std::cout << "토너먼트 리셋" << std::endl;
  }
  is_tournament_on_ = true;
}
