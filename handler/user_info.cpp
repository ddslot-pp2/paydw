#include "user_info.hpp"
#include "../json11.hpp"
#include <cstdint>

void login_req(HttpServer::Response& response, const ptree& payload) {

  auto uid = payload.get<std::string>("uid");

  std::cout << uid << std::endl;

  json11::Json res = json11::Json::object {
      { "type", "test_res" },
      { "result", false },
      { "uid", "12314123213" },
      { "key3", json11::Json::array { 1, 2, 3 } },
    };

  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}


//curl -H "Content-Type: application/json" -X POST -d '{"type":"spin_req", "uid": "1231414", "bet": 50, "id" : 0}' http://127.0.0.1:8081/payload



