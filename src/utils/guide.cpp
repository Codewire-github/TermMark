#include "utils/guide.h"

void printMarkdownGuide() {
    std::string content =
        R"(
# 📘 TermMark Syntax Demo

Welcome to the **TermMark Syntax Demo**. This is a live example showing what your terminal Markdown viewer — *TermMark* — can render beautifully in the terminal. Use this as a quick reference and demo of supported features like **bold**, *italic*, `inline code`, code blocks, lists, quotes, tables, and more.

## 🔠 Headings

TermMark supports six levels of headings using `#` symbols:

# Heading 1 – Page Title  
## Heading 2 – Section Title  
### Heading 3 – Subsection  
#### Heading 4 – Sub-subsection  
##### Heading 5 – Minor Heading  
###### Heading 6 – Smallest Heading

> Use headings to organize your Markdown content clearly.

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
    std::cout << "Hello from TermMark!" << std::endl;
    return 0;
}
````

### Python Example:

```python
def greet(name):
    print(f"Hello, {name}!")
greet("TermMark")
```

## 🔗 Links

You can also include links: [Visit GitHub](https://github.com/Codewire-github/TermMark)

Note: Terminals may show links as underlined or plain text, not clickable.

## 📝 Blockquote

> "Markdown is not a replacement for HTML, but it can complement it." — CommonMark Spec

## 🧱 Horizontal Rule

Use `---`, `***`, or `___` on a line by itself to create a horizontal divider.

---

This section appears after a horizontal rule.

## 📊 Tables

TermMark supports basic tables:

| Feature     | Notes                   |
| ----------- | ----------------------- |
| Bold        | Use `**bold**`          |
| Italic      | Use `*italic*`          |
| Code Blocks | Triple backticks ``` |

---

That's it! 🎉 This guide shows what TermMark can do. Try opening your own `.md` files and see them rendered right in your terminal.
)";

    auto tokens = parseMarkdown(content);
    renderTokens(tokens);
};