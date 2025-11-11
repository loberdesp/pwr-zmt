#include <iostream>
#include "Interp4Set.hh"


using std::cout;
using std::endl;
using std::cerr;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


/*!
 *
 */
Interp4Set::Interp4Set(): _PosX(0), _PosY(0), _PosZ(0), 
                          _AngRoll(0), _AngPitch(0), _AngYaw(0)
{}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  cout << GetCmdName() << " " << _PosX << " " << _PosY << " " << _PosZ 
       << " " << _AngRoll << " " << _AngPitch << " " << _AngYaw << endl;
}


/*!
 *
 */
const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Set::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Czyta parametry: X Y Z Roll Pitch Yaw
   */
  if (!(Strm_CmdsList >> _PosX >> _PosY >> _PosZ 
                      >> _AngRoll >> _AngPitch >> _AngYaw)) {
    cerr << "!!! Błąd: Niepoprawne parametry dla polecenia Set" << endl;
    return false;
  }
  
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set  NazwaObiektu  X[m] Y[m] Z[m]  Roll[deg] Pitch[deg] Yaw[deg]" << endl;
}
