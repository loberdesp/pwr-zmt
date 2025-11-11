#ifndef COMMANDREGISTRY_HH
#define COMMANDREGISTRY_HH

/*!
 * \file
 * \brief Zawiera definicję klasy CommandRegistry
 *
 * Plik zawiera definicję klasy CommandRegistry, która zarządza
 * rejestrem wtyczek i umożliwia wyszukiwanie ich po nazwie polecenia.
 */

#include "LibInterface.hh"
#include <map>
#include <string>
#include <memory>

/*!
 * \brief Klasa zarządzająca rejestrem poleceń i wtyczek
 *
 * Klasa przechowuje mapowanie nazw poleceń na interfejsy wtyczek.
 * Umożliwia rejestrację wtyczek oraz wyszukiwanie ich po nazwie polecenia.
 */
class CommandRegistry {
public:
    /*!
     * \brief Konstruktor domyślny
     */
    CommandRegistry() = default;
    
    /*!
     * \brief Destruktor
     */
    ~CommandRegistry() = default;

    /*!
     * \brief Rejestruje wtyczkę w rejestrze
     *
     * Ładuje wtyczkę z podanego pliku i dodaje ją do rejestru.
     * \param[in] sLibFileName - nazwa pliku biblioteki (np. "libInterp4Move.so")
     * \retval true - wtyczka zarejestrowana pomyślnie
     * \retval false - błąd rejestracji wtyczki
     */
    bool RegisterCmd(const char* sLibFileName);

    /*!
     * \brief Wyszukuje wtyczkę po nazwie polecenia
     *
     * \param[in] sCmdName - nazwa polecenia (np. "Move", "Set")
     * \return Wskaźnik do interfejsu wtyczki lub nullptr jeśli nie znaleziono
     */
    const LibInterface* FindCmd(const std::string& sCmdName) const;

    /*!
     * \brief Tworzy instancję polecenia o podanej nazwie
     *
     * \param[in] sCmdName - nazwa polecenia
     * \return Wskaźnik do nowo utworzonego obiektu polecenia lub nullptr
     */
    AbstractInterp4Command* CreateCmd(const std::string& sCmdName) const;

    /*!
     * \brief Wyświetla listę zarejestrowanych poleceń
     */
    void PrintRegisteredCommands() const;

    /*!
     * \brief Zwraca liczbę zarejestrowanych poleceń
     */
    size_t GetCmdCount() const { return _CmdMap.size(); }

private:
    /*!
     * \brief Mapa przechowująca pary: nazwa_polecenia -> interfejs_wtyczki
     */
    std::map<std::string, std::shared_ptr<LibInterface>> _CmdMap;
};

#endif
