#include "utils/compability.h"

bool supportsHyperlink() {
    const char* termProgram = std::getenv("TERM_PROGRAM");
    const char* term = std::getenv("TERM");
    const char* vteVersion = std::getenv("VTE_VERSION");
    const char* wtSession = std::getenv("WT_SESSION");

    if (termProgram) {
        std::string tp(termProgram);
        if (tp == "iTerm.app" || tp == "WezTerm" || tp == "Hyper") return true;
        if (tp == "WarpTerminal" || tp=="Apple_Terminal") return false; 
    }

    if (term) {
        std::string t(term);
        if (t.find("xterm") != std::string::npos || t.find("screen") != std::string::npos) return true;
    }

    if (vteVersion) return true;
    if (wtSession) return true;   

    return false;
}
