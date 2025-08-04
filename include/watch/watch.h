#pragma once
#include "termmark.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include <fstream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

void watchMarkdownFile(const std::string& path, int intervalMs=1000);