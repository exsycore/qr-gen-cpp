#pragma once

#include "../libs/cpp-httplib/httplib.h"
#include "qr.hpp"
#include <png.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

std::string generate_qr_png(const std::string& text, const std::string& filename);
std::string generate_qr_png_memory(const std::string &text);