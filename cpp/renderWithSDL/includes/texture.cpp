#include "texture.hpp"

tgaImage decompressTGA (const std::string tgaFile){
    
    // This function is mostly stolen off the internet tga images are weird and im sure this wont support every edge case

    tgaImage returnImage;

    std::ifstream file(tgaFile, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: "<<tgaFile<<"\n";
        return returnImage;
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
   
    returnImage.width = width;
    returnImage.height = height;
    returnImage.bpp = bpp;
    returnImage.imageData = imageData;

    return returnImage;
};

pixel tgaReadPixel (const tgaImage inImage, float percentX, float percentY){

    size_t bytesPerPixel = inImage.bpp / 8;
    uint16_t x = (uint16_t)(inImage.width * percentX);
    uint16_t y = (uint16_t)(inImage.height * percentY);
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a=255;

    size_t index = (y * inImage.width + x) * bytesPerPixel;

    if ( inImage.bpp == 24 ) {
        r = inImage.imageData[index+2];
        g = inImage.imageData[index+1];
        b = inImage.imageData[index+0];
    } else {
        r = inImage.imageData[index+2];
        g = inImage.imageData[index+1];
        b = inImage.imageData[index+0];
        a = inImage.imageData[index+3];
    }

    pixel returnPixel={r,g,b,a};

    return returnPixel;

};


int main() {
    tgaImage testtexture = decompressTGA ("textures/testtexture/testtexture.tga");
    
    pixel test = tgaReadPixel(testtexture, 0.8, 0.9); 

    std::cout << "R: "<<(int)test.r<<", G: "<<(int)test.g<<", B: "<<(int)test.b<<", A: "<<(int)test.a<<"/n";
};