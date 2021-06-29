#ifndef HISTO
#define HISTO

#include <string>

struct Histo{
    uint64_t r[256];
    uint64_t g[256];
    uint64_t b[256];

    uint64_t max;

    Histo():r{},g{},b{},max{0}{}
};

/*!
    \class HistoMaker
    \brief BMP, és PNG képből csinál hisztogrammot, és hisztogrammot ment BMP formátumba
*/
class HistoMaker{
    public:
        /*!
            \fn    void GetHistoFromPNG(std::string fileName, Histo& histo)
            \brief Hisztogram készítése PNGből
            \param[in]  filename - A vizsgálni kívánt file neve
            \param[out] histo    - Az elkészült hisztgram
        */
        void GetHistoFromPNG(std::string fileName, Histo& histo);

        /*!
            \fn    void GetHistoFromBMP(std::string fileName, Histo& histo)
            \brief Hisztogram készítése BMPből
            \param[in]  filename - A vizsgálni kívánt file neve
            \param[out] histo    - Az elkészült hisztgram
        */
        void GetHistoFromBMP(std::string fileName, Histo& histo);

        /*!
            \fn    void SaveHistoToBMP(std::string fileName, Histo& histo)
            \brief Hisztogram kimentése BMPbe
            \param[in]  filename - A vizsgálni kívánt file neve
            \param[out] histo    - Az hisztgram
        */
        void SaveHistoToBMP(std::string fileName, Histo& histo);
};

#endif // HISTO

// )\)