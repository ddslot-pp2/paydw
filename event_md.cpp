#include "event_md.hpp"
#include <iostream>
#include <thread>
#include "worker_md.hpp"
#include <future>
#include <chrono>

void credit_event::init() {
  std::cout << " wait for event 1" << std::endl;

  //boost::asio::io_service::work work(io);
  while(true) {
    boost::asio::deadline_timer timer(io);
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait(boost::bind(&credit_event::start_event, shared_from_this(),boost::asio::placeholders::error));
    
    if(io.stopped()) {
      io.reset();
    }
    io.run();
    std::cout << " wait for event 2" << std::endl;
  }

}

void credit_event::start_event(const boost::system::error_code& ec) {

  std::cout << "step 1" << std::endl;
  
  boost::asio::deadline_timer timer(io);
  timer.expires_from_now(boost::posix_time::seconds(3));
  timer.async_wait(boost::bind(&credit_event::end_event, shared_from_this(),boost::asio::placeholders::error));
  io.run();

  std::cout << "step 2" << std::endl;
  
  if(ec) {
    std::cout << ec << std::endl;
    std::cout << "error timer" << std::endl;
    return;
  }

  std::cout << "5초짜리 이벤트 종료" << std::endl;
};

void credit_event::end_event(const boost::system::error_code& ec) {

  std::cout << "4초 이벤트 정리 시작" << std::endl;
  if(ec) {
    std::cout << ec << std::endl;
    std::cout << "[error] 4초 이벤트 정리 타이머" << std::endl;
  }
  std::cout << "4초 이벤트 정리 종료" << std::endl;
};



event_md::event_md() {
  std::cout << "event_md ctor called" << std::endl;
}

event_md::~event_md() {

}

void end_game_event() {

}

bool event_md::init() {
  
  //credit_event_ = std::make_shared<credit_event>();
  //credit_event_->init();

  while(1) {
    is_event_ = true;
    std::cout << " 10초간 이벤트 진행하겠습니다." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    is_event_ = false;

    std::cout << " 10초간 이벤트 종료." << std::endl;
    auto a = std::async(std::launch::async, []{ 
	std::cout << "working working working" << std::endl;
	for(int i=0; i<20; i++) {
	  std::cout << i << std::endl;
	}
      });

    std::cout << " 대기시간 5초 시작" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    a.wait();
  
    std::cout << " 대기시간 5초 종료" << std::endl;
  }
 
  return true;
}

