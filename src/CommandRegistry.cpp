#include "CommandRegistry.hh"
#include <iostream>
#include <iomanip>

using namespace std;

/*!
 * \brief Rejestruje wtyczkę w rejestrze
 */
bool CommandRegistry::RegisterCmd(const char* sLibFileName)
{
    // Tworzymy nowy interfejs wtyczki
    auto pLibIface = make_shared<LibInterface>();
    
    // Ładujemy wtyczkę
    if (!pLibIface->LoadPlugin(sLibFileName)) {
        cerr << "!!! Nie udało się załadować wtyczki: " << sLibFileName << endl;
        return false;
    }

    // Pobieramy nazwę polecenia
    string sCmdName = pLibIface->GetCmdName();
    
    // Sprawdzamy czy polecenie o tej nazwie już istnieje
    if (_CmdMap.find(sCmdName) != _CmdMap.end()) {
        cerr << "!!! Ostrzeżenie: Polecenie '" << sCmdName 
             << "' zostało już zarejestrowane. Nadpisuję..." << endl;
    }

    // Dodajemy do mapy
    _CmdMap[sCmdName] = pLibIface;
    
    cout << "  [+] Zarejestrowano polecenie: " << setw(10) << left 
         << sCmdName << " (" << sLibFileName << ")" << endl;
    
    return true;
}

/*!
 * \brief Wyszukuje wtyczkę po nazwie polecenia
 */
const LibInterface* CommandRegistry::FindCmd(const std::string& sCmdName) const
{
    auto it = _CmdMap.find(sCmdName);
    if (it != _CmdMap.end()) {
        return it->second.get();
    }
    return nullptr;
}

/*!
 * \brief Tworzy instancję polecenia o podanej nazwie
 */
AbstractInterp4Command* CommandRegistry::CreateCmd(const std::string& sCmdName) const
{
    const LibInterface* pLibIface = FindCmd(sCmdName);
    if (pLibIface) {
        return pLibIface->CreateCmd();
    }
    
    cerr << "!!! Błąd: Nie znaleziono polecenia '" << sCmdName << "'" << endl;
    return nullptr;
}

/*!
 * \brief Wyświetla listę zarejestrowanych poleceń
 */
void CommandRegistry::PrintRegisteredCommands() const
{
    cout << "\n=== Zarejestrowane polecenia ===" << endl;
    cout << "Liczba poleceń: " << _CmdMap.size() << endl;
    cout << "Lista poleceń:" << endl;
    
    for (const auto& pair : _CmdMap) {
        cout << "  - " << pair.first << endl;
    }
    cout << "===============================" << endl;
}
