#include <cstdint>
#include "slot_info.hpp"
#include "../json11.hpp"
#include "../slot_md.hpp"
#include "../slot.hpp"

void seat_req(HttpServer::Response& response, const ptree& payload) {

  std::cout << "@@ seat req called@" << std::endl;

  auto uid = payload.get<std::string>("uid");
  auto uid64 = std::stoll(uid);

  auto type = static_cast<slot_type>(payload.get<int>("slot_type"));
  auto id = slot_md::get().get_slot_id(type);

  json11::Json res = json11::Json::object {
      { "type", "seat_res" },
      { "id", id }
    };

  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}

void spin_req(HttpServer::Response& response, const ptree& payload) {
  auto uid = payload.get<std::string>("uid");
  auto id = payload.get<int>("id");
  auto bet = payload.get<int>("bet");
  std::cout << "spin_req" << std::endl;
  // auto spin_result = slots[id]->spin(bet);

  //std::cout << uid << std::endl;
  auto slot = slot_md::get().get_slot(id);

  std::string res = "";

  if(slot) {
    res = slot->spin(bet);
  } else {

  }

  response << "HTTP/1.1 200 OK\r\nContent-Length: " << res.length() << "\r\n\r\n" << res;
}
