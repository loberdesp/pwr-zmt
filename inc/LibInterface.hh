#ifndef LIBINTERFACE_HH
#define LIBINTERFACE_HH

#include "AbstractInterp4Command.hh"
#include <string>
#include <dlfcn.h>

/*!
 * \file
 * \brief Zawiera definicję klasy LibInterface
 *
 * Plik zawiera definicję klasy LibInterface, która enkapsuluje
 * informacje o pojedynczej wtyczce (bibliotece współdzielonej).
 */

/*!
 * \brief Klasa reprezentująca interfejs do pojedynczej wtyczki
 *
 * Klasa przechowuje uchwyt do biblioteki współdzielonej,
 * nazwę polecenia oraz wskaźnik do funkcji tworzącej polecenie.
 */
class LibInterface {
public:
    /*!
     * \brief Konstruktor domyślny
     */
    LibInterface() = default;
    
    /*!
     * \brief Destruktor - automatycznie zamyka bibliotekę
     */
    ~LibInterface();

    /*!
     * \brief Ładuje wtyczkę z pliku biblioteki współdzielonej
     *
     * \param[in] sLibFileName - nazwa pliku biblioteki (np. "libInterp4Move.so")
     * \retval true - wtyczka załadowana pomyślnie
     * \retval false - błąd ładowania wtyczki
     */
    bool LoadPlugin(const char* sLibFileName);

    /*!
     * \brief Zwraca nazwę polecenia obsługiwanego przez wtyczkę
     *
     * \return Nazwa polecenia (np. "Move", "Set")
     */
    const std::string& GetCmdName() const { return _CmdName; }

    /*!
     * \brief Tworzy nową instancję polecenia
     *
     * \return Wskaźnik do nowo utworzonego obiektu polecenia lub nullptr
     */
    AbstractInterp4Command* CreateCmd() const;

private:
    void* _LibHandler = nullptr;  ///< Uchwyt do załadowanej biblioteki
    std::string _CmdName;          ///< Nazwa polecenia
    AbstractInterp4Command* (*_pCreateCmd)(void) = nullptr; ///< Wskaźnik do funkcji CreateCmd
};

#endif
