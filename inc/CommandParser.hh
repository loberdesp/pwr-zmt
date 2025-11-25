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
#include "AbstractScene.hh"
#include "AbstractComChannel.hh"
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
     * \param[in] scene - referencja do sceny z obiektami
     * \param[in] comChannel - referencja do kanału komunikacyjnego
     */
    CommandParser(CommandRegistry& cmdRegistry, AbstractScene& scene, AbstractComChannel& comChannel);
    
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
    CommandRegistry& _CmdRegistry;      ///< Referencja do rejestru poleceń
    AbstractScene& _Scene;              ///< Referencja do sceny z obiektami
    AbstractComChannel& _ComChannel;    ///< Referencja do kanału komunikacyjnego
};

#endif
