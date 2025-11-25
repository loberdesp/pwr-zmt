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
  cout << "  Interpreter Poleceń - Etap 2" << endl;
  cout << "=====================================" << endl;

  // Sprawdź argumenty
  if (argc < 2) {
      cerr << "Usage: " << argv[0] << " <filename>" << endl;
      return 1;
  }

  // Utwórz interpreter
  ProgramInterpreter interpreter;

  // Wczytaj konfigurację XML
  if (!interpreter.Read_XML_Config("config/config.xml")) {
      cerr << "\n!!! Błąd wczytywania konfiguracji" << endl;
      return 1;
  }

  // Wykonaj program
  if (!interpreter.ExecProgram(argv[1])) {
      cerr << "\n!!! Błąd wykonywania programu" << endl;
      return 1;
  }

  cout << "\n=====================================" << endl;
  cout << "  Program zakończony pomyślnie!" << endl;
  cout << "=====================================" << endl;

  return 0;
}
