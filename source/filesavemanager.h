/*!
    \brief   Ettől az osztálytól lehet file kimentőt igényelni
    \author  David Nemeth-Dehalan
    \date    2021-06-29
    \version 0.0.0.1
*/

#ifndef FILE_SM
#define FILE_SM

#include <string>
#include "savemanagerinterface.h"

/*!
    \class FileSaveManager
    \brief Álltalános kimentés managerből származó speciális file kimentés manager
*/
class FileSaveManager: public SaveManagerInterface{
    public:
        /*!
            \fn        FileSaveManager(std::string path)
            \brief     Létrehozza az elérési útnak megfelelő könyvtárakat
            \param[in] path - Az elérési út ahova a filek mentődnek
        */
        FileSaveManager(std::string path);

        FileSaveManager() = delete;
        FileSaveManager(const FileSaveManager&) = delete;
        FileSaveManager(FileSaveManager&&) = delete;
        virtual ~FileSaveManager(){}

        /*!
            \fn     SaverInterface* CreateNewSaver() override
            \brief  Létrehoz egy új file kimentő osztályt
            \return Visszaad egy file kimentő osztályt
        */
        virtual SaverInterface* CreateNewSaver() override;

    private:
        std::string path; /*!< Az elérési út ahova a filek mentődnek */
};

#endif // FILE_SM

// )\)