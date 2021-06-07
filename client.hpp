#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <string>


class Client
{
private:
  boost::asio::io_service service_;
  boost::asio::ip::tcp::socket socket_;
  boost::system::error_code error;
  boost::asio::streambuf buf;

public:
  Client() : socket_(service_){};

  void ConnectToServer(std::string const &ip_address, uint16_t port)
  {
    socket_.connect(boost::asio::ip::tcp::endpoint(
      boost::asio::ip::address_v4::from_string(ip_address), port));
    if (!error) {
      std::cout << "Connect to " << ip_address << ":" << port << std::endl;
      std::cout << ReadFunc();
    } else {
      std::cerr << "An error occurred: " << error.message() << std::endl;
    }
  }

  std::string ReadFunc()
  {
    boost::asio::read_until(socket_, buf, "\n");
    std::string data = boost::asio::buffer_cast<const char *>(buf.data());
    return data;
  }

  void WriteFunc(const std::string &msg)
  {
    boost::asio::write(socket_, boost::asio::buffer(msg + "\n"));
    if (!error) {
      std::cout << "Message sent to " << socket_.remote_endpoint().address()
                << ":" << socket_.remote_endpoint().port()
                << ", with data: " << msg << std::endl;
    } else {
      std::cerr << "An error occurred: " << error.message() << std::endl;
    }
  }

  static void HelpBox()
  {
    std::cout << "- help\n"
              << "       This help message\n"
              << "- exit\n"
              << "       Quit the session\n"
              << "- connect <string>\n"
              << "       Connect to server\n"
              << "- send <string>\n"
              << "       Send message\n"
              << std::endl;
  }
};
void CommandFunc(Client *client);

#endif