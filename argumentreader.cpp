#include "argumentreader.h"
#include <cstring>
#include <iostream>

ArgumentReader::ArgumentReader(int argc, char *argv[]):
    connections{2},
    timeout{5},
    limit{1},
    port{0},
    path{"./"}{

    // Beolvassa az argomentumokat
    ReadArgs(argc, argv);
    // Ellenörzi a paraméterek helyességét
    if(port == 0 ||                      // 0-s port nincs engedélyezve
       timeout < -1 || timeout == 0 ||   // -1-nél kissebb timeout az hiba, de rögtön kapcsolatbontás is
       limit == 0 ||                     // A bufferben kell lennie valamennyi helynek
       connections == 0)                 // Nincs értelme mindenkit kizárni a kapcsolatból
        PrintHelp("Bad argument!");
}

// Help kiiratás
void ArgumentReader::WriteHelp(){
    std::cout<<"NAME"<<std::endl;
    std::cout<<"\tstreamfiler - TCP/IP stream fogadó és lemezre író alkalmazás"<<std::endl<<std::endl;

    std::cout<<"SYNOPSIS"<<std::endl;
    std::cout<<"\tstreamfiler [-options] portnumber"<<std::endl<<std::endl;

    std::cout<<"DESCRIPTION"<<std::endl;
    std::cout<<"\tA streamfiler egy porton figyelő alkalmazás, ami a csatlakozott connection tartalmát lemezre írja."<<std::endl;
    std::cout<<"\tA kiírt fájl neve megegyezik az érkezési időponttal (milisecundum pontossággal)."<<std::endl;
    std::cout<<"\tEgyszerre több connection-t tud fogadni paraméterezéstől függően."<<std::endl<<std::endl;

    std::cout<<"OPTIONS"<<std::endl;

    std::cout<<"\t-c connections"<<std::endl;
    std::cout<<"\t   A maximálisan fogadható párhuzamos connection-ök száma"<<std::endl<<std::endl;

    std::cout<<"\t-f folder"<<std::endl;
    std::cout<<"\t   A mappa, ahova az érkztetett állományokat letárolja a program."<<std::endl;
    std::cout<<"\t   Ha nem létezik induláskor, akkor a program létrehozza azt."<<std::endl<<std::endl;

    std::cout<<"\t-h"<<std::endl;
    std::cout<<"\t   Ez a help"<<std::endl<<std::endl;

    std::cout<<"\t-l limit"<<std::endl;
    std::cout<<"\t   Ekkora limit adatmennyiséget enged szálanként és másodpercenként kilobyte-ban a portjára folyatni a program"<<std::endl<<std::endl;

    std::cout<<"\t-t timeout"<<std::endl;
    std::cout<<"\t   Azt az időt határozza meg másodpercben, ami után a program bontja az idle connection-öket. Timeout -1 esetén nem bontja."<<std::endl<<std::flush;
}

// Hibás paraméter esetén is jiírja a helpet, de ekkor kivétellel jelzi, hogy gond van
void ArgumentReader::PrintHelp(const char* message){
    WriteHelp();
    throw message;   
}

uint64_t ArgumentReader::ReadArgToUInt(int argc, char* param, uint32_t ind){
    // Ha van elég paraméter akkor átkonvetáljuk
    if(ind<argc){
        return std::stoull(param);

    }else{
        PrintHelp("Too few argument!");  
    }
    return 0;
}

int64_t ArgumentReader::ReadArgToInt(int argc, char* param, uint32_t ind){
    // Ha van elég paraméter akkor átkonvetáljuk
    if(ind<argc){
        return std::stoll(param);
    }else{
        PrintHelp("Too few argument!");   
    }
    return 0;
}

void ArgumentReader::ReadArgs(int argc, char *argv[]){
    for(uint32_t i = 1; i<argc; ++i){
        // Kapcsló következik
        if(argv[i][0] == '-'){
            // Nincs meg milyen kapcsoló, az argumentum csak egy - jel volt ez hiba
            if(strlen(argv[i]) != 2) PrintHelp("Bad switch!");
            // Kapcslonek megfelelő beolvasás
            switch(argv[i][1]){
                case 'h': {
                    WriteHelp();
                    return;
                }
                case 'c':{
                    connections = ReadArgToUInt(argc, argv[i+1], i+1);
                    break;
                }
                case 'l':{
                    limit = ReadArgToUInt(argc, argv[i+1], i+1);
                    break;
                }
                case 't':{
                    timeout = ReadArgToInt(argc, argv[i+1], i+1);
                    break;
                }
                case 'f':{
                    path = argv[i+1];
                    break;
                }
                // Ismeretlen kapcsoló esetén hibajelzés
                default:{
                    PrintHelp("Non exist switch!");
                }
                
            }
            ++i;
        }else{
            // Ha nem --szal kezdődik akkor vagy port vagy hiba, de a port mindig az utolsó paraméter
            if(i == argc-1){
                port = ReadArgToUInt(argc, argv[i], i);
            }else{
                PrintHelp("Bad argument list!");
            }
        }
    }
}

// )\)