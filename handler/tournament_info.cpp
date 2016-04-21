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

  auto index = find_user_index(uid, std::ref(tournament_infos));
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
    json11::Json::object obj = { 
      { "uid", result[i].uid },
      { "win_credit", result[i].win_credit },
      { "ranking", result[i].ranking },
      { "name", result[i].name },
      { "img", result[i].img } };
    results.push_back(obj);
  }
  
  json11::Json r = results;
  return r.dump();
}

std::string get_img_url(std::string fid) {
  if(fid == "") return fid;
  return "http://graph.facebook.com/" + fid + "/picture?width=500&height=500";
  //return "http://graph.facebook.com/" + fid + "/picture?type=large";
}

void tournament_refresh_req(HttpServer::Response& response, const ptree& payload) {

  auto zone = payload.get<int>("zone");
  auto uid = payload.get<int>("uid");
  auto name = payload.get<std::string>("name");
  auto fid = payload.get<std::string>("fid");
  auto bet_credit = payload.get<int>("bet_credit");

  std::cout << "zone: " << zone << std::endl;
  std::cout << "uid: " << uid << std::endl;
  std::cout << "fid: " << fid << std::endl;
  std::cout << "bet_credit: " << bet_credit << std::endl;

  if(!scheduler_md::get().get_tournament_on() || uid < 0) {
    std::cout << "현재 토너먼트 중이 아니라서 refresh 처리안함" << std::endl;
    json11::Json res = json11::Json::object {
      { "type", "tournament_refresh_res" },
      { "result", false }
    };
    std::string r = res.dump();
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
    return;
  }

  auto tournament_ptr = scheduler_md::get().get_tournament(zone);
  if(!tournament_ptr) {
    std::cout << "[error] does not exist tournament_ptr" << std::endl;
    return;
  }

  tournament_ptr->add_bet_credit(bet_credit);
  tournament_ptr->set_user_info(uid, std::make_tuple(name, fid));

  std::string query = "call refresh_tournament_user("+ std::to_string(zone) + "," + std::to_string(uid) + ");";

  std::vector<tournament_info> tournament_infos;

  auto rs = db_md::get().execute_query(query);
  while(rs->next()) {
    tournament_info tmp;
    tmp.uid = rs->getInt("uid");
    std::tuple<std::string, std::string> user_info = tournament_ptr->get_user_info(tmp.uid);
    std::string name = std::get<0>(user_info);
    std::string fid = std::get<1>(user_info);
    // uid 기준으로 name 가져오기
    tmp.name = name;
    tmp.win_credit = rs->getInt("win_credit");
    tmp.ranking = rs->getInt("ranking");
    tmp.img = get_img_url(fid);
    tournament_infos.push_back(tmp);
  }

  auto prize_pool = tournament_ptr->get_prize_pool();

  json11::Json res = json11::Json::object {
      { "type", "tournament_refresh_res" },
      { "result", true },
      { "zone", zone },
      { "prize_pool", prize_pool },
      { "tournament_infos", get_tournament_infos(uid, std::ref(tournament_infos)) }
    };
  
  tournament_infos.clear();

  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}

void tournament_update_req(HttpServer::Response& response, const ptree& payload) {

  auto zone = payload.get<int>("zone");
  auto uid = payload.get<int>("uid");
  auto win_credit = payload.get<int>("win_credit");
  auto name = payload.get<std::string>("name");
  auto fid = payload.get<std::string>("fid");
  auto bet_credit = payload.get<int>("bet_credit");

  if(!scheduler_md::get().get_tournament_on() || uid < 0) {
    std::cout << "현재 토너먼트 중이 아니라서 update 처리안함" << std::endl;
    json11::Json res = json11::Json::object {
      { "type", "tournament_update_res" },
      { "result", false }
    };
    std::string r = res.dump();
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
    return;
  }

  std::cout << "zone: " << zone << std::endl;
  std::cout << "uid: " << uid << std::endl;
  std::cout << "fid: " << fid << std::endl;
  std::cout << "win credit: " << win_credit << std::endl;
  std::cout << "bet_credit: " << bet_credit << std::endl;

  auto tournament_ptr = scheduler_md::get().get_tournament(zone);

  if(!tournament_ptr) {
    std::cout << "[error] does not exist tournament_ptr" << std::endl;
    return;
  }

  tournament_ptr->add_bet_credit(bet_credit);
  tournament_ptr->set_user_info(uid, std::make_tuple(name, fid));
  

  // db 작업
  std::string query = "call update_tournament_user("+ std::to_string(zone) + "," + std::to_string(uid) + "," + std::to_string(win_credit) + ")";
  std::cout << "query: " << query << std::endl;

  std::vector<tournament_info> tournament_infos;
  
  auto rs = db_md::get().execute_query(query);
  while(rs->next()) {
    tournament_info tmp;
    tmp.uid = rs->getInt("uid");
    std::tuple<std::string, std::string> user_info = tournament_ptr->get_user_info(tmp.uid);
    std::string name = std::get<0>(user_info);
    std::string fid = std::get<1>(user_info);

    // uid 기준으로 name 가져오기
    tmp.name = name;
    tmp.win_credit = rs->getInt("win_credit");
    tmp.ranking = rs->getInt("ranking");
    tmp.img = get_img_url(fid);
    tournament_infos.push_back(tmp);
  }

  auto prize_pool = tournament_ptr->get_prize_pool();

  json11::Json res = json11::Json::object {
      { "type", "tournament_update_res" },
      { "result", true },
      { "zone", zone },
      { "prize_pool", prize_pool },
      { "tournament_infos", get_tournament_infos(uid, std::ref(tournament_infos)) }
    };

  tournament_infos.clear();  

  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}

void tournament_reward_req(HttpServer::Response& response, const ptree& payload) {


}

void sync_timestamp_req(HttpServer::Response& response, const ptree& payload) {
  auto local_timestamp = payload.get<int>("local_timestamp");
  int timestamp = static_cast<int>(std::time(0));

  std::cout << "server timestamp: " << timestamp << std::endl;
  std::cout << "local timestamp: " << local_timestamp << std::endl;

  int sync_timestamp = timestamp - local_timestamp;

  std::cout << "시간차이: " << sync_timestamp << std::endl;  

  json11::Json res = json11::Json::object {
      { "type", "sync_timestamp_res" },
      { "sync_timestamp", sync_timestamp }
    };
  std::string r = res.dump();
  response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
}
