#include "LibInterface.hh"
#include <iostream>
#include <dlfcn.h>

using namespace std;

/*!
 * \brief Destruktor - zamyka załadowaną bibliotekę
 */
LibInterface::~LibInterface()
{
    if (_LibHandler) {
        dlclose(_LibHandler);
        _LibHandler = nullptr;
    }
}

/*!
 * \brief Ładuje wtyczkę z pliku biblioteki współdzielonej
 */
bool LibInterface::LoadPlugin(const char* sLibFileName)
{
    // Próba załadowania biblioteki
    _LibHandler = dlopen(sLibFileName, RTLD_LAZY);
    if (!_LibHandler) {
        cerr << "!!! Błąd ładowania biblioteki: " << sLibFileName << endl;
        cerr << "    " << dlerror() << endl;
        return false;
    }

    // Pobranie wskaźnika do funkcji GetCmdName
    const char* (*pGetCmdName)() = nullptr;
    void* pFun = dlsym(_LibHandler, "GetCmdName");
    if (!pFun) {
        cerr << "!!! Nie znaleziono funkcji GetCmdName w: " << sLibFileName << endl;
        dlclose(_LibHandler);
        _LibHandler = nullptr;
        return false;
    }
    pGetCmdName = reinterpret_cast<const char* (*)()>(pFun);
    _CmdName = pGetCmdName();

    // Pobranie wskaźnika do funkcji CreateCmd
    pFun = dlsym(_LibHandler, "CreateCmd");
    if (!pFun) {
        cerr << "!!! Nie znaleziono funkcji CreateCmd w: " << sLibFileName << endl;
        dlclose(_LibHandler);
        _LibHandler = nullptr;
        return false;
    }
    _pCreateCmd = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);

    return true;
}

/*!
 * \brief Tworzy nową instancję polecenia
 */
AbstractInterp4Command* LibInterface::CreateCmd() const
{
    if (_pCreateCmd) {
        return _pCreateCmd();
    }
    return nullptr;
}
