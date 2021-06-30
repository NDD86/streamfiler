/*!
    \brief   Szerver alkalmazás ami a klienstől fogadott fileket menti a háttértrra 
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <string>
#include <cstring>
#include <mutex>
#include <condition_variable>

#include "posixserver.h"
#include "argumentreader.h"
#include "filesavemanager.h"

std::condition_variable cv; /*< Signal értesités */ 
std::mutex mtx;             /*< Mutex a signalhoz */

 // Signal esetén végzenő feladat 
void termApp(int sig){
    if(sig == SIGTERM || sig == SIGINT)cv.notify_one();
}

int main(int argc, char *argv[]){
    try{
        // Lekezelt signalok
        std::signal(SIGTERM, termApp);
		std::signal(SIGINT, termApp);

        // Elemzi a kapott paramétereket
        ArgumentReader args(argc, argv);

        // Elkészíti a file kimetését vezérlő osztályt
        FileSaveManager fileManager(args.path);

        // Elindítja a szervere
        std::unique_ptr<ServerInterface> server{new PosixServer{args.port, args.connections, args.timeout, args.limit, fileManager}};
        
        // Addig dolgozik amíg nem jön signal
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk);

        // Szerver szabályos lellítása
        server.get()->Stop();
        std::cout<<"waiting for clients to end"<<std::endl<<std::flush;
    }catch(const char* str){
        // Hibák megjelenítése
        std::cerr<<str<<std::endl<<std::flush;
    }
    return 0;
}