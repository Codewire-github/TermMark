#include "utils/cli_info.h"

constexpr const char* VERSION = "1.0.0";
namespace cli {

std::string getHelpMarkdown() {
    return R"md(
# TermMark — A Terminal Markdown Renderer

## Usage
```sh
termmark [options] <file.md>
```

## Options
- `--guide`: Show an example Markdown document with all features.
- `--version`: Show version information.
- `--help`: Display this help message.

## Example
```sh
termmark notes.md
termmark --guide
termmark --help
```
    )md";
}

std::string getVersionString() {
    return "TermMark version 1.0.0";
}

}