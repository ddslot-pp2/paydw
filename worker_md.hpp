#ifndef WORKER_MD_HPP
#define WORKER_MD_HPP
#include <iostream>
#include <functional>
#include <thread>
#include <memory>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>

class worker_md {

  boost::asio::io_service io_service_;
  std::shared_ptr<boost::asio::io_service::work> work_ptr_;


public:
  worker_md();
  ~worker_md();

  static worker_md& get(){
    static worker_md obj;
    return obj;
  }

  void stop();
  void run(unsigned count);

  boost::asio::io_service& get_io_service() {
    return io_service_;
  }

  // bsonbuilder pointer

  // size of tail call  function

};

#endif
