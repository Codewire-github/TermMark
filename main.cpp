#include "parser/MarkdownParser.h"
#include "renderer/Renderer.h"
#include "utils/FileIO.h"
#include "utils/guide.h"
#include "utils/cli_info.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: termmark <file.md>\n";
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
        auto tokens = parseMarkdown(content);
        renderTokens(tokens);
        return 0;
    }

    try {
        std::string content = utils::readFile(arg);
        auto tokens = parseMarkdown(content);
        renderTokens(tokens);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
