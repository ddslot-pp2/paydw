#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../server_http.hpp"
#include "../json11.hpp"
using namespace boost::property_tree;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

struct tournament_info {
  int uid;
  int win_credit;
  int ranking;
  std::string name;
  std::string img;
};

int find_user_index(int uid, const std::vector<tournament_info>& tournament_infos);
std::string get_tournament_infos(int uid, const std::vector<tournament_info>& tournament_infos);
std::string get_img_url(std::string fid);

void tournament_refresh_req(HttpServer::Response& response, const ptree& payload);
void tournament_update_req(HttpServer::Response& response, const ptree& payload);
void tournament_reward_req(HttpServer::Response& response, const ptree& payload);

void sync_timestamp_req(HttpServer::Response& response, const ptree& payload);
