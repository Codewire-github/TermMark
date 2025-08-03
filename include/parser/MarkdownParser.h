#pragma once
#include <string>
#include <vector>
#include <variant>

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
    Code,
    HyperLink,
};

enum class ListType {
    Unordered,
    Ordered
};

struct InlineToken {
    InlineType type;
    std::string text;
    std::string url;
    std::vector<InlineToken> children;
};

struct HeadingToken {
    int level;
    std::vector<InlineToken> content;
};

struct ParagraphToken {
    std::vector<InlineToken> content;
};

struct QuoteToken {
    std::vector<InlineToken> content;
};

struct ListToken {
    ListType type;
    std::vector<std::vector<InlineToken>> items;
};

struct CodeBlockToken {
    std::string lang;
    std::string code;
};

using TokenData = std::variant<HeadingToken, ParagraphToken, QuoteToken, ListToken, CodeBlockToken>;

struct Token {
    TokenType type;
    TokenData data;
};

std::vector<Token> parseMarkdown(const std::string& input);
std::vector<InlineToken> parseInline(const std::string& text);
