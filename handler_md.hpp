#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <functional>
#include <string>
#include "server_http.hpp"
using namespace boost::property_tree;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

class handler_md {

public:
  static handler_md& get() {
    static handler_md obj;
    return obj;
  }

  bool init();

  bool is_payload(const std::string type) {
    if(m_.find(type) == m_.end()) {
      return false;
    }
    return true;
  }

  bool add_payload(const std::string& type, std::function<void(HttpServer::Response& response, ptree& payload)> handler_fn);

  

  //private:
  std::map<std::string, std::function<void(HttpServer::Response& res, ptree& payload)>> m_;

};
