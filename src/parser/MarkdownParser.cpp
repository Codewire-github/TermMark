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
            tokens.push_back(InlineToken{InlineType::BoldItalic, content});
        }
        else if (text.substr(pos, 2) == "**") {
            pos += 2;
            std::string content = parseUntil(text, pos, "**");
            pos += 2;
            tokens.push_back(InlineToken{InlineType::Bold, content});
        }
        else if (text[pos] == '*') {
            ++pos;
            std::string content = parseUntil(text, pos, "*");
            ++pos;
            tokens.push_back(InlineToken{InlineType::Italic, content});
        }
          else if (text[pos] == '_') {
            ++pos;
            std::string content = parseUntil(text, pos, "_");
            ++pos;
            tokens.push_back(InlineToken{InlineType::Italic, content});
        }
        else if (text[pos] == '`') {
            ++pos;
            std::string content = parseUntil(text, pos, "`");
            ++pos;
            tokens.push_back(InlineToken{InlineType::Code, content});
        }
        else if (text[pos] == '[') {
            size_t endBracket = text.find(']', pos);
            if (endBracket != std::string::npos && endBracket > pos) {
                if (endBracket + 1 < text.size() && text[endBracket + 1] == '(') {
                    size_t startParen = endBracket + 1;
                    size_t endParen = text.find(')', startParen);
                    if (endParen != std::string::npos) {
                        std::string displayText = text.substr(pos + 1, endBracket - pos - 1);
                        std::string url = text.substr(startParen + 1, endParen - startParen - 1);

                        tokens.push_back(InlineToken{ InlineType::HyperLink, displayText, url });
                        pos = endParen + 1;
                        continue;
                    }
                }
            }
            // If we reach here, it's not a valid hyperlink, so consume '[' as plain text:
            tokens.push_back(InlineToken{ InlineType::Plain, "[" });
            ++pos;  // advance position to avoid infinite loop
            continue;
        }

        else {
            std::string plain;
            while (pos < text.size() && text[pos] != '*' && text[pos] != '`' && text[pos] != '[') {
                plain += text[pos++];
            }
            if (!plain.empty()) {
                tokens.push_back(InlineToken{InlineType::Plain, plain});
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
    std::regex bulletRegex(R"(^\s*[-*+]\s+(.*))");
    std::regex numberedRegex(R"(^\s*\d+\.\s+(.*))");

    std::smatch match;
    bool inCodeBlock = false;
    std::string codeLang;
    std::string codeBuffer;

    bool inList = false;
    ListType currentListType = ListType::Unordered;
    std::vector<std::vector<InlineToken>> currentListItems;

    while (std::getline(stream, line)) {
        if (line.empty()) {
            if (inList) {
                tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
                currentListItems.clear();
                inList = false;
            }
            continue;
        }

        if (line.rfind("```", 0) == 0) {
            if (!inCodeBlock) {
                inCodeBlock = true;
                codeLang = line.substr(3);
                codeBuffer.clear();
            } else {
                inCodeBlock = false;
                tokens.push_back(Token{TokenType::CodeBlock, CodeBlockToken{codeLang, codeBuffer}});
            }
            continue;
        }

        if (inCodeBlock) {
            codeBuffer += line + "\n";
            continue;
        }

        if (std::regex_match(line, match, headingRegex)) {
            if (inList) {
                tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
                currentListItems.clear();
                inList = false;
            }
            int level = static_cast<int>(match[1].str().size());
            tokens.push_back(Token{ TokenType::Heading, HeadingToken{level, parseInline(match[2].str())} });
            continue;
        }

        if (std::regex_match(line, match, quoteRegex)) {
            if (inList) {
                tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
                currentListItems.clear();
                inList = false;
            }
            tokens.push_back(Token{ TokenType::Quote, QuoteToken{parseInline(match[1].str())} });
            continue;
        }

        // Check if bullet list
        if (std::regex_match(line, match, bulletRegex)) {
            if (!inList) {
                inList = true;
                currentListType = ListType::Unordered;
            } else if (currentListType != ListType::Unordered) {
                // Flush previous list if switching types
                tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
                currentListItems.clear();
                currentListType = ListType::Unordered;
            }
            currentListItems.push_back(parseInline(match[1].str()));
            continue;
        }

        // Check if numbered list
        if (std::regex_match(line, match, numberedRegex)) {
            if (!inList) {
                inList = true;
                currentListType = ListType::Ordered;
            } else if (currentListType != ListType::Ordered) {
                tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
                currentListItems.clear();
                currentListType = ListType::Ordered;
            }
            currentListItems.push_back(parseInline(match[1].str()));
            continue;
        }

        // Normal paragraph or other
        if (inList) {
            tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
            currentListItems.clear();
            inList = false;
        }
        tokens.push_back(Token{ TokenType::Paragraph, ParagraphToken{parseInline(line)} });
    }

    // Flush any remaining list at EOF
    if (inList) {
        tokens.push_back(Token{ TokenType::List, ListToken{currentListType, currentListItems} });
    }

    return tokens;
}
