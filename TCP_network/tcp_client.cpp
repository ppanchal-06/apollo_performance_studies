#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <iomanip>

using boost::asio::ip::tcp;

struct SizeLabel {
    size_t bytes;
    std::string label;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./tcp_client <host> [-o output_file]" << std::endl;
        return 1;
    }

    std::string host = argv[1];
    std::string output_file = "throughput_results.csv";

    // Parse optional -o output argument
    for (int i = 2; i < argc - 1; ++i) {
        if (std::string(argv[i]) == "-o") {
            output_file = argv[i + 1];
            break;
        }
    }

    std::vector<SizeLabel> sizes;

    // for (int i = 1; i <= 100; ++i) { // 0.1MB to 10.0MB
    //     double size_mb = i * 0.1;
    //     size_t size_bytes = static_cast<size_t>(size_mb * 1024 * 1024);
    //     std::ostringstream label;
    //     label << std::fixed << std::setprecision(1) << size_mb << "MB";
    //     sizes.push_back({size_bytes, label.str()});
    // }
    
    for (int i = 1; i <= 2; ++i) { // 0.1MB to 10.0MB
        double size_mb = i * 0.1;
        size_t size_bytes = static_cast<size_t>(size_mb * 1024 * 1024);
        std::ostringstream label;
        label << std::fixed << std::setprecision(1) << size_mb << "MB";
        sizes.push_back({size_bytes, label.str()});
    }
    

    const int trials_per_size = 20;
    std::ofstream csv(output_file);
    csv << "Size,Trial,Duration,Bitrate_Mbps\n";

    try {
        for (const auto& size : sizes) {
            for (int trial = 1; trial <= trials_per_size; ++trial) {
                size_t total_bytes_to_send = size.bytes;
                std::vector<char> buffer(64 * 1024, 'x'); // 64KB chunk
                size_t bytes_sent = 0;

                boost::asio::io_service io_context;
                tcp::resolver resolver(io_context);
                tcp::resolver::query query(host, "2542");
                tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
                tcp::socket socket(io_context);
                boost::asio::connect(socket, endpoint_iterator);


                socket.set_option(boost::asio::ip::tcp::no_delay(true));

                auto start = std::chrono::high_resolution_clock::now();

                while (bytes_sent < total_bytes_to_send) {
                    size_t to_send = std::min(buffer.size(), total_bytes_to_send - bytes_sent);
                    boost::asio::write(socket, boost::asio::buffer(buffer.data(), to_send));
                    bytes_sent += to_send;
                }

                socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);

                char ack_buffer[2] = {};
                size_t ack_received = 0;
                while (ack_received < 2) {
                    ack_received += socket.read_some(boost::asio::buffer(ack_buffer + ack_received, 2 - ack_received));
                }

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                double seconds = duration.count();
                double bandwidth_mbps = (bytes_sent * 8.0 / 1e6) / seconds;

                std::cout << "Size " << size.label << ", Trial " << trial
                          << ": " << seconds << "s, " << bandwidth_mbps << " Mbps" << std::endl;

                csv << size.label << "," << trial << "," << std::fixed << std::setprecision(6)
                    << seconds << "," << bandwidth_mbps << "\n";

                socket.close();
            }
        }

        csv.close();
        std::cout << "\nAll results saved to " << output_file << "\n";
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

