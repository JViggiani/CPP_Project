#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include "connectionhandler.h"
#include <boost/array.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

//void handleConnection(std::shared_ptr<tcp::socket>& socket) {
void handleConnection(tcp::socket socket) {
	ConnectionHandler handler;
	handler.start();
	for (;;) {
		boost::array<char, 128> buf;
		boost::system::error_code error;

		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error == boost::asio::error::eof) {
			std::cout << "Connection closed cleanly by peer." << std::endl;
			break; // Connection closed cleanly by peer.
		}
		else if (error)
			throw boost::system::system_error(error); // Some other error.

		handler.queueMessage(buf.data(), len);
		if (handler.quitReceived()) {
			break;
		}
	}
	handler.join();
	//	std::string message = make_daytime_string();
	//
	//	boost::system::error_code ignored_error;
	//	boost::asio::write(*socket, boost::asio::buffer(message), ignored_error);
	//	sleep(10);
	//	boost::asio::write(*socket, boost::asio::buffer(message), ignored_error);
}

int main() {
	try {
		boost::asio::io_service io_service;
		std::cout << "Started server listening on localhost:3000\n"; //TASK make 3000 dynamic
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 3000));
		int clientNum = 0;
		std::vector<std::thread>connectionHandlerThreads;
		for (;;) {	//TASK make it do an exponential retry loop
			tcp::socket sock(io_service);
			acceptor.accept(sock);
			std::cout << "Client " << ++clientNum << " connected " << sock.local_endpoint() << " " << sock.remote_endpoint() << "\n";
			connectionHandlerThreads.push_back(std::thread(handleConnection, std::move(sock)));
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}