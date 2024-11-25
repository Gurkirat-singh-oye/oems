#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

int main()
{
    try
    {
        // The io_context is required for all I/O
        net::io_context ioc;
        ssl::context ctx(ssl::context::tls_client);

        ctx.set_default_verify_paths();

        // These will hold the resolved IP and port
        std::string host = "test.deribit.com"; // WebSocket testing server
        std::string port = "443";

        
        // SSL and WebSocket stream setup
        websocket::stream<beast::ssl_stream<tcp::socket>> ws(ioc, ctx);

        SSL_set_tlsext_host_name(ws.next_layer().native_handle(),
                                 host.c_str());

        // Resolver for converting hostname to IP address
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(host, port);


        // Create a WebSocket stream connected to the given endpoint
        net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

        // Perform SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // Perform WebSocket handshake
        // ws.set_option(websocket::stream_base::decorator(
        //     [](websocket::request_type& req) {
        //         req.set(beast::http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-ssl");
        //     }));
        ws.handshake(host, "/ws/api/v2");

        // Send a message
        std::string message = "Hello, WebSocket!";
        ws.write(net::buffer(message));
        std::cout << "Sent message: " << message << std::endl;

        // Buffer for incoming message
        beast::flat_buffer buffer;

        // Receive a message
        ws.read(buffer);
        std::cout << "Received message: " << beast::make_printable(buffer.data()) << std::endl;

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
