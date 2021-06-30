/*!
    \brief   Adatok kimentését vezérlő osztályok őse
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef SM_IF
#define SM_IF

#include "saverinterface.h"

/*!
    \class SaveManagerInterface
    \brief Adatok kimentését vezérlő osztályok őse
*/
class SaveManagerInterface{
    public:
        /*!
            \fn    virtual ~SaveManagerInterface()
            \brief Gyermekek destruktorának meghívását biztosítja
        */
        virtual ~SaveManagerInterface(){}
        /*!
            \fn     virtual SaverInterface* CreateNewSaver()
            \brief  Ez szolgál az aktuális kimentő osztály megszerzésére   
            \return Kimentő osztály pointere
        */
        virtual SaverInterface* CreateNewSaver() = 0;
};

#endif // SM_IF

// )\)