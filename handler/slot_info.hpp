#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../server_http.hpp"
using namespace boost::property_tree;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

void seat_req(HttpServer::Response& response, const ptree& payload);
void spin_req(HttpServer::Response& response, const ptree& payload);
