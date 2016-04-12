#include "tournament_info.hpp"
#include <cstdint>
#include "../scheduler_md.hpp"
#include "../db_md.hpp"
#include "../worker_md.hpp"

int find_user_index(int uid, const std::vector<tournament_info>& tournament_infos) {
  int i=0;
  for(auto& r : tournament_infos) {
    if(r.uid == uid) return i;
    i++;
  }
  return -1;
}

std::string get_tournament_infos(int uid, const std::vector<tournament_info>& tournament_infos) {

  const int max_tournament_pool = 3;

  auto index = find_user_index(uid, tournament_infos);
  auto start = 0;
  auto end = tournament_infos.size()-1;

  if(index == 0) {
    end = start + 2;
  } else if(index == tournament_infos.size()-1) {
    start = end - 2;
  } else {
    start = index - 1;
    end = index + 1;
  }

  if(tournament_infos.size() < max_tournament_pool) {
    start = 0;
    end = tournament_infos.size() -1;
  }

  tournament_info result[max_tournament_pool];
  auto j=0;
  for(unsigned i=start; i<end+1; i++) {
    result[j++] = tournament_infos[i];
  }

  json11::Json::array results = {};
  for(auto i=0; i<tournament_infos.size(); i++) {
    json11::Json::object obj = { { "uid", result[i].uid },
                       	       { "ranking", result[i].ranking },
			       { "img", result[i].img } };
    results.push_back(obj);
  }

  json11::Json r = results;
  return r.dump();
}

void tournament_refresh_req(HttpServer::Response& response, const ptree& payload) {

  auto uid = payload.get<int>("uid");
  auto zone = payload.get<int>("zone");
  auto ranking = 100;
  std::string imgs[3] = { "", };

  std::cout << "uid: " << uid << std::endl;
  std::cout << "zone: " << zone << std::endl;

  if(!scheduler_md::get().get_tournament_on() || uid < 0) {
    std::cout << "현재 토너먼트 중이 아니라서 refresh 처리안함" << std::endl;
    return;
  }

  std::string query = "call refresh_tournament_user("+ std::to_string(zone) + "," + std::to_string(uid) + ");";

  std::vector<tournament_info> tournament_infos;

  auto rs = db_md::get().execute_query(query);
  while(rs->next()) {
    tournament_info tmp;
    tmp.uid = rs->getInt("uid");
    tmp.win_credit = rs->getInt("win_credit");
    tmp.ranking = rs->getInt("ranking");
    tmp.img = rs->getString("img");
    tournament_infos.push_back(tmp);
  }

  json11::Json res = json11::Json::object {
      { "type", "tournament_refresh_res" },
      { "zone", zone },
      { "tournament_infos", get_tournament_infos(uid, tournament_infos) }
    };
  

  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}

void tournament_update_req(HttpServer::Response& response, const ptree& payload) {

  auto uid = payload.get<int>("uid");
  auto zone = payload.get<int>("zone");
  auto win_credit = payload.get<int>("win_credit");
  auto ranking = 100;
  std::string imgs[3] = { "", };

  if(!scheduler_md::get().get_tournament_on() || uid < 0) {
    std::cout << "현재 토너먼트 중이 아니라서 update 처리안함" << std::endl;
    return;
  }

  std::cout << "uid: " << uid << std::endl;
  std::cout << "zone: " << zone << std::endl;
  std::cout << "win credit: " << win_credit << std::endl;

  tournament_ptr tournament = scheduler_md::get().get_tournament(zone);
  if(tournament) {
    tournament->add_win_credit(win_credit);
  }

  // db 작업
  std::string query = "call update_tournament_user("+ std::to_string(zone) + "," + std::to_string(uid) + "," + std::to_string(win_credit) + ")";
  std::cout << "query: " << query << std::endl;

  std::vector<tournament_info> tournament_infos;
  
  auto rs = db_md::get().execute_query(query);
  while(rs->next()) {
    tournament_info tmp;
    tmp.uid = rs->getInt("uid");
    tmp.win_credit = rs->getInt("win_credit");
    tmp.ranking = rs->getInt("ranking");
    tmp.img = rs->getString("img");
    tournament_infos.push_back(tmp);
  }

  json11::Json res = json11::Json::object {
      { "type", "tournament_update_res" },
      { "zone", zone },
      { "tournament_infos", get_tournament_infos(uid, tournament_infos) }
    };

  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}

void tournament_reward_req(HttpServer::Response& response, const ptree& payload) {


}
