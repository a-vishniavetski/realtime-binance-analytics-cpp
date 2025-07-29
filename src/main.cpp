// main.cpp - fixed version with TLS support
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

#include <vector>
#include <deque>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "analysis/Analyzer.h"

using json = nlohmann::json;

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

class MarketDataFeed {
private:
    client m_client;
    websocketpp::connection_hdl m_hdl;

public:
    void connect() {
        try {
            // Set logging
            m_client.set_access_channels(websocketpp::log::alevel::all);
            m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

            // Initialize ASIO
            m_client.init_asio();

            // Set TLS init handler for WSS
            m_client.set_tls_init_handler([](websocketpp::connection_hdl) {
                return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
            });

            // Set handlers
            m_client.set_message_handler([this](websocketpp::connection_hdl hdl, client::message_ptr msg) {
                handle_message(msg->get_payload());
            });

            m_client.set_open_handler([this](websocketpp::connection_hdl hdl) {
                std::cout << "Connection opened!" << '\n';
                m_hdl = hdl;
            });

            m_client.set_fail_handler([this](websocketpp::connection_hdl hdl) {
                std::cout << "Connection failed!" << '\n';
            });

            m_client.set_close_handler([this](websocketpp::connection_hdl hdl) {
                std::cout << "Connection closed!" << '\n';
            });

            // Create connection
            websocketpp::lib::error_code ec;
            std::string connection_url = "wss://stream.binance.com:9443/ws/btcusdt@trade";
            auto con = m_client.get_connection("wss://stream.binance.com:9443/ws/btcusdt@trade", ec);

            if (ec) {
                std::cout << "Could not create connection: " << ec.message() << '\n';
                return;
            }

            if (!con) {
                std::cout << "Connection object is null!" << '\n';
                return;
            }

            // Connect
            m_client.connect(con);

            // Run in separate thread to avoid blocking
            std::thread client_thread([this]() {
                m_client.run();
            });

            // Keep main thread alive
            client_thread.join();

        } catch (websocketpp::exception const & e) {
            std::cout << "WebSocket++ exception: " << e.what() << '\n';
        } catch (std::exception const & e) {
            std::cout << "Standard exception: " << e.what() << '\n';
        }
    }

    std::function<void(double)> callback;

    void setCallback(std::function<void(double)> cb) {
        callback = cb;
    }
private:

    void handle_message(const std::string& payload) {
        auto data = json::parse(payload);

        if (callback) {
            double price = std::stod(data["p"].get<std::string>());
            callback(price);
        }
    }

};


int main() {
    MarketDataFeed feed;
    Analyzer analyzer;

    // Connect: feed -> analyzer -> your logic
    feed.setCallback([&analyzer](double price) {
        analyzer.process_price(price);

        if (analyzer.counter % 100 == 0) {
            analyzer.print_counter();
            analyzer.print_stats();
        }
    });

    feed.connect();
    return 0;
}