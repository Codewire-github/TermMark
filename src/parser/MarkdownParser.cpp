#include "parser/MarkdownParser.h"
#include <sstream>
#include <regex>

std::vector<InlineToken> parseInline(const std::string& text) {
    std::vector<InlineToken> tokens;
    std::regex boldRe(R"(\*\*(.+?)\*\*)");
    std::regex italicRe(R"(\*(.+?)\*)");

    std::string remaining = text;
    std::smatch match;

    while (!remaining.empty()) {
        if (std::regex_search(remaining, match, boldRe)) {
            if (match.position() > 0) {
                tokens.push_back({InlineType::Plain, remaining.substr(0, match.position())});
            }
            tokens.push_back({InlineType::Bold, match[1]});
            remaining = match.suffix();
        } else if (std::regex_search(remaining, match, italicRe)) {
            if (match.position() > 0) {
                tokens.push_back({InlineType::Plain, remaining.substr(0, match.position())});
            }
            tokens.push_back({InlineType::Italic, match[1]});
            remaining = match.suffix();
        } else {
            tokens.push_back({InlineType::Plain, remaining});
            break;
        }
    }

    return tokens;
}

std::vector<Token> parseMarkdown(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream stream(input);
    std::string line;
    std::regex headingRegex(R"(^(\#{1,6})\s+(.*))");
    std::smatch match;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        if (std::regex_match(line, match, headingRegex)) {
            int level = match[1].str().size();
            tokens.push_back({TokenType::Heading, parseInline(match[2]), level});
        } else {
            tokens.push_back({TokenType::Paragraph, parseInline(line)});
        }
    }

    return tokens;
}
