#include "tcp_connection.h"
#include "http_header.h"

using namespace Illuminati;

tcp_connection::pointer tcp_connection::create(
    boost::asio::io_service &io_service) {
  static int count;
  count++;
  LOG("Create socket #" << count);
  return std::shared_ptr<tcp_connection>(
      new tcp_connection(io_service), [k = count](auto p) {
        LOG("Del socket #" << k);
        delete p;
      });
}

boost::asio::ip::tcp::socket &tcp_connection::socket() { return socket_; }

void tcp_connection::start() {
  boost::asio::async_read_until(
      socket_, m_request,
      '\r', [ptr = shared_from_this()](const auto &e, auto) {
        ptr->handle_read(e);
      });
}

tcp_connection::~tcp_connection() { LOG("Del connection with"); }

tcp_connection::tcp_connection(boost::asio::io_service &io_service)
    : socket_(io_service) {}

void tcp_connection::handle_write(const boost::system::error_code &) {}

void tcp_connection::handle_read(const boost::system::error_code &e) {
  if (!e) {
    http_header header;
    header << m_request;
    header >> m_response;

    boost::asio::async_write(
        socket_, m_response, [ptr = shared_from_this()](auto &e, auto) {
          ptr->handle_write(e);
        });
  } else {
    LOG(e.message());
  }
}
