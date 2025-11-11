#include <iostream>
#include <cctype>
#include "Interp4Rotate.hh"


using std::cout;
using std::endl;
using std::cerr;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *
 */
Interp4Rotate::Interp4Rotate(): _AngSpeed_degS(0), _Angle_deg(0), _Axis("OZ")
{}


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _Axis << " " << _AngSpeed_degS  << " " << _Angle_deg << endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd( AbstractScene      &rScn, 
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
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Czyta parametry: Os(x/y/z)  PredkoscKatowa[deg/s]  KatObrotu[deg]
   */
  std::string axis;
  double angSpeed, angle;
  
  if (!(Strm_CmdsList >> axis >> angSpeed >> angle)) {
    cerr << "!!! Błąd: Niepoprawne parametry dla polecenia Rotate" << endl;
    return false;
  }
  
  _Axis = axis;
  _AngSpeed_degS = angSpeed;
  _Angle_deg = angle;
  
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate  NazwaObiektu  Os(x/y/z)  PredkoscKatowa[deg/s]  KatObrotu[deg]" << endl;
}
