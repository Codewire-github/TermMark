#include "renderer/Renderer.h"
#include "utils/TermStyle.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int getMaxLineWidth(const std::string& code) {
    std::istringstream iss(code);
    std::string line;
    int maxLen = 0;
    while (std::getline(iss, line)) {
        if ((int)line.length() > maxLen) maxLen = (int)line.length();
    }
    return maxLen;
}

void renderInline(const std::vector<InlineToken>& content) {
    for (const auto& token : content) {
        switch (token.type) {
            case InlineType::Bold:
                std::cout << termstyle::BOLD << token.text << termstyle::RESET;
                break;
            case InlineType::Italic:
                std::cout << termstyle::ITALIC << token.text << termstyle::RESET;
                break;

            case InlineType::Code:
                std::cout << termstyle::BG_CYAN << termstyle::BLACK << token.text << termstyle::RESET;
                break;
            case InlineType::BoldItalic:
                std::cout << termstyle::BOLD << termstyle::ITALIC << token.text << termstyle::RESET;
                break;
            case InlineType::Plain:
            default:
                std::cout << token.text;
        }
    }
}

void renderTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        if (token.type == TokenType::Heading) {
            std::string color;
            switch (token.headingLevel) {
                case 1: color = termstyle::BLUE; break;
                case 2: color = termstyle::GREEN; break;
                case 3: color = termstyle::MAGENTA; break;
                case 4: color = termstyle::YELLOW; break;
                case 5: color = termstyle::BRIGHT_GREEN; break;
                case 6: color = termstyle::CYAN; break;
                default: color = termstyle::BOLD;
            }
            std::cout << "\n";
            std::cout << color << termstyle::BOLD;
            renderInline(token.content);
            std::cout << termstyle::RESET << "\n\n";
        }
        else if (token.type == TokenType::Quote) {
            std::cout << "\n" << termstyle::BG_DARK_GRAY << termstyle::DARK_GRAY << "> " << termstyle::ITALIC;
            renderInline(token.content);
            std::cout << termstyle::RESET << "\n";

        }
        else if (token.type == TokenType::CodeBlock) {
    using namespace termstyle;
    const std::string& code = token.codeContent.code;
    const std::string& lang = token.codeContent.lang;

    int width = getMaxLineWidth(code);
    std::string topBorder = "┌─ " + lang + " " + std::string(width - lang.size(), '-') + "┐";
    std::string bottomBorder = "└" + std::string(width + 3, '-') + "┘";

    std::cout << BOLD << GREEN << "\n" << topBorder << RESET << "\n";

    std::istringstream iss(code);
    std::string line;
    while (std::getline(iss, line)) {
        std::cout << GREEN << "│ " << RESET
                  << std::left << std::setw(width) << line << "\n";
    }

    std::cout << GREEN << bottomBorder << RESET << "\n";
}

        else if (token.type == TokenType::List) {
             if (token.listItems.empty()) {
                    std::cerr << "Warning: List token with empty listItems\n";
             }
             else {
                 for (const auto& item : token.listItems) {
                     std::cout << "• ";
                     renderInline(item);
                     std::cout << "\n";
                 }
             }
            std::cout << "\n";

        } else {
            renderInline(token.content);
            std::cout << "\n";
        }
    }
}
