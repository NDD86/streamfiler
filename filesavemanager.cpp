#include "filesavemanager.h"
#include "filesaver.h"

#include <filesystem>

FileSaveManager::FileSaveManager(std::string path):path(path){
    //Ha nem létezik a megadott könyvtár struktúra létre kell hozni
    std::filesystem::create_directories(path);

    // Ha nincs / az elérési út végén most dateszi, hogy a kimentőnek már csak a fájl nevével keljen fogalkozni
    if(path[path.size()-1] != '/') this->path += "/";
}

SaverInterface* FileSaveManager::CreateNewSaver(){
    // Kérésre új kimentőt készít
    return new FileSaver(path);
}

// )\)