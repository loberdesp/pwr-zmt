#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"
#include "preprocessor.hh"
#include <string>

using namespace std;

#define PLUGIN_NAME__move "libInterp4Move.so"
#define PLUGIN_NAME__set "libInterp4Set.so"


int main(int argc, char *argv[])
{

  //////////////////////////// preprocessor test ////////////////////////////
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
  std::cout << "File Content:\n" << fileContent << std::endl;



////////////////////////////// move plugin test ////////////////////////////

  
  void *pLibHnd_Move = dlopen(PLUGIN_NAME__move,RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Move)(void);
  void *pFun;

  if (!pLibHnd_Move) {
    cerr << "!!! Brak biblioteki: " PLUGIN_NAME__move << endl;
    return 1;
  }


  pFun = dlsym(pLibHnd_Move,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd = pCreateCmd_Move();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;

  dlclose(pLibHnd_Move);


////////////////////////////// set plugin test ////////////////////////////


  void *pLibHnd_Set = dlopen(PLUGIN_NAME__set,RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Set)(void);
  void *pFun_set;

  if (!pLibHnd_Set) {
    cerr << "!!! Brak biblioteki: " PLUGIN_NAME__set << endl;
    return 1;
  }


  pFun_set = dlsym(pLibHnd_Set,"CreateCmd");
  if (!pFun_set) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Set = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun_set);


  AbstractInterp4Command *pCmd_Set = pCreateCmd_Set();

  cout << endl;
  cout << pCmd_Set->GetCmdName() << endl;
  cout << endl;
  pCmd_Set->PrintSyntax();
  cout << endl;
  pCmd_Set->PrintCmd();
  cout << endl;
  
  delete pCmd_Set;

  dlclose(pLibHnd_Set);
}
