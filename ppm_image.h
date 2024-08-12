#pragma once
#include "img_lib.h"

#include <filesystem>

namespace img_lib {
using Path = std::filesystem::path;

Image LoadPPM(const Path& file);

bool SavePPM(const Path& file, const Image& image);

}  // namespace img_lib