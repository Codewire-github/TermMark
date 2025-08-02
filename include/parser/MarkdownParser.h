#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Heading,
    Paragraph,
    Quote,
    List,
    CodeBlock,

};

enum class InlineType {
    Plain,
    Bold,
    Italic,
    BoldItalic,
    Code
};

struct InlineToken {
    InlineType type;
    std::string text;
};

struct CodeBlockToken {
    std::string lang;
    std::string code;
};

struct Token {
    TokenType type;
    std::vector<InlineToken> content;
    CodeBlockToken codeContent;
    std::vector<std::vector<InlineToken>> listItems = {};
    int headingLevel = 0; // For headings only
};

std::vector<Token> parseMarkdown(const std::string& input);
std::vector<InlineToken> parseInline(const std::string& text);
