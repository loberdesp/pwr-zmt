#include <iostream>
#include <cassert>
#include "AbstractInterp4Command.hh"
#include "preprocessor.hh"
#include "CommandRegistry.hh"
#include "CommandParser.hh"
#include "Configuration.hh"
#include "XMLReader.hh"
#include <string>

using namespace std;


int main(int argc, char *argv[])
{
  cout << "\n=====================================" << endl;
  cout << "  Interpreter Poleceń - Etap 2" << endl;
  cout << "=====================================" << endl;

  ///////////////// Wczytywanie konfiguracji XML /////////////////
  
  Configuration config;
  
  cout << "\n--- Wczytywanie konfiguracji XML ---" << endl;
  if (!ReadXMLConfiguration("config/config.xml", config)) {
      cerr << "!!! Błąd wczytywania konfiguracji XML" << endl;
      return 1;
  }
  
  // Wyświetl wczytaną konfigurację
  config.Print();


  //////////////////////////// Preprocessor test ////////////////////////////
  if (argc < 2) {
      cerr << "Usage: " << argv[0] << " <filename>" << endl;
      return 1;
  }

  const char* filename = argv[1];
  string fileContent = readFileAndPreprocess(filename);
  if (fileContent.empty()) {
      cerr << "Failed to read file: " << filename << endl;
      return 1;
  }
  cout << "\n--- Zawartość pliku po preprocessingu ---" << endl;
  cout << fileContent << endl;
  cout << "----------------------------------------\n" << endl;


  ///////////////// Rejestracja wtyczek z konfiguracji XML /////////////////
  
  cout << "\n--- Ładowanie wtyczek z konfiguracji XML ---" << endl;
  
  CommandRegistry cmdRegistry;
  
  // Rejestracja wtyczek z pliku XML
  for (const auto& libName : config.GetPluginLibraries()) {
      string libPath = "./plugin/" + libName;
      cmdRegistry.RegisterCmd(libPath.c_str());
  }
  
  // Wyświetlenie zarejestrowanych poleceń
  cmdRegistry.PrintRegisteredCommands();


  ///////////////// Test rejestru poleceń /////////////////
  
  cout << "\n--- Test rejestru poleceń ---" << endl;
  
  // Lista poleceń do przetestowania
  string testCommands[] = {"Move", "Set", "Rotate", "Pause"};
  
  for (const string& cmdName : testCommands) {
      cout << "\n[TEST] Polecenie: " << cmdName << endl;
      
      AbstractInterp4Command* pCmd = cmdRegistry.CreateCmd(cmdName);
      
      if (pCmd) {
          cout << "  Nazwa: " << pCmd->GetCmdName() << endl;
          cout << "  Składnia: ";
          pCmd->PrintSyntax();
          
          delete pCmd;
      } else {
          cerr << "  !!! Błąd tworzenia polecenia" << endl;
      }
  }


  ///////////////// Parsowanie i wykonywanie poleceń z pliku /////////////////
  
  CommandParser parser(cmdRegistry);
  
  if (!parser.ParseAndExecute(fileContent)) {
      cerr << "\n!!! Wystąpiły błędy podczas parsowania pliku" << endl;
      return 1;
  }
  
  cout << "\n=====================================" << endl;
  cout << "  Program zakończony pomyślnie!" << endl;
  cout << "=====================================" << endl;

  return 0;
}
