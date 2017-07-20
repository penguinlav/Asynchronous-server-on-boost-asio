#pragma once
#include <boost/asio.hpp>

#include <iostream>

#define DEBUG_ON
#ifdef DEBUG_ON
#define LOG(str)                   \
  do {                             \
    std::cout << str << std::endl; \
  } while (0);
#elif
#define LOG(str)
#endif

namespace Illuminati {

/*!
 * \brief The http_header struct  parses at the query and forms the response
 */
struct http_header {
  std::string method;
  std::string url;
  std::string version;
  std::string host;

  http_header& operator<<(boost::asio::streambuf& buf);

  http_header& operator>>(boost::asio::streambuf& buf);

 private:
  std::map<std::string, std::function<bool(std::istream&)>> key_words = {
      {"GET",
       [this](std::istream& is) -> bool {
         is >> url;
         is >> version;
         version.erase(version.begin(), version.begin() + 5);

         LOG("GET ");
         LOG("url: " << url);
         LOG("verison http: " << version);

         return false;
       }},
      {"Host:", [this](std::istream& is) -> bool {
         is >> host;

         LOG("Host: ");
         LOG(host);

         return false;
       }}};
};
}
