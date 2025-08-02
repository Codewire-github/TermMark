#include "utils/TermStyle.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace utils {
    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}

