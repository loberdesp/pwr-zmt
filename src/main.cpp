#include <iostream>
#include "ProgramInterpreter.hh"

using namespace std;

/*!
 * \brief Główna funkcja programu
 *
 * Tworzy interpreter poleceń, wczytuje konfigurację XML
 * i wykonuje program z pliku.
 */
int main(int argc, char *argv[])
{
  cout << "\n=====================================" << endl;
  cout << "  Interpreter Poleceń" << endl;
  cout << "=====================================" << endl;

  // Sprawdź argumenty
  if (argc < 2) {
      cerr << "Usage: " << argv[0] << " [config.xml] <commands_file.txt>" << endl;
      cerr << "  config.xml - opcjonalny plik konfiguracyjny (domyślnie: config/config.xml)" << endl;
      cerr << "  commands_file.txt - plik z poleceniami do wykonania" << endl;
      return 1;
  }

  // Utwórz interpreter
  ProgramInterpreter interpreter;

  // Określ plik konfiguracyjny i plik poleceń
  const char* configFile = "config/config.xml";
  const char* commandsFile = argv[1];
  
  if (argc >= 3) {
      // Podano config i plik poleceń
      configFile = argv[1];
      commandsFile = argv[2];
  }

  // Wczytaj konfigurację XML
  if (!interpreter.Read_XML_Config(configFile)) {
      cerr << "\n!!! Błąd wczytywania konfiguracji: " << configFile << endl;
      return 1;
  }

  // Wykonaj program
  if (!interpreter.ExecProgram(commandsFile)) {
      cerr << "\n!!! Błąd wykonywania programu: " << commandsFile << endl;
      return 1;
  }

  cout << "\n=====================================" << endl;
  cout << "  Program zakończony pomyślnie!" << endl;
  cout << "=====================================" << endl;

  return 0;
}
