#include "utils/cli_info.h"

const std::string VERSION = "1.0.0";
namespace cli {

std::string getHelpMarkdown() {
    return R"md(
# TermMark — A Terminal Markdown Renderer

## Usage
```sh
termmark [options] <file.md>
```

## Options
- `--watch`: Live Preview of md file (Updates when saved).
- `--guide`: Show an example Markdown document with all features.
- `--version`: Show version information.
- `--help`: Display this help message.

## Example
```sh
termmark file.md
termmark --watch file.md
termmark --guide
termmark --help
```
    )md";
}

std::string getVersionString() {
    return "TermMark version " + VERSION;
}

}