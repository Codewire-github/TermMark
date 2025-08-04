#include "watch/watch.h"
#include "utils/TermStyle.h"
#include <unistd.h>
#include <sys/ioctl.h>

namespace fs = std::filesystem;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else 
    std::cout << "\033c" << std::flush; 
#endif
}

void printBoxText(const std::string& text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;

    std::cout << termstyle::YELLOW;
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << termstyle::RESET << "\n";

    std::cout << termstyle::BG_CYAN;
    for (int i = 0; i < width; ++i) std::cout << " ";
    std::cout << termstyle::RESET << "\n";

    std::cout << termstyle::BG_CYAN << termstyle::BLACK;
    int padding = width - (int)text.size();
    std::cout << text;
    if (padding > 0) std::cout << std::string(padding, ' ');
    std::cout << termstyle::RESET << "\n";

    std::cout << termstyle::BG_CYAN;
    for (int i = 0; i < width; ++i) std::cout << " ";
    std::cout << termstyle::RESET << "\n";

    std::cout << termstyle::YELLOW;
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << termstyle::RESET << "\n";
}


void watchMarkdownFile(const std::string& path, int intervalMs) {
    fs::path filePath(path);

    if (!fs::exists(filePath)) {
        std::cerr << "Error: File does not exist: " << path << "\n";
        return;
    }

    std::error_code ec;
    auto lastTime = fs::last_write_time(filePath, ec);
    if (ec) {
        std::cerr << "Error reading file timestamp: " << ec.message() << "\n";
        return;
    }

    std::cout << "Watching file for changes: " << path << "\n\n";

    // Initial render
    clearScreen();
    std::cout << "Rendering: " << path << "\n\n";
    try {
        termmark::renderFile(filePath.string());
    } catch (const std::exception& e) {
        std::cerr << "Initial render error: " << e.what() << std::endl;
    }

    // Continue watching for changes
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));

        auto currentTime = fs::last_write_time(filePath, ec);
        if (ec) continue;

        if (currentTime != lastTime) {
            lastTime = currentTime;

            clearScreen();
            printBoxText("Re-Rendering: " + path);
           
            try {
                termmark::renderFile(filePath.string());
            } catch (const std::exception& e) {
                std::cerr << "Render error: " << e.what() << std::endl;
            }
        }
    }
}
