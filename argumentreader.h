/*!
    \brief   Argumentumok beolvasása
    \author  David Nemeth-Dehalan
    \details Ez a struktúra végi az argumentumok beolvasását, és ellenőrzését
             Ezt már  konstruktórban elvégzi ezért az osztály létrehozása után már a paranéterek rendelkezésre állnak
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef ARG_READ
#define ARG_READ

#include <string>

/*!
\struct ArgumentReader
\brief Az argumentum beolvasását megvalosító struktúra
*/
struct ArgumentReader{
        uint8_t     connections; /*!< Egyszerre kapcsolódni tudó kliensek száma */
        int32_t     timeout;     /*!< A szerve eddig tűri a passzív viselkedést a klienstől */
        uint64_t    limit;       /*!< Ennyi KB adatt fogad a szerver másodpercenként */
        uint16_t    port;        /*!< Szerver fogyelési portja */
        std::string path;        /*!< Erre az elérési útra menti a szerver a kapott fájlokat*/

        /*!
            \fn        ArgumentReader(int argc, char *argv[])
            \brief     A struktúra konstruktora a ami lefutása után elérhetők lesznek a paraméterek
            \param[in] argc   - Argumentumok száma az argv-ben
            \param[in] argv   - Argumentumok listája
        */
        ArgumentReader(int argc, char *argv[]);

        ArgumentReader() = delete;
        ArgumentReader(const ArgumentReader&) = delete;
        ArgumentReader(ArgumentReader&&) = delete;

        /*!
            \fn     void WriteHelp();
            \brief  Strandard kimenetre írja a program használat utasításait
        */
        void WriteHelp();
        /*!
            \fn        void PrintHelp(const char* message)
            \brief     Strandard kimenetre írja a program használat utasításait, és a kapott üzenettel kivételt dob
            \param[in] message   - Kivétel szövege
        */
        void PrintHelp(const char* message);
    
    private:
        /*!
            \fn        uint64_t ReadArgToUInt(int argc, char* param, uint32_t ind)
            \brief     Unsigned számmá konvertál egy argumentumot
            \param[in] argc  - Összes argumentum száma
            \param[in] param - Konvertállásra váró argumentum
            \param[in] ind   - Argumentum indexe
            \return    Beolvasott paraméter értéke
        */
        uint64_t ReadArgToUInt(int argc, char* param, uint32_t ind);
        /*!
            \fn        int64_t  ReadArgToInt(int argc, char* param, uint32_t ind)
            \brief     Számmá konvertál egy argumentumot
            \param[in] argc  - Összes argumentum száma
            \param[in] param - Konvertállásra váró argumentum
            \param[in] ind   - Argumentum indexe
            \return    Beolvasott paraméter értéke
        */
        int64_t  ReadArgToInt(int argc, char* param, uint32_t ind);
        /*!
            \fn        void ReadArgs(int argc, char *argv[])
            \brief     Argumentumok feldolgozása, és ellenőrzése
            \param[in] argc   - Argumentumok száma az argv-ben
            \param[in] argv   - Argumentumok listája
        */
        void     ReadArgs(int argc, char *argv[]);
};

#endif // ARG_READ

// )\)