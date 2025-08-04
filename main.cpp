#include "utils/guide.h"
#include "utils/cli_info.h"
#include "termmark.h"
#include "watch/watch.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: termmark <file.md> [--watch]\n";
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "--guide") {
        printMarkdownGuide();
        return 0;

    } else if (arg == "--version") {
        std::cout << cli::getVersionString() << "\n";
        return 0;

    } else if (arg == "--help") {
        std::string content = cli::getHelpMarkdown();
        termmark::renderMarkdown(content);
        return 0;

    } else if (arg == "--watch") {
        if (argc < 3) {
            std::cerr << "Usage: termmark --watch <file.md>\n";
            return 1;
        }
        std::string filepath = argv[2];
        watchMarkdownFile(filepath, 1000);
        return 0;
    }

    try {
        termmark::renderFile(arg);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
