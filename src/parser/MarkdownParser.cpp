#include "parser/MarkdownParser.h"
#include <sstream>
#include <regex>
#include <iostream>

// Recursive inline parser helper
std::vector<InlineToken> parseInline(const std::string& text, size_t& pos);

std::string parseUntil(const std::string& text, size_t& pos, const std::string& delimiter) {
    std::string result;
    while (pos < text.size() && text.substr(pos, delimiter.size()) != delimiter) {
        result += text[pos++];
    }
    return result;
}

std::vector<InlineToken> parseInline(const std::string& text, size_t& pos) {
    std::vector<InlineToken> tokens;

    while (pos < text.size()) {
        if (text.substr(pos, 3) == "***") {
            pos += 3;
            std::string content = parseUntil(text, pos, "***");
            pos += 3;
            tokens.push_back({InlineType::BoldItalic, content});
        }
        else if (text.substr(pos, 2) == "**") {
            pos += 2;
            std::string content = parseUntil(text, pos, "**");
            pos += 2;
            tokens.push_back({InlineType::Bold, content});
        }
        else if (text[pos] == '*') {
            ++pos;
            std::string content = parseUntil(text, pos, "*");
            ++pos;
            tokens.push_back({InlineType::Italic, content});
        }
        else if (text[pos] == '`') {
            ++pos;
            std::string content = parseUntil(text, pos, "`");
            ++pos;
            tokens.push_back({InlineType::Code, content});
        }
        else if (text[pos] == '>') {
            
        }
        else {
            std::string plain;
            while (pos < text.size() && text[pos] != '*' && text[pos] != '`') {
                plain += text[pos++];
            }
            if (!plain.empty()) {
                tokens.push_back({InlineType::Plain, plain});
            }
        }
    }

    return tokens;
}

std::vector<InlineToken> parseInline(const std::string& text) {
    size_t pos = 0;
    return parseInline(text, pos);
}

void parseCodeBlock(const std::string& text) {
    size_t pos = 0;
    pos += 3;
    std::string lang = parseUntil(text, pos, "\n");
    std::cout << "Lang==" << lang << "\n";
    std::cout << "[CodeBlock]" << text << "\n";
}

std::vector<Token> parseMarkdown(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream stream(input);
    std::string line;
    std::regex headingRegex(R"(^(\#{1,6})\s+(.*))");
    std::regex quoteRegex(R"(^\s*> ?(.*))");
    std::smatch match;
    bool inCodeBlock = false;
    std::string codeLang;
    std::string codeBuffer;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;
         if (line.rfind("```", 0) == 0) {
            if (!inCodeBlock) {
                // Starting code block
                inCodeBlock = true;
                codeLang = line.substr(3); // Could be empty
                codeBuffer.clear();
            } else {
                // Ending code block
                inCodeBlock = false;
                tokens.push_back({
                    TokenType::CodeBlock,
                    {},  // Inline content
                    {codeLang, codeBuffer}, // Code content
                });
            }
            continue;
        }

        if (inCodeBlock) {
            codeBuffer += line + "\n";
            continue;
        }

        else if (std::regex_match(line, match, headingRegex)) {
            int level = static_cast<int>(match[1].str().size());
            std::string content = match[2];
            tokens.push_back({
                TokenType::Heading,
                parseInline(content),{},
                {},
                level
            });
        } 
        else if (std::regex_match(line, match, quoteRegex)) {
            tokens.push_back({ TokenType::Quote, parseInline(match[1].str()) });
        }
        else {
            tokens.push_back({
                TokenType::Paragraph,
                parseInline(line)
            });
        }
    }

    return tokens;
}
