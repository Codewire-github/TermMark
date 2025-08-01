#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Heading,
    Paragraph,
};

enum class InlineType {
    Plain,
    Bold,
    Italic
};

struct InlineToken {
    InlineType type;
    std::string text;
};

struct Token {
    TokenType type;
    std::vector<InlineToken> content;
    int headingLevel = 0; // For headings only
};

std::vector<Token> parseMarkdown(const std::string& input);
std::vector<InlineToken> parseInline(const std::string& text);
