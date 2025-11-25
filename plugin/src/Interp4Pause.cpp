#include <iostream>
#include <thread>
#include <chrono>
#include "Interp4Pause.hh"


using std::cout;
using std::endl;
using std::cerr;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Pause"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Pause::CreateCmd();
}


/*!
 *
 */
Interp4Pause::Interp4Pause(): _Duration_ms(0)
{}


/*!
 *
 */
void Interp4Pause::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _Duration_ms << endl;
}


/*!
 *
 */
const char* Interp4Pause::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Pause::ExecCmd( AbstractScene      &rScn, 
                            const char         *sMobObjName,
			    AbstractComChannel &rComChann
			  )
{
  cout << "  [Pause] Czekam " << _Duration_ms << " ms..." << endl;
  
  // Konwersja milisekund na czas i czekanie
  std::this_thread::sleep_for(std::chrono::milliseconds((int)_Duration_ms));
  
  cout << "  [Pause] ✓ Zakończono oczekiwanie" << endl;
  
  return true;
}


/*!
 *
 */
bool Interp4Pause::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Czyta parametr: CzasPauzy[ms]
   */
  double duration;
  
  if (!(Strm_CmdsList >> duration)) {
    cerr << "!!! Błąd: Niepoprawne parametry dla polecenia Pause" << endl;
    return false;
  }
  
  _Duration_ms = duration;
  
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Pause::CreateCmd()
{
  return new Interp4Pause();
}


/*!
 *
 */
void Interp4Pause::PrintSyntax() const
{
  cout << "   Pause  CzasPauzy[ms]" << endl;
}
