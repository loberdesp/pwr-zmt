#include "CommandParser.hh"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

/*!
 * \brief Konstruktor
 */
CommandParser::CommandParser(CommandRegistry& cmdRegistry)
    : _CmdRegistry(cmdRegistry)
{
}

/*!
 * \brief Parsuje i wykonuje polecenia z zawartości pliku
 */
bool CommandParser::ParseAndExecute(const std::string& fileContent)
{
    stringstream contentStream(fileContent);
    string line;
    int lineNum = 0;
    bool allSuccess = true;

    cout << "\n=====================================" << endl;
    cout << "  Parsowanie i wykonywanie poleceń" << endl;
    cout << "=====================================" << endl;

    while (getline(contentStream, line)) {
        lineNum++;
        
        // Pomiń puste linie
        if (line.empty()) {
            continue;
        }
        
        // Przetwórz linię polecenia
        if (!ProcessCommandLine(line, lineNum)) {
            allSuccess = false;
            // Kontynuuj mimo błędu, aby zobaczyć wszystkie problemy
        }
    }

    cout << "=====================================" << endl;
    if (allSuccess) {
        cout << "  Wszystkie polecenia wykonane" << endl;
    } else {
        cout << "  Wystąpiły błędy podczas wykonywania" << endl;
    }
    cout << "=====================================" << endl;

    return allSuccess;
}

/*!
 * \brief Przetwarza pojedynczą linię polecenia
 */
bool CommandParser::ProcessCommandLine(const std::string& line, int lineNum)
{
    stringstream lineStream(line);
    string cmdName, objName;
    
    // Wczytaj nazwę polecenia
    if (!(lineStream >> cmdName)) {
        cerr << "Linia " << lineNum << ": Błąd - brak nazwy polecenia" << endl;
        return false;
    }
    
    cout << "\n[Linia " << lineNum << "] Polecenie: " << cmdName << endl;
    
    // Wczytaj nazwę obiektu (oprócz Pause, które nie ma nazwy obiektu)
    if (cmdName != "Pause") {
        if (!(lineStream >> objName)) {
            cerr << "Linia " << lineNum << ": Błąd - brak nazwy obiektu" << endl;
            return false;
        }
        cout << "  Obiekt: " << objName << endl;
    }
    
    // Utwórz instancję polecenia z rejestru
    AbstractInterp4Command* pCmd = _CmdRegistry.CreateCmd(cmdName);
    if (!pCmd) {
        cerr << "Linia " << lineNum << ": Błąd - nieznane polecenie: " 
             << cmdName << endl;
        return false;
    }
    
    // Wczytaj parametry polecenia
    if (!pCmd->ReadParams(lineStream)) {
        cerr << "Linia " << lineNum << ": Błąd wczytywania parametrów" << endl;
        delete pCmd;
        return false;
    }
    
    // Wyświetl parametry
    cout << "  Parametry: ";
    pCmd->PrintCmd();
    
    // Opcjonalnie: wykonaj polecenie (na razie tylko wyświetlamy)
    // AbstractScene scene;
    // AbstractComChannel channel;
    // pCmd->ExecCmd(scene, objName.c_str(), channel);
    
    delete pCmd;
    return true;
}
