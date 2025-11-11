#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

/*!
 * \file
 * \brief Zawiera definicję klasy Configuration
 *
 * Plik zawiera definicję klasy Configuration, która przechowuje
 * konfigurację wczytaną z pliku XML.
 */

#include "Vector3D.hh"
#include <vector>
#include <string>

/*!
 * \brief Struktura opisująca pojedynczy obiekt prostopadłościenny (Cube)
 */
struct CubeConfig {
    std::string Name;        ///< Pełna kwalifikowana nazwa obiektu
    Vector3D Shift;          ///< Przesunięcie środka lokalnego układu
    Vector3D Scale;          ///< Skala (Sx, Sy, Sz)
    Vector3D RotXYZ_deg;     ///< Kąty Roll, Pitch, Yaw w stopniach
    Vector3D Trans_m;        ///< Translacja (pozycja w metrach)
    Vector3D RGB;            ///< Kolor RGB (0-255)
    
    /*!
     * \brief Konstruktor domyślny - ustawia wartości domyślne
     */
    CubeConfig() 
    {
        Shift[0] = 0; Shift[1] = 0; Shift[2] = 0;
        Scale[0] = 1; Scale[1] = 1; Scale[2] = 1;
        RotXYZ_deg[0] = 0; RotXYZ_deg[1] = 0; RotXYZ_deg[2] = 0;
        Trans_m[0] = 0; Trans_m[1] = 0; Trans_m[2] = 0;
        RGB[0] = 128; RGB[1] = 128; RGB[2] = 128;
    }
};

/*!
 * \brief Klasa przechowująca konfigurację z pliku XML
 *
 * Klasa przechowuje listę wtyczek do załadowania oraz
 * listę obiektów prostopadłościennych ze sceny.
 */
class Configuration {
public:
    /*!
     * \brief Konstruktor domyślny
     */
    Configuration() = default;
    
    /*!
     * \brief Destruktor
     */
    ~Configuration() = default;

    /*!
     * \brief Dodaje nazwę biblioteki wtyczki do listy
     * \param[in] sLibName - nazwa pliku biblioteki (np. "libInterp4Move.so")
     */
    void AddPluginLibrary(const std::string& sLibName);
    
    /*!
     * \brief Dodaje konfigurację obiektu prostopadłościennego
     * \param[in] rCubeConf - konfiguracja obiektu
     */
    void AddCube(const CubeConfig& rCubeConf);
    
    /*!
     * \brief Zwraca listę nazw bibliotek wtyczek
     * \return Wektor nazw bibliotek
     */
    const std::vector<std::string>& GetPluginLibraries() const { 
        return _PluginLibs; 
    }
    
    /*!
     * \brief Zwraca listę konfiguracji obiektów
     * \return Wektor konfiguracji prostopadłościanów
     */
    const std::vector<CubeConfig>& GetCubes() const { 
        return _Cubes; 
    }
    
    /*!
     * \brief Wyświetla zawartość konfiguracji
     */
    void Print() const;

private:
    std::vector<std::string> _PluginLibs;  ///< Lista nazw bibliotek wtyczek
    std::vector<CubeConfig>  _Cubes;       ///< Lista konfiguracji obiektów
};

#endif
