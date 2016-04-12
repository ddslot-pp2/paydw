#include "handler_md.hpp"
#include "handler/user_info.hpp"
#include "handler/slot_info.hpp"
#include "handler/tournament_info.hpp"

bool handler_md::add_payload(const std::string& type, std::function<void(HttpServer::Response& response, ptree& payload)> handler_fn) {
  return m_.insert(std::make_pair(type, handler_fn)).second;
}

#define ADD_PAYLOAD(x) add_payload(#x, x);

bool handler_md::init() {
  auto r = true;

  // 유저 정보
  r &= ADD_PAYLOAD(login_req);
  r &= ADD_PAYLOAD(uid_req);

  // 게임 정보 
  r &= ADD_PAYLOAD(seat_req);
  r &= ADD_PAYLOAD(spin_req);

  // 각종 정보들
  r &= ADD_PAYLOAD(tournament_refresh_req);
  r &= ADD_PAYLOAD(tournament_update_req);
  r &= ADD_PAYLOAD(tournament_reward_req);

  return r;
}
