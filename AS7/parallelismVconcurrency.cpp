
#include "asio/awaitable.hpp"
#include "asio/io_context.hpp"
#include "asio/steady_timer.hpp"
#include "asio/this_coro.hpp"
#include "asio/use_awaitable.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include <asio.hpp>

asio::awaitable<void> task(int& i) {
    auto executor = co_await asio::this_coro::executor;
    asio::steady_timer timer(executor);
    timer.expires_after(std::chrono::milliseconds(17));

    co_await timer.async_wait(asio::use_awaitable);
    
    i += 1;
}

int main() {

    // srand(time(nullptr));

    int i = 0;

    // std::vector<std::thread> threads;

    // for(size_t j = 0; j < 24; ++j) 
    //     threads.emplace_back([] (int& i) {
    //         auto tmp = i;
    //         std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 28));
    //         tmp = tmp + 1;
    //         i = tmp;
    //     }, std::ref(i));

    // for(auto& t: threads)
    //     t.join();

    asio::io_context io;

    for(size_t j = 0; j < 7; ++j)
        asio::co_spawn(io, task(i), asio::detached);

    io.run();

    std::cout << i << std::endl;
}