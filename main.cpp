#include <boost/asio.hpp>
#include <fstream>

#include <iostream>

#include "tcp_server.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: localhost<port>\n";
    return 1;
  }

  try {
    boost::asio::io_service io_service;
    Illuminati::tcp_server server(io_service, argv[1]);
    io_service.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
