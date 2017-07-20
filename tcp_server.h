#pragma once

#include <boost/asio.hpp>
#include "tcp_connection.h"

namespace Illuminati {

/*!
 * \brief The tcp_server class is connects manager
 */
class tcp_server {
 public:
  tcp_server(boost::asio::io_service& io_service, const std::string& port);

 private:
  void start_accept();

  void handle_accept(tcp_connection::pointer new_connection,
                     const boost::system::error_code& error);

  boost::asio::ip::tcp::acceptor acceptor_;
};
}
