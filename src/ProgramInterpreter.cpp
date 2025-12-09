#include "ProgramInterpreter.hh"
#include "XMLReader.hh"
#include "CommandParser.hh"
#include "MobileObj.hh"
#include "preprocessor.hh"
#include "klient.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

/*!
 * \brief Konstruktor
 */
ProgramInterpreter::ProgramInterpreter()
{
}

/*!
 * \brief Wczytuje konfigurację XML
 */
bool ProgramInterpreter::Read_XML_Config(const char* FileName)
{
    cout << "\n--- Wczytywanie konfiguracji XML ---" << endl;
    
    if (!ReadXMLConfiguration(FileName, _Config)) {
        cerr << "!!! Błąd wczytywania konfiguracji XML" << endl;
        return false;
    }
    
    // Wyświetl wczytaną konfigurację
    _Config.Print();
    
    // Załaduj wtyczki z konfiguracji
    cout << "\n--- Ładowanie wtyczek z konfiguracji XML ---" << endl;
    for (const auto& libName : _Config.GetPluginLibraries()) {
        string libPath = "./plugin/" + libName;
        _LibManager.RegisterCmd(libPath.c_str());
    }
    
    _LibManager.PrintRegisteredCommands();
    
    // Utwórz scenę z obiektami z konfiguracji
    cout << "\n--- Tworzenie sceny ---" << endl;
    cout << "Dodawanie obiektów z konfiguracji do sceny..." << endl;
    for (const auto& cubeConfig : _Config.GetCubes()) {
        MobileObj* obj = new MobileObj(cubeConfig);
        _Scn.AddMobileObj(obj);
        cout << "  + Dodano obiekt: " << obj->GetName() << endl;
    }
    
    _Scn.PrintObjects();
    
    return true;
}

/*!
 * \brief Inicjalizuje połączenie z serwerem graficznym
 */
bool ProgramInterpreter::InitConnection()
{
    cout << "\n--- Połączenie z serwerem graficznym ---" << endl;
    
    int socket;
    if (!OpenConnection(socket)) {
        cerr << "!!! Błąd połączenia z serwerem graficznym!" << endl;
        return false;
    }
    
    _Chann2Serv.Init(socket);
    cout << "Połączono z serwerem (socket: " << socket << ")" << endl;
    
    return true;
}

/*!
 * \brief Wysyła obiekty ze sceny do serwera graficznego
 */
void ProgramInterpreter::SendObjectsToServer()
{
    cout << "\n--- Wysyłanie obiektów do serwera graficznego ---" << endl;
    
    for (const auto& cubeConfig : _Config.GetCubes()) {
        // Budowanie komendy AddObj
        ostringstream cmd;
        cmd << "AddObj Name=" << cubeConfig.Name;
        cmd << " RGB=(" << (int)cubeConfig.RGB[0] << "," 
            << (int)cubeConfig.RGB[1] << "," 
            << (int)cubeConfig.RGB[2] << ")";
        cmd << " Scale=(" << cubeConfig.Scale[0] << "," 
            << cubeConfig.Scale[1] << "," 
            << cubeConfig.Scale[2] << ")";
        cmd << " Shift=(" << cubeConfig.Shift[0] << "," 
            << cubeConfig.Shift[1] << "," 
            << cubeConfig.Shift[2] << ")";
        cmd << " RotXYZ_deg=(" << cubeConfig.RotXYZ_deg[0] << "," 
            << cubeConfig.RotXYZ_deg[1] << "," 
            << cubeConfig.RotXYZ_deg[2] << ")";
        cmd << " Trans_m=(" << cubeConfig.Trans_m[0] << "," 
            << cubeConfig.Trans_m[1] << "," 
            << cubeConfig.Trans_m[2] << ")\n";
        
        string cmdStr = cmd.str();
        Send(_Chann2Serv.GetSocket(), cmdStr.c_str());
        cout << "  ✓ Wysłano: " << cubeConfig.Name << endl;
    }
}

/*!
 * \brief Wykonuje program z pliku
 */
bool ProgramInterpreter::ExecProgram(const char* FileName_Prog)
{
    // Połącz się z serwerem graficznym
    if (!InitConnection()) {
        return false;
    }

    std::string clear = "Clear\n";
    write(_Chann2Serv.GetSocket(), clear.c_str(), clear.length());
    
    // Wyślij obiekty ze sceny do serwera
    SendObjectsToServer();
    
    // Wczytaj i przetwórz plik
    string fileContent = readFileAndPreprocess(FileName_Prog);
    if (fileContent.empty()) {
        cerr << "!!! Błąd wczytywania pliku: " << FileName_Prog << endl;
        return false;
    }
    
    cout << "\n--- Zawartość pliku po preprocessingu ---" << endl;
    cout << fileContent << endl;
    cout << "----------------------------------------\n" << endl;
    
    // Wykonaj polecenia
    CommandParser parser(_LibManager, _Scn, _Chann2Serv);
    
    if (!parser.ParseAndExecute(fileContent)) {
        cerr << "\n!!! Wystąpiły błędy podczas parsowania pliku" << endl;
        return false;
    }
    std::string close = "Close\n";
    write(_Chann2Serv.GetSocket(), close.c_str(), close.length());
    
    return true;
}
