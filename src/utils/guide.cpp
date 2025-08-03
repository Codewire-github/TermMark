#include "utils/guide.h"

void printMarkdownGuide() {
    std::string content = R"(
# 📘 Markdown Syntax Guide

Welcome to the **Markdown Syntax Guide**. This guide shows how to format text using *Markdown*, including how to use `inline code`, code blocks, lists, quotes, and more.

## 🔠 Headings

Headings are created using `#` symbols at the beginning of a line. The number of `#` symbols determines the heading level (from 1 to 6).

# Heading 1 – Page Title
## Heading 2 – Section Title
### Heading 3 – Subsection
#### Heading 4 – Sub-subsection
##### Heading 5 – Minor Heading
###### Heading 6 – Smallest Heading

> Use headings to organize your content clearly.

## ✨ Text Formatting

You can write **bold**, *italic*, or ***bold italic*** text.

Inline code looks like this: `printf("Hello, World!");`

## 🔢 Lists

### Unordered List:
- Apples
- Bananas
  - Yellow
  - Ripe
- Cherries

### Ordered List:
1. Install dependencies
2. Run the app
3. Profit 💰

## 🧠 Code Blocks

### C++ Example:
```cpp
#include <iostream>
int main() {
    std::cout << "Hello from C++!" << std::endl;
    return 0;
}
```
### Python Example:
```python
def greet(name):
    print(f"Hello, {name}!")
greet("Markdown")
```

## 🔗 Links

Here's a useful resource: [Visit GitHub](https://github.com/Codewire-github/TermMark)

If your terminal doesn't support clickable links, it will show as underlined or plain text.
## 📝 Blockquote

> "Markdown is not a replacement for HTML, but it can complement it." — CommonMark Spec

That’s it! Use this guide as a reference when writing notes or documentation in Markdown.
)";
    auto tokens = parseMarkdown(content);
    renderTokens(tokens);
};