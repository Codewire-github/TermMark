#include "parser/MarkdownParser.h"
#include "renderer/Renderer.h"
#include "utils/FileIO.h"
#include <fstream>
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: markdown <file.md>\n";
        return 1;
    }

    try {
        std::string content = utils::readFile(argv[1]);
        auto tokens = parseMarkdown(content);
        renderTokens(tokens);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
