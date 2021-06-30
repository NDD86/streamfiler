#include "histomaker.h"
#include <png.h>
#include <fstream>

#include "bitmap_image.hpp"

void HistoMaker::GetHistoFromPNG(std::string fileName, Histo& histo){
    FILE *inFile = fopen(fileName.c_str(), "rb");
    if(!inFile)throw "Open error!";
    
    // Ha nem így kezdődik nem png file
    if(fgetc(inFile) != 0x89 || fgetc(inFile) != 0x50 || fgetc(inFile) != 0x4E || fgetc(inFile) != 0x47){
        fclose(inFile);
        throw "Not png!";
    }

    // Ha png akkor visszaugrik a file elejére
    fseek(inFile, 0, SEEK_SET);

    // libpng inicializálás
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png){
        fclose(inFile);
        throw "PNG error!";
    }

    // info struktura készítése
    png_infop info = png_create_info_struct(png);
    if(!info){
        fclose(inFile);
        png_destroy_read_struct(&png, NULL, NULL);
        throw "PNG Info error!";
    }

    if(setjmp(png_jmpbuf(png))){
        fclose(inFile);
        png_destroy_read_struct(&png, &info, NULL);
        throw "PNG jump error!";
    }

    // png és file összekötése
    png_init_io(png, inFile);
    // információk kiolvasása
    png_read_info(png, info);

    int32_t width      = png_get_image_width(png, info);
    int32_t height     = png_get_image_height(png, info);
    png_byte colorType = png_get_color_type(png, info);
    png_byte bitDepth  = png_get_bit_depth(png, info);

    // Minden esetben 8 bites RGBA formátumot szed ki, ha nem ilyen átalakítja 
    if(bitDepth == 16) png_set_strip_16(png);
    if(colorType == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);

    if(colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)png_set_expand_gray_1_2_4_to_8(png);
    if(png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if(colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if(colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);

    // Formátum konverzió
    png_read_update_info(png, info);

    // Hely fogalása a kép pontoknak
    png_bytep *rowPointers = new png_bytep[height];
    for(int32_t y = 0; y < height; y++) {
        rowPointers[y] = new png_byte[png_get_rowbytes(png,info)];
    }

    // Adatok kiolvasása
    png_read_image(png, rowPointers);

    fclose(inFile);
    png_destroy_read_struct(&png, &info, NULL);

    // Hisztogram készítés
    for(int32_t y = 0; y < height; y++) {
        png_bytep row = rowPointers[y];
        for(int32_t x = 0; x < width; x++) {
            png_bytep px = &(row[x*4]);
            ++histo.r[px[0]];
            if(histo.max<histo.r[px[0]]) histo.max = histo.r[px[0]];
            ++histo.g[px[1]];
            if(histo.max<histo.g[px[1]]) histo.max = histo.g[px[1]];
            ++histo.b[px[2]];
            if(histo.max<histo.b[px[3]]) histo.max = histo.b[px[3]];
        }
    }

    // Lefogalat terület felszabadítása
    for(int32_t y = 0; y < height; y++) {
        delete[] rowPointers[y];
    }
    delete[] rowPointers;
}

void HistoMaker::GetHistoFromBMP(std::string fileName, Histo& histo){
    FILE *inFile = fopen(fileName.c_str(), "rb");
    if(!inFile)throw "Open error!";
    
    // Ha nem így kezdődik nem png file
    if(fgetc(inFile) != 'B' || fgetc(inFile) != 'M'){
        fclose(inFile);
        throw "Not bmp!";
    }
    fclose(inFile);

    bitmap_image image(fileName);

   if (!image)throw "Open error!";

   uint32_t height = image.height();
   uint32_t width  = image.width();

   for(uint32_t y = 0; y < height; ++y){
        for(uint32_t x = 0; x < width; ++x){
            rgb_t color;
            image.get_pixel(x, y, color);
            ++histo.r[color.red];
            if(histo.max<histo.r[color.red]) histo.max = histo.r[color.red];
            ++histo.g[color.green];
            if(histo.max<histo.g[color.green]) histo.max = histo.g[color.green];
            ++histo.b[color.blue];
            if(histo.max<histo.b[color.blue]) histo.max = histo.b[color.blue];
        }
   }
}

constexpr uint32_t histoWidth  = 256; // Hiszto szélessége 
constexpr uint32_t histoHeight = 128; // Hiszto magassága 

/*!
    \fn    void DrawHisto(bitmap_image& canvas, rgb_t color, uint64_t histo[256], uint32_t offset, uint32_t max)
    \brief Hisztogram kimentése BMPbe
    \param[out] canvas - Ide készül el a kép
    \param[in]  color  - Hisztogram színe
    \param[in]  histo  - Hisztogram
    \param[in]  offset - Képen belüli y irányú eltolás
    \param[in]  max    - Maximális érték a 3 (pirs, zöld, kék) hisztogramban
*/
void DrawHisto(bitmap_image& canvas, rgb_t color, uint64_t histo[256], uint32_t offset, uint32_t max){
    for(uint32_t x = 0; x < histoWidth; ++x){
        uint32_t end = (histo[x]/double(max))*128;
        for(uint32_t i = 1; i <= end; ++i){
            canvas.set_pixel(x,offset+histoHeight-i,color);
        }
    }    
}

void HistoMaker::SaveHistoToBMP(std::string fileName, Histo& histo){
    bitmap_image image(histoWidth,histoHeight*3+2);

    // Háttér szín
    image.set_all_channels(255, 255, 255);
    rgb_t red{255,0,0};
    DrawHisto(image, red, histo.r, 0, histo.max);

    rgb_t green{0,255,0};
    DrawHisto(image, green, histo.g, histoHeight+1, histo.max);

    rgb_t blue{0,0, 255};
    DrawHisto(image, blue, histo.b, histoHeight*2+2, histo.max);

    image.save_image(fileName+".bmp");
}