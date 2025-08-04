#pragma once

#include <string>

namespace termmark {
    // Rendering a markdown string content to the terminal
    void renderMarkdown(const std::string& markdown);

    // Rendering a markdown from a file
    void renderFile(const std::string& filepath);
}