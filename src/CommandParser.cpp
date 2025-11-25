#include "CommandParser.hh"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

/*!
 * \brief Konstruktor
 */
CommandParser::CommandParser(CommandRegistry& cmdRegistry, AbstractScene& scene, AbstractComChannel& comChannel)
    : _CmdRegistry(cmdRegistry), _Scene(scene), _ComChannel(comChannel)
{
}

/*!
 * \brief Parsuje i wykonuje polecenia z zawartości pliku
 */
bool CommandParser::ParseAndExecute(const std::string& fileContent)
{
    stringstream contentStream(fileContent);
    int lineNum = 0;
    bool allSuccess = true;

    cout << "\n=====================================" << endl;
    cout << "  Parsowanie i wykonywanie poleceń" << endl;
    cout << "=====================================" << endl;

    // Czytamy bezpośrednio ze strumienia
    string cmdName;
    while (contentStream >> cmdName) {
        lineNum++;
        
        cout << "\n[Linia " << lineNum << "] Polecenie: " << cmdName << endl;
        
        string objName;
        
        // Wczytaj nazwę obiektu (oprócz Pause, które nie ma nazwy obiektu)
        if (cmdName != "Pause") {
            if (!(contentStream >> objName)) {
                cerr << "Linia " << lineNum << ": Błąd - brak nazwy obiektu" << endl;
                allSuccess = false;
                break;
            }
            cout << "  Obiekt: " << objName << endl;
        }
        
        // Utwórz instancję polecenia z rejestru
        AbstractInterp4Command* pCmd = _CmdRegistry.CreateCmd(cmdName);
        if (!pCmd) {
            cerr << "Linia " << lineNum << ": Błąd - nieznane polecenie: " 
                 << cmdName << endl;
            allSuccess = false;
            continue;  // Kontynuuj do następnego polecenia
        }
        
        // Wczytaj parametry polecenia bezpośrednio ze strumienia
        if (!pCmd->ReadParams(contentStream)) {
            cerr << "Linia " << lineNum << ": Błąd wczytywania parametrów" << endl;
            delete pCmd;
            allSuccess = false;
            continue;
        }
        
        // Wyświetl parametry
        cout << "  Parametry: ";
        pCmd->PrintCmd();
        
        // Wykonaj polecenie na scenie
        cout << "  Wykonywanie polecenia..." << endl;
        if (!pCmd->ExecCmd(_Scene, objName.c_str(), _ComChannel)) {
            cerr << "Linia " << lineNum << ": Błąd wykonywania polecenia" << endl;
            allSuccess = false;
        } else {
            cout << "  ✓ Polecenie wykonane pomyślnie" << endl;
        }
        
        // Zwolnij pamięć
        delete pCmd;
    }

    cout << "\n=====================================" << endl;
    if (allSuccess) {
        cout << "  Wszystkie polecenia wykonane" << endl;
    } else {
        cout << "  Wystąpiły błędy podczas wykonywania" << endl;
    }
    cout << "=====================================" << endl;

    return allSuccess;
}

