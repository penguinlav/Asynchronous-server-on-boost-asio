#pragma once

#include <boost/asio.hpp>

namespace Illuminati {

/*!
 * \brief The tcp_connection class stores a socket
 */
class tcp_connection : public std::enable_shared_from_this<tcp_connection>,
                       private boost::noncopyable {
 public:
  typedef std::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service);

  boost::asio::ip::tcp::socket& socket();

  void start();

  ~tcp_connection();

 private:
  tcp_connection(boost::asio::io_service& io_service);
  void handle_write(const boost::system::error_code&);
  void handle_read(const boost::system::error_code&);

  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf m_request;
  boost::asio::streambuf m_response;
};
}
