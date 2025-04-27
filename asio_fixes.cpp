#include <iostream>
#include <fstream>
#include <string>
#include <regex>

std::string fix_asio_compatibility(const std::string& content) {
    std::string fixed = content;
    
    // Replace io_service with io_context throughout the files
    fixed = std::regex_replace(fixed, std::regex("io_service"), "io_context");
    
    // Fix expires_from_now to expires_after for Boost 1.70+
    fixed = std::regex_replace(fixed, std::regex("([a-zA-Z0-9_]+)->expires_from_now\\(\\)"), "$1->expires_after()");
    
    // Fix the strand instantiation
    fixed = std::regex_replace(fixed, 
        std::regex("m_strand\\.reset\\(new lib::asio::io_context::strand\\(\\*io_service\\)\\);"), 
        "m_strand.reset(new lib::asio::strand<lib::asio::io_context::executor_type>(io_service->get_executor()));");
    
    // Fix other strand usages
    fixed = std::regex_replace(fixed, std::regex("io_context::strand"), "strand<lib::asio::io_context::executor_type>");
    
    // Fix basic_waitable_timer constructor with time_duration
    fixed = std::regex_replace(fixed,
        std::regex("new lib::asio::steady_timer\\(\\s*\\*m_io_service,\\s*lib::asio::milliseconds\\(duration\\)\\)"),
        "new lib::asio::steady_timer(*m_io_service)");
        
    // Add separate expires_after calls
    fixed = std::regex_replace(fixed,
        std::regex("(lib::asio::steady_timer\\* new_timer = new lib::asio::steady_timer\\()\\*m_io_service\\)(;)"),
        "$1*m_io_service$2\n            new_timer->expires_after(lib::asio::milliseconds(duration))");
    
    return fixed;
}

bool process_file(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }
    
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();
    
    std::string fixed = fix_asio_compatibility(content);
    
    std::ofstream output(filename);
    if (!output) {
        std::cerr << "Could not write to file: " << filename << std::endl;
        return false;
    }
    
    output << fixed;
    output.close();
    
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_to_fix>" << std::endl;
        return 1;
    }
    
    if (process_file(argv[1])) {
        std::cout << "Successfully patched: " << argv[1] << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to patch: " << argv[1] << std::endl;
        return 1;
    }
}
