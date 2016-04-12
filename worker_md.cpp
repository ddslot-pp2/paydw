#include "worker_md.hpp"

worker_md::worker_md() {

}

worker_md::~worker_md() {

}

void worker_md::stop() {
  work_ptr_.reset();
}

void worker_md::run(unsigned count) {
  work_ptr_.reset( new boost::asio::io_service::work(io_service_) );

  std::vector<std::shared_ptr<std::thread>> threads;
  for (unsigned i = 0; i < count; ++i) {
    std::shared_ptr<std::thread> t( new std::thread(std::bind(static_cast<size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run), &io_service_)));
    threads.push_back(t);
  }
  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < threads.size(); ++i) {
    threads[i]->join();
  }

  std::cout << "woker_md stop" << std::endl;
}
