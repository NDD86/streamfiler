#include "filesaver.h"
#include <chrono>

FileSaver::FileSaver(std::string path){
    // Idő meghatározása
    int32_t elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(/*std::chrono::system_clock::now().time_since_epoch()*/std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    std::string s = std::to_string(elapsed_milliseconds);
    // File név elkészítése
    path += std::to_string(elapsed_milliseconds);
    // File megnyitsa
    file.open(path.c_str() ,std::ios::binary);
    // Ha nem sikerült megnyitni a filet akkor kivétel jelzi 
    if(file.fail()) throw "File open error!";

    fileName = path;
}

FileSaver::~FileSaver(){
    file.close();
    Histo histo;
    HistoMaker histoMaker;
    try{
        histoMaker.GetHistoFromPNG(fileName, histo);
    }catch(const char*){
        try{
            histoMaker.GetHistoFromBMP(fileName, histo);
        }catch(const char*){
            // Nem PNG, vagy BMP volt
            return;
        }
    }
    histoMaker.SaveHistoToBMP(fileName, histo);
}

uint64_t FileSaver::Write(const char* buffer, int64_t bufferlen){
    // Adatok filebe írása
    file.write(buffer, bufferlen);
    return bufferlen;
}

// )\)