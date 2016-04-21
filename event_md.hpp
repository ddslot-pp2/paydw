#ifndef EVENT_MD_HPP
#define EVENT_MD_HPP
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <functional>

class credit_event : public std::enable_shared_from_this<credit_event> {

public:
  credit_event() {}
  ~credit_event() {}
  
  void init();
  void start_event(const boost::system::error_code& ec);
  void end_event(const boost::system::error_code& ec);

  boost::asio::io_service io;
};


class event_md {

public:

  event_md();
  ~event_md();

  static event_md& get() {
    static event_md obj;
    return obj;
  }

  void end_game_event();

  bool init();

  bool is_event_;  
  std::shared_ptr<credit_event> credit_event_;
};

#endif
