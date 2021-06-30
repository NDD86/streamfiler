/*!
    \brief   Álltalános kimentő osztály, ebből szármaik minden speciális kimentő
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef SAVER_IF
#define SAVER_IF

#include <cinttypes>

/*!
    \class SaverInterface
    \brief Álltalános kimentő osztály, ebből szármaik minden speciális kimentő
*/
class SaverInterface{
    public:
        /*!
            \fn    virtual ~SaverInterface()
            \brief Gyermekek destruktorának meghívását biztosítja
        */
        virtual ~SaverInterface(){}
        /*!
            \fn     virtual uint64_t Write(const char* buffer, int64_t bufferlen
            \brief  Adatok kimentésére szolgáló függvény
            \param[in] buffer    - Adat buffer
            \param[in] bufferlen - Kiírni kívánt adatok száma  
            \return    Kimentett adatok száma
        */
        virtual uint64_t Write(const char* buffer, int64_t bufferlen) = 0;
};

#endif // SAVER_IF

// )\)