// main.cpp - fixed version with TLS support
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

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
                std::cout << "Connection opened!" << std::endl;
                m_hdl = hdl;
            });

            m_client.set_fail_handler([this](websocketpp::connection_hdl hdl) {
                std::cout << "Connection failed!" << std::endl;
            });

            m_client.set_close_handler([this](websocketpp::connection_hdl hdl) {
                std::cout << "Connection closed!" << std::endl;
            });

            // Create connection
            websocketpp::lib::error_code ec;
            auto con = m_client.get_connection("wss://stream.binance.com:9443/ws/btcusdt@ticker", ec);

            if (ec) {
                std::cout << "Could not create connection: " << ec.message() << std::endl;
                return;
            }

            if (!con) {
                std::cout << "Connection object is null!" << std::endl;
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
            std::cout << "WebSocket++ exception: " << e.what() << std::endl;
        } catch (std::exception const & e) {
            std::cout << "Standard exception: " << e.what() << std::endl;
        }
    }

private:
    void handle_message(const std::string& payload) {
        auto data = json::parse(payload);
        std::cout << "BTC / USDT last price: " << data["c"] << std::endl;
    }
};

int main() {
    MarketDataFeed feed;
    feed.connect();
    return 0;
}