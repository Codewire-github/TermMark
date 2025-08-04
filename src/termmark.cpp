#include "termmark.h"
#include "parser/MarkdownParser.h"
#include "renderer/Renderer.h"
#include "utils/FileIO.h"
#include <fstream>
#include <iostream>

namespace termmark {
    void renderMarkdown(const std::string& markdown) {
        auto tokens = parseMarkdown(markdown);
        renderTokens(tokens);
    }

    void renderFile(const std::string& filepath) {
        try {
        std::string content = utils::readFile(filepath);
        auto tokens = parseMarkdown(content);
        renderTokens(tokens);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    }
}