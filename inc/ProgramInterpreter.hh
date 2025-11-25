#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

/*!
 * \file
 * \brief Zawiera definicję klasy ProgramInterpreter
 *
 * Plik zawiera definicję klasy ProgramInterpreter, która
 * jest główną klasą interpretera poleceń. Agreguje wszystkie
 * komponenty: scenę, rejestr wtyczek, kanał komunikacyjny.
 */

#include "Scene.hh"
#include "CommandRegistry.hh"
#include "ComChannel.hh"
#include "Configuration.hh"
#include <string>

/*!
 * \brief Główna klasa interpretera poleceń
 *
 * Klasa agreguje wszystkie komponenty systemu:
 * - Scenę z obiektami mobilnymi
 * - Rejestr wtyczek (LibManager)
 * - Kanał komunikacyjny z serwerem graficznym
 */
class ProgramInterpreter {
public:
    /*!
     * \brief Konstruktor
     */
    ProgramInterpreter();
    
    /*!
     * \brief Destruktor
     */
    ~ProgramInterpreter() = default;

    /*!
     * \brief Wczytuje konfigurację XML
     *
     * \param[in] FileName - nazwa pliku konfiguracyjnego XML
     * \retval true - konfiguracja wczytana pomyślnie
     * \retval false - błąd wczytywania
     */
    bool Read_XML_Config(const char* FileName);

    /*!
     * \brief Wykonuje program z pliku
     *
     * \param[in] FileName_Prog - nazwa pliku z programem do wykonania
     * \retval true - program wykonany pomyślnie
     * \retval false - błąd wykonywania programu
     */
    bool ExecProgram(const char* FileName_Prog);

private:
    Scene _Scn;                      ///< Scena z obiektami mobilnymi
    CommandRegistry _LibManager;     ///< Rejestr wtyczek (Set4LibInterfaces)
    ComChannel _Chann2Serv;          ///< Kanał komunikacyjny z serwerem
    Configuration _Config;           ///< Konfiguracja z XML
    
    /*!
     * \brief Inicjalizuje połączenie z serwerem graficznym
     * \retval true - połączenie nawiązane
     * \retval false - błąd połączenia
     */
    bool InitConnection();
    
    /*!
     * \brief Wysyła obiekty ze sceny do serwera graficznego
     */
    void SendObjectsToServer();
};

#endif
