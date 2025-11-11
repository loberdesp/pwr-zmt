#include "Configuration.hh"
#include <iostream>
#include <iomanip>

using namespace std;

/*!
 * \brief Dodaje nazwę biblioteki wtyczki do listy
 */
void Configuration::AddPluginLibrary(const std::string& sLibName)
{
    _PluginLibs.push_back(sLibName);
}

/*!
 * \brief Dodaje konfigurację obiektu prostopadłościennego
 */
void Configuration::AddCube(const CubeConfig& rCubeConf)
{
    _Cubes.push_back(rCubeConf);
}

/*!
 * \brief Wyświetla zawartość konfiguracji
 */
void Configuration::Print() const
{
    cout << "\n=== Konfiguracja z pliku XML ===" << endl;
    
    cout << "\nWtyczki do załadowania (" << _PluginLibs.size() << "):" << endl;
    for (const auto& libName : _PluginLibs) {
        cout << "  - " << libName << endl;
    }
    
    cout << "\nObiekty na scenie (" << _Cubes.size() << "):" << endl;
    for (const auto& cube : _Cubes) {
        cout << "  Cube: " << cube.Name << endl;
        cout << "    Shift:      " << cube.Shift << endl;
        cout << "    Scale:      " << cube.Scale << endl;
        cout << "    RotXYZ_deg: " << cube.RotXYZ_deg << endl;
        cout << "    Trans_m:    " << cube.Trans_m << endl;
        cout << "    RGB:        " << cube.RGB << endl;
    }
    
    cout << "===============================" << endl;
}
