#include "posixserver.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cstring>
#include <unistd.h>
#include <atomic>

#include <iostream>
#include <fstream>
#include <chrono>
#include <string>


constexpr uint64_t SecInMicrosec{1000000}; // Másodpers millisecben
constexpr uint32_t BufferSize{1024};       // A program 1KB bufferrel dolgozik 

PosixServer::PosixServer(uint16_t port, uint8_t connections, int32_t timeout, uint64_t limit, SaveManagerInterface& savemng):ServerInterface{}, sockfd{socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)}, savemng{savemng}{

    // TCP/IP socket kérés eredményének ellenőrzése
    if(sockfd <= 0)throw "Bad socket fd!";

    int enable{1};
	// Ha nincs használatban (TIME_WAIT status) a port, address újra kiosztható legyen
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&enable, sizeof(enable));

    int32_t keepalive{1};
    if(timeout >= 0 ){
        int32_t kc{1};
        // Mivel az idel észleles utan 1 alkalommal 1s után űjra probálkozik, így 1s-et levonok az idel timeoutból
        int32_t idle{timeout-1};
        int32_t inv{1};
        if(idle == 0){
            idle = 1;
        }
        if(setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, &kc, sizeof(kc))<0){
            std::cerr<<"Keepcnt set error!"<<std::endl<<std::flush;
        }
        if(setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof(idle))<0){
            std::cerr<<"Keepidle set error!"<<std::endl<<std::flush;
        }
        if(setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, &inv, sizeof(inv))<0){
            std::cerr<<"Keepinvtvl set error!"<<std::endl<<std::flush;
        }
    }else{
        keepalive = 0;
    }

    // Megpróbálja beállitani keepalive-et de sajnos van, hogy hiba jelzes élkül, de nem sikerül
    if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive))<0){
        std::cerr<<"Keepalive set error!"<<std::endl<<std::flush;
    }

    // Megpróbálja a fogadó buffert 1024-re állítan, a terheles korlátozása miatt, de ekkorára nem lehet (nálam 2304 a minimum)
    int32_t bs = 1024;
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bs, sizeof(int32_t))<0){
        std::cerr<<"Rcvbuf set error!"<<std::endl<<std::flush;
    }

    sockaddr_in srvAddr{};

	srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = INADDR_ANY;
    srvAddr.sin_port = htons(port);

    // Socket és cím összerendeles
    if(bind(sockfd, (sockaddr *)&srvAddr, sizeof(sockaddr_in))){
        close(sockfd);
        throw "Binding error!";    
    }

    // Mivel tpc/ip a kapcsolatok számát figyelmen kívül hagyja
    if(listen(sockfd, connections)){
        close(sockfd);
        throw "Listen error!";
    }

    // Szálat indít a kilensek fogadásához, a pointer felszabadítás elmulasztásának elkerülése érdekébe smart pointert használok
    acceptThread = std::make_unique<std::thread>(std::thread(Accept, sockfd, connections, timeout, limit, std::ref(isRunning), std::ref(clientCount), std::ref(savemng)));
}



PosixServer::~PosixServer(){
    Stop();
    // Ha még nem állt le a szál akkor megvárja
    Join();
    close(sockfd);
}

void PosixServer::Accept(int32_t sockfd, uint8_t connections, int32_t timeout, uint64_t limit, bool& isRunning, ClientCounter& clientCounter, SaveManagerInterface& savemng){
    fd_set rfds;
    struct timeval tv;
    int32_t retval;

    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    tv.tv_sec = 1; // Másodpercenként ellenőrzi a bezárási kérelmet
    tv.tv_usec = 0;

    // Addig fut amíg nem jön bezárási kérés a programnak
    while(isRunning){
        // A bezárás blokkolásának elkerülése miatt van select 
        retval = select(sockfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
            // Socket hiba esetén unként leáll
            FD_CLR(sockfd, &rfds);
            isRunning = false;
        }else if(retval == 0){
            FD_CLR(sockfd, &rfds);
            FD_SET(sockfd, &rfds);
        }else{
            if(FD_ISSET(sockfd,&rfds)){
                // Új kliensek fogadása
                struct sockaddr_in cliAddr{};
                socklen_t cliLen = sizeof(cliAddr);
                int32_t newsockfd = accept(sockfd, (struct sockaddr *) &cliAddr, &cliLen);
                if(newsockfd>0){
                    // Mivel a listen nem kezeli, így itt is ellenőrzi, hogy van-e szabad kapacitása kliensnek
                    if(clientCounter.Get(connections)){
                        // Ha fogadta a klienst akkor az adatok olvasására új szálat indít
                        std::thread(Read, newsockfd, timeout, limit, std::ref(clientCounter), std::ref(savemng)).detach();
                        // Csak olvasható socket
                        shutdown(newsockfd, 1);
                    }else{
                        // Ha nem szabad kapacsízás bezárja a kliens socketjét
                        shutdown(newsockfd, 2);
                    }
                }
            }
        }
    }
}

void PosixServer::Read(int32_t id, int32_t timeout, uint64_t limit, ClientCounter& clientCounter, SaveManagerInterface& savemng){
    shutdown(id, 1);
    fd_set rfds;
    struct timeval tv;
    int32_t retval;
    
    // Az egyenletes terhelés miatt egyszere 1KB adatot olvas mindig.
    // Azért hogy meglegyen az 1s alatt beolvasandó adatok száma, kiszámlja mennyi időnként kell olvasnia
    uint64_t us = SecInMicrosec/limit;

    FD_ZERO(&rfds);
    FD_SET(id, &rfds);

    struct timeval *tvptr = NULL;
    // Ha a timeout nem -1 akkor beállít időzítést a selecthez
    if(timeout!=-1){
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        tvptr = &tv;
    }

    bool wait = true;
    try{
        // Az aktuális kimentő managertől kér kimetőt osztályt
        SaverInterface *saver = savemng.CreateNewSaver();
        while(wait) {   
                    
            retval = select(id+1, &rfds, NULL, NULL, tvptr);
            if(retval == -1){
                FD_CLR(id, &rfds);
                wait = false;
            }else if(retval == 0){
                FD_CLR(id, &rfds);
                wait = false;
            }else{
                if(FD_ISSET(id,&rfds)){
                    uint32_t bufferSize = BufferSize;
                    int32_t len = 1;
                    // 1KB méretű bufferrel dolgozik, de a TPC buffere nálam nagyobb, így garantalom, hogy a szerver csak az előre kivánt adat mennyiséget olvassa 
                    while(len>0 && bufferSize>0){
                        char buffer[BufferSize];    
                        memset(buffer, 0 ,BufferSize);
                        len = recv(id, buffer, bufferSize, MSG_DONTWAIT);
                        // Ha nem tud a program semmit olvasni akkor a kliens már átküldte a filet
                        if(len > 0){
                            saver->Write(buffer, len);
                            bufferSize -= len;
                        }else{
                            // Befejezi az olvasását
                            FD_CLR(id, &rfds);
                            wait = false;
                        }
                    }
                    usleep(us);
                }
            }
        }
        delete saver;
    }catch(const char* message){
        // Nem sikerülrt filet létrehozni
        std::cerr<<message<<std::endl;
    }

    // Átadja a helyét más kliensnek
    clientCounter.Dec();
    FD_ZERO(&rfds);
    close(id);
}

// )\)