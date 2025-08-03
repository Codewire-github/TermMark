#include "renderer/Renderer.h"
#include "utils/TermStyle.h"
#include "utils/compability.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <sys/ioctl.h>
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

void printHorizontalRule() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int width = w.ws_col;

    std::cout << termstyle::DARK_GRAY;
    for (int i = 0; i < width; ++i) std::cout << "_";
    std::cout << termstyle::RESET << "\n" << std::endl;
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

            else if constexpr (std::is_same_v<T, HorizontalRuleToken>) {
               
                printHorizontalRule();
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

           else if constexpr (std::is_same_v<T, TableToken>) {
                using namespace termstyle;

                std::vector<size_t> colWidths(content.headers.size(), 0);

                for (size_t i = 0; i < content.headers.size(); ++i) {
                    size_t cellWidth = 0;
                    for(const auto& token : content.headers[i]) {
                        cellWidth += token.text.length();
                    }
                    colWidths[i] = cellWidth;
                }

                for (const auto& row : content.rows) {
                    for (size_t i = 0; i < row.size(); ++i) {
                        size_t cellWidth = 0;
                        for(const auto& token : row[i]) {
                            cellWidth += token.text.length();
                        }
                        colWidths[i] = std::max(colWidths[i], cellWidth);
                    }
                }

                auto printRow = [&](const std::vector<std::vector<InlineToken>>& row, bool isHeader = false) {
                    std::cout << BRIGHT_BLACK << "│" << RESET;
                    for (size_t i = 0; i < row.size(); ++i) {
                        std::cout << " ";
                        if (isHeader) {
                            std::cout << BRIGHT_BLUE;
                        }
                        size_t currentWidth = 0;
                        for(const auto& token : row[i]) {
                            currentWidth += token.text.length();
                        }
                        renderInline(row[i]);
                        for (size_t j = 0; j < colWidths[i] - currentWidth; ++j) {
                            std::cout << " ";
                        }
                        std::cout << RESET << " ";
                        std::cout << BRIGHT_BLACK << "│" << RESET;
                    }
                    std::cout << "\n";
                };

                auto printDivider = [&](const std::string& start, const std::string& mid, const std::string& end) {
                    std::cout << BRIGHT_BLACK << start;
                    for (size_t i = 0; i < colWidths.size(); ++i) {
                        std::cout << std::string(colWidths[i] + 2, '-');
                        std::cout << (i == colWidths.size() - 1 ? end : mid);
                    }
                    std::cout << RESET << "\n";
                };

                printDivider("┌", "┬", "┐");
                printRow(content.headers, true);
                printDivider("├", "┼", "┤");

                for (const auto& row : content.rows) {
                    printRow(row);
                }

                printDivider("└", "┴", "┘");
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