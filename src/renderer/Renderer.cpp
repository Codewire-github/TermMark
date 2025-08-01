#include "renderer/Renderer.h"
#include "utils/TermStyle.h"
#include <iostream>


void renderInline(const std::vector<InlineToken>& content) {
    for (const auto& token : content) {
        switch (token.type) {
            case InlineType::Bold:
                std::cout << termstyle::BOLD << token.text << termstyle::RESET;
                break;
            case InlineType::Italic:
                std::cout << termstyle::ITALIC << token.text << termstyle::RESET;
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
                case 2: color = termstyle::CYAN; break;
                case 3: color = termstyle::MAGENTA; break;
                case 4: color = termstyle::YELLOW; break;
                case 5: color = termstyle::BRIGHT_GREEN; break;
                default: color = termstyle::BOLD;
            }
            std::cout << "\n";
            std::cout << color << termstyle::BOLD;
            renderInline(token.content);
            std::cout << termstyle::RESET << "\n\n";
        } else {
            renderInline(token.content);
            std::cout << "\n";
        }
    }
}
