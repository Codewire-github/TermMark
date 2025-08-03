#include "parser/MarkdownParser.h"
#include <sstream>
#include <regex>
#include <iostream>
#include <algorithm>
#include <cctype>

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
            if (pos + 3 <= text.size()) {
                pos += 3;
                tokens.push_back(InlineToken{InlineType::BoldItalic, content});
            } else {
                tokens.push_back(InlineToken{InlineType::Plain, "***" + content});
            }
        }
        else if (text.substr(pos, 2) == "**") {
            pos += 2;
            std::string content = parseUntil(text, pos, "**");
            if (pos + 2 <= text.size()) {
                pos += 2;
                tokens.push_back(InlineToken{InlineType::Bold, content});
            } else {
                tokens.push_back(InlineToken{InlineType::Plain, "**" + content});
            }
        }
        else if (text.substr(pos, 2) == "__") {
            pos += 2;
            std::string content = parseUntil(text, pos, "__");
            if (pos + 2 <= text.size()) {
                pos += 2;
                tokens.push_back(InlineToken{InlineType::Bold, content});
            } else {
                tokens.push_back(InlineToken{InlineType::Plain, "__" + content});
            }
        }
        else if (text[pos] == '*') {
            ++pos;
            std::string content = parseUntil(text, pos, "*");
            if (pos + 1 <= text.size()) {
                ++pos;
                tokens.push_back(InlineToken{InlineType::Italic, content});
            } else {
                tokens.push_back(InlineToken{InlineType::Plain, "*" + content});
            }
        }
        else if (text[pos] == '_') {
            ++pos;
            std::string content = parseUntil(text, pos, "_");
            if (pos + 1 <= text.size()) {
                ++pos;
                tokens.push_back(InlineToken{InlineType::Italic, content});
            } else {
                tokens.push_back(InlineToken{InlineType::Plain, "_" + content});
            }
        }
        else if (text[pos] == '`') {
            ++pos;
            std::string content = parseUntil(text, pos, "`");
            if (pos + 1 <= text.size()) {
                ++pos;
                tokens.push_back(InlineToken{InlineType::Code, content});
            } else {
                tokens.push_back(InlineToken{InlineType::Plain, "`" + content});
            }
        }
        else if (text[pos] == '[') {
            size_t endBracket = text.find(']', pos);
            if (endBracket != std::string::npos && endBracket > pos) {
                if (endBracket + 1 < text.size() && text[endBracket + 1] == '(') {
                    size_t startParen = text.find('(', endBracket + 1);
                    size_t endParen = text.find(')', startParen);
                    if (startParen != std::string::npos && endParen != std::string::npos) {
                        std::string displayText = text.substr(pos + 1, endBracket - pos - 1);
                        std::string url = text.substr(startParen + 1, endParen - startParen - 1);
                        tokens.push_back(InlineToken{ InlineType::HyperLink, displayText, url });
                        pos = endParen + 1;
                        continue;
                    }
                }
            }
            tokens.push_back(InlineToken{ InlineType::Plain, "[" });
            ++pos;
            continue;
        }
        else {
            std::string plain;
            while (pos < text.size() &&
                   text.substr(pos, 3) != "***" &&
                   text.substr(pos, 2) != "**" &&
                   text.substr(pos, 2) != "__" &&
                   text[pos] != '*' && text[pos] != '`' && text[pos] != '[' && text[pos] != '_') {
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

std::vector<Token> parseMarkdown(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream stream(input);
    std::string line;

    std::regex headingRegex(R"(^(\#{1,6})\s+(.*))");
    std::regex quoteRegex(R"(^\s*> ?(.*))");
    std::regex bulletRegex(R"(^\s*[-*+]\s+(.*))");
    std::regex numberedRegex(R"(^\s*\d+\.\s+(.*))");
    std::regex tableHeaderRegex(R"(^\|?(.+)\|.*$)");
    std::regex tableSeparatorRegex(R"(^\|?(\s*:-+\s*|\s*-+:\s*|\s*:-+:\s*|\s*-+\s*\|)+(\s*:-+\s*|\s*-+:\s*|\s*:-+:\s*|\s*-+\s*)?\|?\s*$)");
    
    std::smatch match;
    bool inCodeBlock = false;
    std::string codeLang;
    std::string codeBuffer;
    bool inList = false;
    ListType currentListType = ListType::Unordered;
    bool inTable = false;
    
    while (std::getline(stream, line)) {
        if (line.empty()) {
            inList = false;
            inTable = false;
            continue;
        }
        
        if (line.rfind("```", 0) == 0) {
            inList = false;
            inTable = false;
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
        
        if (!inTable && !inList && !inCodeBlock) {
            if (std::regex_match(line, match, tableHeaderRegex)) {
                std::string nextLine;
                std::streampos currentPos = stream.tellg();
                if (std::getline(stream, nextLine) && std::regex_match(nextLine, tableSeparatorRegex)) {
                    inTable = true;
                    std::vector<std::vector<InlineToken>> tableHeaders;
                    std::string headerContent = match[1].str();
                    if (!headerContent.empty() && headerContent.front() == '|') headerContent.erase(0, 1);
                    if (!headerContent.empty() && headerContent.back() == '|') headerContent.pop_back();

                    std::istringstream headerStream(headerContent);
                    std::string headerCell;
                    while (std::getline(headerStream, headerCell, '|')) {
                        headerCell.erase(0, headerCell.find_first_not_of(" \t"));
                        headerCell.erase(headerCell.find_last_not_of(" \t") + 1);
                        tableHeaders.push_back(parseInline(headerCell));
                    }
                    tokens.push_back(Token{ TokenType::Table, TableToken{tableHeaders, {}} });
                    continue;
                } else {
                    stream.seekg(currentPos);
                }
            }
        }

        if (inTable) {
            std::vector<std::vector<InlineToken>> row;
            std::istringstream rowStream(line);
            std::string cell;
            while (std::getline(rowStream, cell, '|')) {
                cell.erase(0, cell.find_first_not_of(" \t"));
                cell.erase(cell.find_last_not_of(" \t") + 1);
                row.push_back(parseInline(cell));
            }
            if (!row.empty()) {
                if (line.front() == '|' && row.front().empty()) {
                    row.erase(row.begin());
                }
                auto& table_data = std::get<TableToken>(tokens.back().data);
                while(row.size() < table_data.headers.size()) {
                    row.push_back({});
                }
                 while(row.size() > table_data.headers.size()) {
                    row.pop_back();
                }
                table_data.rows.push_back(row);
            }
            continue;
        }

        if (std::regex_match(line, match, headingRegex)) {
            inList = false;
            inTable = false;
            int level = static_cast<int>(match[1].str().size());
            tokens.push_back(Token{ TokenType::Heading, HeadingToken{level, parseInline(match[2].str())} });
            continue;
        }

        if (std::regex_match(line, match, quoteRegex)) {
            inList = false;
            inTable = false;
            tokens.push_back(Token{ TokenType::Quote, QuoteToken{parseInline(match[1].str())} });
            continue;
        }
        
        if (std::regex_match(line, match, bulletRegex)) {
            inTable = false;
            if (!inList || currentListType != ListType::Unordered) {
                inList = true;
                currentListType = ListType::Unordered;
                tokens.push_back(Token{ TokenType::List, ListToken{ListType::Unordered, {}} });
            }
            auto& list_data = std::get<ListToken>(tokens.back().data);
            list_data.items.push_back(parseInline(match[1].str()));
            continue;
        }

        if (std::regex_match(line, match, numberedRegex)) {
            inTable = false;
            if (!inList || currentListType != ListType::Ordered) {
                inList = true;
                currentListType = ListType::Ordered;
                tokens.push_back(Token{ TokenType::List, ListToken{ListType::Ordered, {}} });
            }
            auto& list_data = std::get<ListToken>(tokens.back().data);
            list_data.items.push_back(parseInline(match[1].str()));
            continue;
        }
        
        if (line.rfind("---", 0) == 0) {
            inList = false;
            inTable = false;
            tokens.push_back(Token{ TokenType::HorizontalRule, HorizontalRuleToken{} });
            continue;
        }
        inList = false;
        inTable = false;
        tokens.push_back(Token{ TokenType::Paragraph, ParagraphToken{parseInline(line)} });
    }

    return tokens;
}