#ifndef COMMANDPARSER_HH
#define COMMANDPARSER_HH

/*!
 * \file
 * \brief Zawiera definicję klasy CommandParser
 *
 * Plik zawiera definicję klasy CommandParser, która parsuje
 * plik z poleceniami i wywołuje odpowiednie wtyczki.
 */

#include "CommandRegistry.hh"
#include <string>
#include <sstream>

/*!
 * \brief Klasa parsująca i wykonująca polecenia z pliku
 *
 * Klasa przetwarza plik z poleceniami, identyfikuje polecenia,
 * wywołuje odpowiednie wtyczki i przekazuje im parametry.
 */
class CommandParser {
public:
    /*!
     * \brief Konstruktor
     * \param[in] cmdRegistry - referencja do rejestru poleceń
     */
    CommandParser(CommandRegistry& cmdRegistry);
    
    /*!
     * \brief Destruktor
     */
    ~CommandParser() = default;

    /*!
     * \brief Parsuje i wykonuje polecenia z zawartości pliku
     *
     * \param[in] fileContent - zawartość pliku po preprocessingu
     * \retval true - wszystkie polecenia wykonane pomyślnie
     * \retval false - wystąpił błąd podczas parsowania lub wykonywania
     */
    bool ParseAndExecute(const std::string& fileContent);

private:
    CommandRegistry& _CmdRegistry;  ///< Referencja do rejestru poleceń
    
    /*!
     * \brief Przetwarza pojedynczą linię polecenia
     *
     * \param[in] line - linia z poleceniem
     * \param[in] lineNum - numer linii (do raportowania błędów)
     * \retval true - polecenie wykonane pomyślnie
     * \retval false - wystąpił błąd
     */
    bool ProcessCommandLine(const std::string& line, int lineNum);
};

#endif
