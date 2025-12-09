#include <iostream>
#include <sstream>
#include <unistd.h>
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
  // Znajdź obiekt w scenie
  AbstractMobileObj* pObj = rScn.FindMobileObj(sMobObjName);
  if (!pObj) {
    cerr << "!!! Błąd: Nie znaleziono obiektu: " << sMobObjName << endl;
    return false;
  }

  cout << "  [Set] Ustawianie pozycji obiektu: " << sMobObjName << endl;
  cout << "        Pozycja: (" << _PosX << ", " << _PosY << ", " << _PosZ << ")" << endl;
  cout << "        Orientacja: Roll=" << _AngRoll << "° Pitch=" << _AngPitch << "° Yaw=" << _AngYaw << "°" << endl;

  // KROK 1: Zablokuj obiekt
  pObj->LockAccess();
  
  // Ustaw nową pozycję
  Vector3D newPos;
  newPos[0] = _PosX;
  newPos[1] = _PosY;
  newPos[2] = _PosZ;
  pObj->SetPosition_m(newPos);

  // Ustaw nowe kąty orientacji
  pObj->SetAng_Roll_deg(_AngRoll);
  pObj->SetAng_Pitch_deg(_AngPitch);
  pObj->SetAng_Yaw_deg(_AngYaw);

  // KROK 2: Zablokuj kanał komunikacyjny
  rComChann.LockAccess();
  
  // Wyślij UpdateObj do serwera graficznego
  std::ostringstream cmd;
  cmd << "UpdateObj Name=" << sMobObjName;
  cmd << " Trans_m=(" << _PosX << "," << _PosY << "," << _PosZ << ")";
  cmd << " RotXYZ_deg=(" << _AngRoll << "," << _AngPitch << "," << _AngYaw << ")\n";

  std::string cmdStr = cmd.str();
  write(rComChann.GetSocket(), cmdStr.c_str(), cmdStr.length());
  
  // KROK 3: Odblokuj obie zasoby
  rComChann.UnlockAccess();
  pObj->UnlockAccess();

  cout << "  [Set] ✓ Obiekt zaktualizowany i wysłany do serwera" << endl;

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
