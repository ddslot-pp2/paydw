#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Added for the default_resource example
#include <fstream>
#include <boost/filesystem.hpp>
#include <array>
#include "server_http.hpp"
#include "json11.hpp"
#include "handler_md.hpp"
#include "slot_md.hpp"

using namespace std;
//Added for the json-example:
using namespace boost::property_tree;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

int main() {
  //HTTP-server at port 8081 using 4 threads
  HttpServer server(8081, 4);
    
  //Add resources using path-regex and method-string, and an anonymous function
  //POST-example for the path /string, responds the posted string
  server.resource["^/string$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
    //Retrieve string:
    auto content=request->content.string();
    //request->content.string() is a convenience function for:
    //stringstream ss;
    //ss << request->content.rdbuf();
    //string content=ss.str();
        
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
  };
    
  //POST-example for the path /json, responds firstName+" "+lastName from the posted json
  //Responds with an appropriate error message if the posted json is not valid, or if firstName or lastName is missing
  //Example posted json:
  //{
  //  "firstName": "John",
  //  "lastName": "Smith",
  //  "age": 25
  //}
  server.resource["^/payload"]["POST"]=[](HttpServer::Response& response, std::shared_ptr<HttpServer::Request> req) {
    try {
      ptree pt;
      read_json(req->content, pt);

      auto type = pt.get<string>("type");
	
      if(handler_md::get().is_payload(type)) {
	handler_md::get().m_[type](response, pt);
      } else {
	std::cout << "[error] 핸들러 없음" << std::endl;
	json11::Json res = json11::Json::object {
	  { "type", "not_found_type" },
	  { "ec", 45893 },
	};

	std::string r = res.dump();
	response << "HTTP/1.1 200 OK\r\nContent-Length: " << r.length() << "\r\n\r\n" << r;
      }
    }
    catch(exception& e) {
      response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
    }
  };
    

  server.resource["^/slot/([0-9]+)$"]["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {

    auto id = std::stoi(request->path_match[1]);
    std::cout << "id: " << id << std::endl;

    auto content=request->content.string();
    response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;    
  };


  server.default_resource["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {

    string content="Could not open path " + request->path;
    //response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;

    //std::cout <<" 11111111111111111111" << std::endl;

    json11::Json my_json = json11::Json::object {
      { "key1", "value1" },
      { "key2", false },
      { "key3", json11::Json::array { 1, 2, 3 } },
    };
    std::string json_str = my_json.dump();
    response << json_str;
  };
  
  handler_md::get().init();

  slot_md::get().init();
 
  thread server_thread([&server](){
      server.start();
    });

  //Wait for server to start so that the client can connect
  this_thread::sleep_for(chrono::seconds(1));
            
  server_thread.join();
    
  return 0;
}
