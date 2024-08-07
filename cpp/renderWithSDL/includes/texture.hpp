#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

struct tgaImage {
    int width;
    int height;
    int bpp;
    std::vector<uint8_t> imageData;
};

struct pixel {
    uint8_t r,g,b,a;
};

pixel tgaReadPixel (const std::string tgaFile, float percentX, float percentY);

tgaImage *decompressTGA (const std::string tgaFile);

extern std::unordered_map<std::string,tgaImage> textureCache;


