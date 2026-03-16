#include "asio/io_context.hpp"
#include <asio.hpp>
#include <deque>
#include <memory>

#include <iostream>

std::string client_read_buffer;
std::deque<std::string> client_write_queue;

void handle_client_read(std::shared_ptr<asio::ip::tcp::socket> socket) {
	asio::async_read_until(*socket, asio::dynamic_buffer(client_read_buffer), '\n',
		[socket](std::error_code ec, std::size_t length) {
			if(ec) {
				socket->close();
				return;
			}

			auto msg = client_read_buffer.substr(0, length);
			client_read_buffer.erase(0, length);

			std::cout << msg << std::flush;
			handle_client_read(socket);
		});
}

void handle_client_write(std::shared_ptr<asio::ip::tcp::socket> socket) {
	if (client_write_queue.empty()) return;

	asio::async_write(*socket, asio::buffer(client_write_queue.front()),
		[socket](std::error_code ec, std::size_t) {
			if(ec) {
				socket->close();
				return;
			}
			
			client_write_queue.pop_front();
			if (!client_write_queue.empty()) 
				handle_client_write(socket);
		});
}

inline void client_main(std::string_view host) {
    asio::io_context ctx;
    asio::ip::tcp::resolver resolver(ctx);
    // facebook.com -> 157.240.23.45
    auto endpoints = resolver.resolve(host, "9999");

    auto socket = std::make_shared<asio::ip::tcp::socket>(ctx);
    auto local_endpoint = asio::connect(*socket, endpoints);

    handle_client_read(socket);

    std::thread t([&ctx] { ctx.run(); });

    while(true) {
        // Homework get some input from the user here!
        std::string message = "hi\n";
        bool write_in_progress = !client_write_queue.empty();
        client_write_queue.push_back(message);
        if(!write_in_progress)
            asio::post(ctx, [socket]() { handle_client_write(socket); });
    }
    
    t.join();
}