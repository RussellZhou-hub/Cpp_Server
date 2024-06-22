#pragma once
#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

using asio::ip::tcp;

// const std::string RESPONSE =
//     "HTTP/1.1 200 OK\r\n"
//     "Content-Type: application/json\r\n"
//     "Content-Length: 28\r\n"
//     "\r\n"
//     "{\"message\": \"Hello, World!\"}";

std::unordered_map<std::string, std::string> parse_query_params(const std::string& query) 
{
    std::unordered_map<std::string, std::string> params;
    size_t start = 0;
    while (start < query.length()) {
        size_t end = query.find('&', start);
        if (end == std::string::npos) {
            end = query.find('\t', start);
        }
        if (end == std::string::npos) {
            end = query.length();
        }
        size_t eq_pos = query.find('=', start);
        if (eq_pos != std::string::npos && eq_pos < end) {
            std::string key = query.substr(start, eq_pos - start);
            std::string value = query.substr(eq_pos + 1, end - eq_pos - 1);
            params[key] = value;
        }
        start = end + 1;
    }
    return params;
}

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

        // Parse the request line
        size_t method_end = request.find(' ');
        size_t path_end = request.find(' ', method_end + 1);
        std::string method = request.substr(0, method_end);
        std::string path = request.substr(method_end + 1, path_end - method_end - 1);

        std::cout << "\n Received path:\n" << path << std::endl;


        // Parse query parameters
        size_t query_start = path.find('?');
        std::unordered_map<std::string, std::string> query_params;
        if (query_start != std::string::npos) {
            std::string query = path.substr(query_start + 1);
            path = path.substr(0, query_start);
            std::cout << "\n Received path:\n" << query << std::endl;
            query_params = parse_query_params(query);
        }

        // Generate JSON response
        std::string response_content = "{";
        for (const auto& param : query_params) {
            response_content += "\"" + param.first + "\": \"" + param.second + "\", ";
        }
        if (!query_params.empty()) {
            response_content.pop_back();
            response_content.pop_back();
        }
        response_content += "}";

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(response_content.length()) + "\r\n"
            "\r\n" +
            response_content;

        std::cout << "\n response:\n" << response << std::endl;

        // Write the response
        asio::write(socket, asio::buffer(response), error);
        if (error) {
            throw asio::system_error(error);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in handle_request: " << e.what() << std::endl;
    }
}



class AsyncHttpServer {
public:
    AsyncHttpServer(asio::io_context& io_context, short port)
        : acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
          socket_(io_context),port_(port) {
        StartAccept();
    }

    int getPort()
    {
        return port_;
    }

private:
    void StartAccept() {
        acceptor_.async_accept(socket_,
            [this](std::error_code ec) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket_))->Start();
                }
                StartAccept();
            });
    }

    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(asio::ip::tcp::socket socket)
            : socket_(std::move(socket)) {
        }

        void Start() {
            ReadRequest();
        }

    private:
        void ReadRequest() {
            auto self(shared_from_this());
            asio::async_read_until(socket_, request_, "\r\n\r\n",
                [this, self](std::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) {
                        std::istream request_stream(&request_);
                        std::string header;
                        while (std::getline(request_stream, header) && header != "\r") {
                            //std::cout << "\n header:\n" << header << std::endl;
                            // Process headers if needed
                            ProcessHeader(header);
                        }
                        ProcessRequest();
                    }
                });
        }

        void ProcessHeader(std::string& header)
        {
            try {
                if(header.substr(0,3)!="GET") return;
                std::string request(header);
                std::cout << "Received header:\n" << header << std::endl;

                // get the params line that contain username and password
                size_t method_end = request.find(' ');
                size_t path_end = request.find(' ', method_end + 1);
                std::string method = request.substr(0, method_end);
                std::string path = request.substr(method_end + 1, path_end - method_end - 1);

                //std::cout << "\n params line:\n" << path << std::endl;


                // Parse query parameters
                size_t query_start = path.find('?');
                std::unordered_map<std::string, std::string> query_params;
                if (query_start != std::string::npos) {
                    std::string query = path.substr(query_start + 1);
                    //std::cout << "\n Received parms:\n" << query << std::endl;
                    query_params = parse_query_params(query);
                }

                // Generate JSON response
                std::string response_content = "{";
                for (const auto& param : query_params) {
                    response_content += "\"" + param.first + "\": \"" + param.second + "\", ";
                }

                // delete extra ", "
                if (!query_params.empty()) {
                    response_content.pop_back();
                    response_content.pop_back();
                }
                response_content += "}";

                std::string response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: " + std::to_string(response_content.length()) + "\r\n"
                    "\r\n" +
                    response_content;

                std::cout << "\n response:\n" << response << std::endl;

                WriteResponse(response);
            } catch (std::exception& e) {
                std::cerr << "Exception in ProcessHeader: " << e.what() << std::endl;
            }
        }

        void ProcessRequest() {
            std::string response_content = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
            WriteResponse(response_content);
        }

        void WriteResponse(const std::string& response_content) {
            auto self(shared_from_this());
            asio::async_write(socket_, asio::buffer(response_content),
                [this, self](std::error_code ec, std::size_t /*bytes_transferred*/) {
                    if (!ec) {
                        std::cout << "Response sent" << std::endl;
                    }
                });
        }

        asio::ip::tcp::socket socket_;
        asio::streambuf request_;
    };

    int port_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket socket_;
};