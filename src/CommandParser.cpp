#include "CommandParser.hh"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

// Mutex globalny do synchronizacji wypisów
static std::mutex g_cout_mutex;

/*!
 * \brief Konstruktor
 */
CommandParser::CommandParser(CommandRegistry& cmdRegistry, AbstractScene& scene, AbstractComChannel& comChannel)
    : _CmdRegistry(cmdRegistry), _Scene(scene), _ComChannel(comChannel)
{
}

/*!
 * \brief Wykonuje pojedyncze polecenie
 */
bool CommandParser::ExecuteSingleCommand(const std::string& cmdName, const std::string& objName, std::istream& params)
{
    bool success = true;
    
    // Synchronizacja wypisów
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cout << "\n[Wątek " << std::this_thread::get_id() << "] Polecenie: " << cmdName << endl;
        if (!objName.empty()) {
            cout << "  Obiekt: " << objName << endl;
        }
    }
    
    // Utwórz instancję polecenia z rejestru
    AbstractInterp4Command* pCmd = _CmdRegistry.CreateCmd(cmdName);
    if (!pCmd) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cerr << "!!! Błąd: Nie znaleziono polecenia '" << cmdName << "'" << endl;
        return false;
    }
    
    // Wczytaj parametry polecenia
    if (!pCmd->ReadParams(params)) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cerr << "!!! Błąd wczytywania parametrów dla '" << cmdName << "'" << endl;
        delete pCmd;
        return false;
    }
    
    // Wyświetl parametry
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cout << "  Parametry: ";
        pCmd->PrintCmd();
    }
    
    // Wykonaj polecenie na scenie
    {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cout << "  Wykonywanie polecenia..." << endl;
    }
    
    if (!pCmd->ExecCmd(_Scene, objName.c_str(), _ComChannel)) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cerr << "!!! Błąd wykonywania polecenia '" << cmdName << "'" << endl;
        success = false;
    } else {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        cout << "  ✓ Polecenie wykonane pomyślnie" << endl;
    }
    
    // Zwolnij pamięć
    delete pCmd;
    
    return success;
}

/*!
 * \brief Parsuje i wykonuje polecenia z zawartości pliku
 */
bool CommandParser::ParseAndExecute(const std::string& fileContent)
{
    stringstream contentStream(fileContent);
    bool allSuccess = true;

    cout << "\n=====================================" << endl;
    cout << "  Parsowanie i wykonywanie poleceń" << endl;
    cout << "=====================================" << endl;

    string token;
    while (contentStream >> token) {
        if (token == "Begin_Parallel_Actions") {
            // === BLOK WSPÓŁBIEŻNY ===
            cout << "\n>>> Rozpoczęcie bloku poleceń współbieżnych <<<" << endl;
            
            // Struktura do przechowywania informacji o poleceniu
            struct CommandInfo {
                string cmdName;
                string objName;
                string params;
            };
            
            std::vector<CommandInfo> parallelCmds;
            
            // Zbierz wszystkie polecenia z bloku
            while (contentStream >> token && token != "End_Parallel_Actions") {
                CommandInfo cmdInfo;
                cmdInfo.cmdName = token;
                
                // Wczytaj nazwę obiektu (jeśli nie Pause)
                if (token != "Pause") {
                    if (!(contentStream >> cmdInfo.objName)) {
                        cerr << "!!! Błąd: Brak nazwy obiektu dla '" << token << "'" << endl;
                        allSuccess = false;
                        break;
                    }
                }
                
                // Wczytaj resztę linii jako parametry
                std::string line;
                std::getline(contentStream, line);
                cmdInfo.params = line;
                
                parallelCmds.push_back(cmdInfo);
            }
            
            // Uruchom wątki dla każdego polecenia
            std::vector<std::thread> threads;
            std::vector<bool> results(parallelCmds.size(), false);
            
            for (size_t i = 0; i < parallelCmds.size(); i++) {
                threads.emplace_back([this, &parallelCmds, &results, i]() {
                    std::stringstream paramStream(parallelCmds[i].params);
                    results[i] = ExecuteSingleCommand(
                        parallelCmds[i].cmdName,
                        parallelCmds[i].objName,
                        paramStream
                    );
                });
            }
            
            // Czekaj na zakończenie wszystkich wątków
            for (auto& t : threads) {
                if (t.joinable()) {
                    t.join();
                }
            }
            
            // Sprawdź wyniki
            for (bool result : results) {
                if (!result) {
                    allSuccess = false;
                }
            }
            
            cout << ">>> Zakończenie bloku poleceń współbieżnych <<<\n" << endl;
            
        } else {
            // === POLECENIE SEKWENCYJNE ===
            string objName;
            
            // Wczytaj nazwę obiektu (oprócz Pause)
            if (token != "Pause") {
                if (!(contentStream >> objName)) {
                    cerr << "!!! Błąd: Brak nazwy obiektu dla '" << token << "'" << endl;
                    allSuccess = false;
                    break;
                }
            }
            
            // Wykonaj polecenie sekwencyjnie
            if (!ExecuteSingleCommand(token, objName, contentStream)) {
                allSuccess = false;
            }
        }
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

