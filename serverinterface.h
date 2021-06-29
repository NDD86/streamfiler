/*!
    \brief   TCP/IP szerver interface
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef TPCIP_SRV
#define TPCIP_SRV

#include <cstdint>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <iostream>

/*!
    \class ServerInterface
    \brief Álltalános szerver osztály
*/
class ServerInterface{
    public:
        /*!
            \fn    ServerInterface()
            \brief TCP/IP szerver inicializálása
        */
        ServerInterface():isRunning{true}{}
        /*!
            \fn    virtual ~ServerInterface()
            \brief Gyermekek destruktorának meghívását biztosítja
        */
        virtual ~ServerInterface(){}

        /*!
            \fn    void Join()
            \brief Szerver fő szálának megvárása
        */
        void Join(){
            // Minden kliens megszűnést megvár
            clientCount.Wait();
            // Fő szálra vár
            if(acceptThread.get()->joinable()) acceptThread.get()->join();
        }

        /*!
            \fn    void Stop()
            \brief Szerver leállítási kérelme
        */
        void Stop(){
            isRunning = false;
        }

    protected:
        std::unique_ptr<std::thread> acceptThread; /*!< Szerver fő szála */
        bool isRunning; /*!< Futást irnyító változó*/

        /*!
            \struct ClientCounter
            \brief Kliens számának nyilvántartása
        */
        struct ClientCounter{
            std::condition_variable cv; /*!< Kliensek megállásának észlelését segíti */ 
            std::mutex mtx;             /*!< Mutex a biztonságos darabszőám állításhoz */
            uint8_t existConnections;   /*!< Kliensek száma*/

            /*!
                \fn    ClientCounter()
                \brief Kezdetben nincs kliens
            */
            ClientCounter():existConnections{0}{}

            /*!
                \fn    void Inc()
                \brief Kliens számláló növelése
            */
            void Inc(){
                const std::lock_guard<std::mutex> lock{mtx};
                ++existConnections;
            }

            /*!
                \fn    void Dec()
                \brief Kliens számláló csökkentése, értesítés a megszünésről
            */
            void Dec(){
                mtx.lock();
                --existConnections;
                mtx.unlock();
                cv.notify_one();
            }

            /*!
                \fn    void Wait()
                \brief Összes kliens megszünésésre várakozás
            */
            void Wait(){
                mtx.lock();
                while(existConnections != 0){
                    mtx.unlock();
                    {
                        std::unique_lock<std::mutex> lk(mtx);
                        cv.wait(lk);
                    }
                    mtx.lock();
                }
                mtx.unlock();
            }

            /*!
                \fn    bool Get(uint8_t max)
                \brief Kliens növelés ha lehetséges
                \param[in] - max Kliensek maximális számma
                \return true sikeres volt a növelés, flase nincs több szabad kapacsítás
            */
            bool Get(uint8_t max){
                const std::lock_guard<std::mutex> lock{mtx};
                if(existConnections<max){
                    ++existConnections;
                    return true;
                }
                return false;
            }
        } clientCount;
};

#endif // TPCIP_SRV

// )\)