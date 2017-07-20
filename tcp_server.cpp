#include "tcp_server.h"

#include "http_header.h"

using namespace Illuminati;

using boost::asio::ip::tcp;

tcp_server::tcp_server(boost::asio::io_service &io_service,
                       const std::string &port)
    : acceptor_(io_service,
                tcp::endpoint(tcp::v4(), u_short(std::stoi(port)))) {
  start_accept();
}

void tcp_server::start_accept() {
  tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());

  acceptor_.async_accept(new_connection->socket(),
                         [this, new_connection](const auto &e) {
                           this->handle_accept(new_connection, e);
                         });
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
                               const boost::system::error_code &error) {
  if (!error) {
    LOG("New connection with "
        << new_connection->socket().remote_endpoint().address().to_string());
    new_connection->start();
  }

  start_accept();
}
