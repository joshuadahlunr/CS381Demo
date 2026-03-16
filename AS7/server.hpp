#include "asio/buffer.hpp"
#include "asio/io_context.hpp"
#include "asio/read_until.hpp"
#include "asio/write.hpp"
#include <asio.hpp>
#include <deque>
#include <memory>
#include <system_error>
#include <iostream>
#include <set>

struct ClientConnection {
    std::shared_ptr<asio::ip::tcp::socket> connection;
    std::string read_buffer;
    std::deque<std::string> write_queue;
};

std::set<std::shared_ptr<ClientConnection>> clients;
std::deque<std::string> message_history;

void handle_client_write(std::shared_ptr<ClientConnection> client) {
    if(client->write_queue.empty()) return;

    asio::async_write(*client->connection, asio::buffer(client->write_queue.front()),
    [client](std::error_code error, size_t bytes_written){
        if(error) {
            clients.erase(client);
            return;
        }

        client->write_queue.pop_front();
        handle_client_write(client);
    });
}

void broadcast_message(const std::string& msg) {
    message_history.push_back(msg);

    for(auto& client: clients) {
        bool write_in_progress = !client->write_queue.empty();
        client->write_queue.push_back(msg);
        if(!write_in_progress)
            handle_client_write(client);
    }
}

void handle_client_read(std::shared_ptr<ClientConnection> client) {
    asio::async_read_until(*client->connection, asio::dynamic_buffer(client->read_buffer), '\n',
    [client](std::error_code error, size_t bytes_read){
        if(error) {
            clients.erase(client);
            return;
        }

        std::string msg = client->read_buffer.substr(0, bytes_read);
        client->read_buffer.erase(0, bytes_read);
        
        // Sends the message back out to every client
        broadcast_message(msg);
        handle_client_read(client);
    });
}

void accept_connections(asio::ip::tcp::acceptor& acceptor) {
    acceptor.async_accept([&acceptor](std::error_code error, asio::ip::tcp::socket socket){
        accept_connections(acceptor);

        if(error) return;

        std::cout << "New client connected from: " << socket.remote_endpoint() << std::endl;
        auto client = std::make_shared<ClientConnection>();
        client->connection = std::make_shared<asio::ip::tcp::socket>(std::move(socket));

        clients.insert(client);

        for(auto& msg: message_history)
            client->write_queue.push_back(msg);

        if(!client->write_queue.empty())
            handle_client_write(client);

        handle_client_read(client);
    });
}

inline void server_main() {
    asio::io_context ctx;
    asio::ip::tcp::acceptor acceptor(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), 9999));

    accept_connections(acceptor);
    ctx.run();
}