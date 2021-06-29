/*!
    \brief   Ennek az osztlynak a segítségével írodik fileba a kapott adat
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef FILE_SAVER
#define FILE_SAVER

#include "saverinterface.h"
#include <fstream>

/*!
    \class FileSaver
    \brief Álltalános kimentő osztályból származó speciális file kimentő
*/
class FileSaver: public SaverInterface{
    public:
        /*!
            \fn        FileSaver(std::string path)
            \brief     Megnyitja a filet a megfelelő néven
            \param[in] path - Az elérési út ahova a file kerül
        */
        FileSaver(std::string path);

        FileSaver() = delete;
        FileSaver(const FileSaver&) = delete;
        FileSaver(FileSaver&&) = delete;

        virtual ~FileSaver(){}

        /*!
            \fn        virtual uint64_t Write(const char* buffer, int64_t bufferlen) override
            \brief     Adatok fileba írsa
            \param[in] buffer    - Adatok
            \param[in] bufferlen - Adatok száma
            \return    Kimentett adatok száma
        */
        virtual uint64_t Write(const char* buffer, int64_t bufferlen) override;
        
    private:

        std::ofstream file; /*< Ebbe a fileba mentődnek az adatok */
};

#endif // FILE_SAVER

// )\)