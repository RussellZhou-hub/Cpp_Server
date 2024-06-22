#pragma once
#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

using asio::ip::tcp;

const std::string RESPONSE =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 28\r\n"
    "\r\n"
    "{\"message\": \"Hello, World!\"}";

void handle_request(tcp::socket& socket) {
    try {
        // Buffer to store the request
        std::array<char, 1024> buffer;
        asio::error_code error;

        // Read the request
        size_t length = socket.read_some(asio::buffer(buffer), error);

        if (error == asio::error::eof) {
            // Connection closed cleanly by peer
            return;
        } else if (error) {
            throw asio::system_error(error);
        }

        std::string request(buffer.data(), length);
        std::cout << "Received request:\n" << request << std::endl;

        // Write the response
        asio::write(socket, asio::buffer(RESPONSE), error);
        if (error) {
            throw asio::system_error(error);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in handle_request: " << e.what() << std::endl;
    }
}