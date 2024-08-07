#include "texture.hpp"

std::unordered_map<std::string,tgaImage> textureCache;

tgaImage *decompressTGA (const std::string tgaFile){
     
    // Check if tgaImage texture is already cached
    if(auto search = textureCache.find(tgaFile); search != textureCache.end()) {
        // Return the texture if it is cached
        return &(search->second);
    }


    // This function is mostly stolen off the internet tga images are weird and im sure this wont support every edge case

    tgaImage decompressedImage;

    std::ifstream file(tgaFile, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: "<<tgaFile<<"\n";
        return nullptr;
    }

    char header[18];

    // Read the header from the tga file
    file.read(header, 18);

    uint16_t width = *(uint16_t*)&header[12];
    uint16_t height = *(uint16_t*)&header[14];
    uint8_t bpp = header[16];

    std::vector<uint8_t> imageData;
    while (file) {
        uint8_t chunkHeader;
        file.read((char*)&chunkHeader,1);
        if(!file) break;

        if(chunkHeader < 128) {
            size_t packetSize = chunkHeader + 1;
            std::vector<uint8_t> rawData(packetSize * (bpp / 8));
            file.read((char*)rawData.data(), rawData.size());
            imageData.insert(imageData.end(), rawData.begin(), rawData.end());
        } else {
            size_t packetSize = chunkHeader-127;
            std::vector<uint8_t> colorData(bpp / 8);
            file.read((char*)colorData.data(),colorData.size());
            for(size_t i = 0; i < packetSize; ++i) {
                imageData.insert(imageData.end(), colorData.begin(), colorData.end());
            }
        }
    }

    file.close();
   
    decompressedImage.width = width;
    decompressedImage.height = height;
    decompressedImage.bpp = bpp;
    decompressedImage.imageData = imageData;

    // Cache Texture
    textureCache.insert({tgaFile, decompressedImage});

    return decompressTGA(tgaFile);
};

pixel tgaReadPixel (const std::string tgaFile, float percentX, float percentY){

    tgaImage *inImage = decompressTGA(tgaFile);

    percentY = 1.0-percentY;

    size_t bytesPerPixel = inImage->bpp / 8;
    uint16_t x = (uint16_t)(inImage->width * percentX);
    uint16_t y = (uint16_t)(inImage->height * percentY);
    x = (x>inImage->width) ? inImage->width : x;
    x = (x<0) ? 0 : x;
    y = (y>inImage->height) ? inImage->height : y;
    y = (y<0) ? 0 : y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a=255;

    size_t index = (y * inImage->width + x) * bytesPerPixel;

    if ( inImage->bpp == 24 ) {
        r = inImage->imageData[index+2];
        g = inImage->imageData[index+1];
        b = inImage->imageData[index+0];
    } else {
        r = inImage->imageData[index+2];
        g = inImage->imageData[index+1];
        b = inImage->imageData[index+0];
        a = inImage->imageData[index+3];
    }

    pixel returnPixel={r,g,b,a};

    return returnPixel;

};
