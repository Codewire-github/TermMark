#include "renderer/Renderer.h"
#include "utils/TermStyle.h"
#include "utils/compability.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

int getMaxLineWidth(const std::string& code) {
    std::istringstream iss(code);
    std::string line;
    int maxLen = 0;
    while (std::getline(iss, line)) {
        maxLen = std::max(maxLen, static_cast<int>(line.length()));
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
            case InlineType::BoldItalic:
                std::cout << termstyle::BOLD << termstyle::ITALIC << token.text << termstyle::RESET;
                break;
            case InlineType::Code:
                std::cout << termstyle::BG_DARK_GRAY << termstyle::LIGHT_GRAY << token.text << termstyle::RESET;
                break;
            case InlineType::HyperLink:
                if (supportsHyperlink()) {
                    std::cout << termstyle::BRIGHT_BLUE << "\033]8;;" << token.url << "\033\\"
                        << termstyle::UNDERLINE << token.text << termstyle::RESET
                        << "\033]8;;\033\\";
                }
                else {
                    std::cout << token.text << " (" << termstyle::UNDERLINE << token.url << termstyle::RESET << ")";
                }
                break;
            default:
                std::cout << token.text;
        }
    }
}


void renderTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::visit([&](auto&& content) {
            using T = std::decay_t<decltype(content)>;

            if constexpr (std::is_same_v<T, HeadingToken>) {
                std::string color;
                switch (content.level) {
                    case 1: color = termstyle::MAGENTA; break;
                    case 2: color = termstyle::GREEN; break;
                    case 3: color = termstyle::BLUE; break;
                    case 4: color = termstyle::YELLOW; break;
                    case 5: color = termstyle::BRIGHT_GREEN; break;
                    case 6: color = termstyle::CYAN; break;
                    default: color = termstyle::BOLD; break;
                }

                std::cout << color << termstyle::BOLD;
                renderInline(content.content);
                std::cout << termstyle::RESET << "\n\n";
            }

            else if constexpr (std::is_same_v<T, ParagraphToken>) {
                renderInline(content.content);
                std::cout << "\n\n";
            }

            else if constexpr (std::is_same_v<T, QuoteToken>) {
                std::cout << termstyle::BG_DARK_GRAY << termstyle::LIGHT_GRAY << "> " << termstyle::ITALIC;
                renderInline(content.content);
                std::cout << termstyle::RESET << "\n\n";
            }

           else if constexpr (std::is_same_v<T, ListToken>) {
            if (content.items.empty()) {
                std::cerr << "Warning: List token with empty items\n";
             } else {
            for (size_t i = 0; i < content.items.size(); ++i) {
                if (content.type == ListType::Ordered) {
                    std::cout << std::to_string(i + 1) << ". ";
                } else {
                    std::cout << "• ";
                }
                renderInline(content.items[i]);
                std::cout << "\n";
            }
            }
            std::cout << "\n";
            }


            else if constexpr (std::is_same_v<T, CodeBlockToken>) {
                using namespace termstyle;

                const std::string& code = content.code;
                const std::string& lang = content.lang.empty() ? "code" : content.lang;

                int width = getMaxLineWidth(code);
                std::string topBorder    = "┌─ " + lang + " " + std::string(std::max(0, width - static_cast<int>(lang.length())), '-') + "┐";
                std::string bottomBorder = "└" + std::string(width + 3, '-') + "┘";

                std::cout << BOLD << BRIGHT_YELLOW << topBorder << RESET << "\n";

                std::istringstream iss(code);
                std::string line;
                while (std::getline(iss, line)) {
                    std::cout << BRIGHT_YELLOW << "│ " << RESET
                              << std::left << std::setw(width) << line << "\n";
                }

                std::cout << BRIGHT_YELLOW << bottomBorder << RESET << "\n\n";
            }

        }, token.data);
    }
}
