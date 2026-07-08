#include <boost/asio.hpp>
#include <iostream>
#include <vector>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_service io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 5201));

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Client connected: " << socket.remote_endpoint() << std::endl;

            std::vector<char> buffer(64 * 1024);
            std::size_t total_bytes = 0;
            boost::system::error_code error;

            // Read until EOF (client shutdown send)
            while (true) {
                size_t len = socket.read_some(boost::asio::buffer(buffer), error);
                if (error == boost::asio::error::eof)
                    break;
                else if (error)
                    throw boost::system::system_error(error);

                total_bytes += len;
            }
            boost::asio::write(socket, boost::asio::buffer("OK", 2)); //send ack back
            std::cout << "Received " << total_bytes / 1e6 << " MB in total.\n";           
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    return 0;
}
