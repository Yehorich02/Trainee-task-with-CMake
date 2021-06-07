#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>

class con_handler : public boost::enable_shared_from_this<con_handler>
{
private:
  boost::asio::ip::tcp::socket socket_;
  std::string message = "Successfully connected!\r\n";
  enum {
    max_length = 1024
  };
  char data[max_length]{};
  bool started_ = false;

  void handle_read(boost::system::error_code const &error,
    size_t bytes_received)
  {
    if (bytes_received > 0) {
      std::cout << "Message received from IP: "
                << socket_.remote_endpoint().address()
                << ", with data: " << data;
      socket_.close();
    }

    if (error) {
      std::cerr << "An error occurred: " << error.message() << std::endl;
      socket_.close();
    }
  }

  void handle_write(const boost::system::error_code &error,
    size_t bytes_transferred)
  {
    (void)bytes_transferred;
    if (error) {
      std::cerr << "An error occurred(handle_write): " << error.message()
                << std::endl;
      socket_.close();
    }
  }

public:
  typedef boost::shared_ptr<con_handler> pointer;

  explicit con_handler(boost::asio::io_service &io_service)
    : socket_(io_service) {}

  static auto create(boost::asio::io_service &io_service)
  {
    return pointer(new con_handler(io_service));
  }

  boost::asio::ip::tcp::socket &socket() { return socket_; }

  void start()
  {
    if (!started_) {
      do_write();
      started_ = true;
    }
    do_read();
  }

  // function for sending messages from a client
  void do_write()
  {
    auto handler = [ObjectPtr = shared_from_this()](const boost::system::error_code &error,
                     size_t bytes_transferred) {
      ObjectPtr->handle_write(error, bytes_transferred);
    };
    socket_.async_write_some(boost::asio::buffer(message, max_length), handler);
  }

  // function for reading messages from a client
  void do_read()
  {
    socket_.async_read_some(
      boost::asio::buffer(data, max_length),
      [ObjectPtr = shared_from_this()](const boost::system::error_code &error,
        size_t bytes_transferred) {
        ObjectPtr->handle_read(error, bytes_transferred);
      });
  }
};

class Server
{
private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  bool port_ = false;
  bool started_ = false;
  uint16_t port_s{};

  // asynchronous accept operation
  void start_accept()
  {
    con_handler::pointer connection = con_handler::create(io_service_);

    acceptor_.async_accept(connection->socket(),
      [this, connection](const boost::system::error_code &error) {
        handle_accept(connection, error);
      });
    if (!started_) {
      std::cout << "Server started on " << port_s << std::endl;
      started_ = true;
    }
    io_service_.run_one();
  }

  void handle_accept(const con_handler::pointer &connection,
    const boost::system::error_code &error)
  {
    if ((!error) && port_) {

      connection->start();
    } else {
      std::cerr << "An error occurred: " << error.message() << std::endl;
      connection->socket().close();
    }
    start_accept();
  }

public:
  // constructor for accepting connection
  Server() : io_service_(), acceptor_(io_service_) {}

  static void HelpBox()
  {
    std::cout << "- help\n"
              << "       This help message\n"
              << "- exit\n"
              << "       Quit the session\n"
              << "- start\n"
              << "       Start server\n"
              << "- stop\n"
              << "       Stop server\n"
              << "- SetPort <int>\n"
              << "        Set port or server\n"
              << std::endl;
  }

  // function to set the port for the server
  void SetPort(uint16_t port)
  {
    auto endpoint =
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    std::cout << "Server will be on port: " << port << std::endl;

    port_ = true;
    port_s = port;
  }

  void start()
  {
    start_accept();
    io_service_.run_one();
  }

  void stop() { io_service_.stop(); }
};

void CommandFunc(Server &srv);

#endif