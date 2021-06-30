/*!
    \brief   TCP/IP szerver posix megvalosítása
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef TPCIP_PSX
#define TPCIP_PSX

#include "serverinterface.h"
#include "savemanagerinterface.h"

/*!
    \class PosixServer
    \brief Álltalános szerver osztályból származó speciális posix szerver
*/
class PosixServer:public ServerInterface{
    public:
        /*!
            \fn        PosixServer(uint16_t port, uint8_t connections, int32_t timeout, uint64_t limit, SaveManagerInterface& savemng)
            \brief     Szerver elinditasa
            \param[in] port        - Elérési port
            \param[in] connections - Egyszerre kapcsolódni tudó kliensek száma
            \param[in] timeout     - Passziv kliense várakozás ideje
            \param[in] limit       - Másodpercenként olvasott adatok száma kliensenként
            \param[in] savemng     - Kimentést irányító osztály
        */
        PosixServer(uint16_t port, uint8_t connections, int32_t timeout, uint64_t limit, SaveManagerInterface& savemng);
        
        PosixServer() = delete;
        PosixServer(const PosixServer&) = delete;
        PosixServer(PosixServer&&) = delete;

        /*!
            \fn        virtual ~PosixServer()
            \brief     Szerver szabályos lellítása
        */
        virtual ~PosixServer();

    private:
        int32_t sockfd;                 /*!< Szerver socketje */
        SaveManagerInterface& savemng;  /*!< Kimentést vezérlő osztály */

        /*!
            \fn        static void Accept(int32_t sockfd, uint8_t connections, int32_t timeout, uint64_t limit, bool& isRunning, ClientCounter& clientCounter, SaveManagerInterface& savemng)
            \brief     Szerver fő szála, kliensek fogadásáért felel
            \param[in] scok          - Szerver socket azonosító
            \param[in] port          - Elérési port
            \param[in] connections   - Egyszerre kapcsolódni tudó kliensek száma
            \param[in] timeout       - Passziv kliense várakozás ideje
            \param[in] limit         - Másodpercenként olvasott adatok száma kliensenként
            \param[in] isRunning     - Lállást itányító váltzó
            \param[in] clientCounter - Kliens számát nyilvántartó osztály
            \param[in] savemng       - Kimentést irányító osztály
        */
        static void Accept(int32_t sockfd, uint8_t connections, int32_t timeout, uint64_t limit, bool& isRunning, ClientCounter& clientCounter, SaveManagerInterface& savemng);
        /*!
            \fn        static void Read(int32_t id, int32_t timeout, uint64_t limit, ClientCounter& clientCounter, SaveManagerInterface& savemng)
            \brief     Adatok fogasásáért felelős szál
            \param[in] id            - Kliens socket azonosító
            \param[in] port          - Elérési port
            \param[in] connections   - Egyszerre kapcsolódni tudó kliensek száma
            \param[in] timeout       - Passziv kliense várakozás ideje
            \param[in] limit         - Másodpercenként olvasott adatok száma kliensenként
            \param[in] clientCounter - Kliens számát nyilvántartó osztály
            \param[in] savemng       - Kimentést irányító osztály
        */
        static void Read(int32_t id, int32_t timeout, uint64_t limit, ClientCounter& clientCounter, SaveManagerInterface& savemng);
};

#endif // TPCIP_PSX

// )\)