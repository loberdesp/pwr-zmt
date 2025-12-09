#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <thread>
#include <chrono>
#include "Interp4Move.hh"


using std::cout;
using std::endl;
using std::cerr;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move(): _Speed_mmS(0), _Distance_m(0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  cout << GetCmdName() << " " << _Speed_mmS  << " " << _Distance_m << endl;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Move::ExecCmd( AbstractScene      &rScn, 
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

  cout << "  [Move] Ruch obiektu: " << sMobObjName << endl;
  cout << "         Prędkość: " << _Speed_mmS << " m/s, Dystans: " << _Distance_m << " m" << endl;

  // Pobierz obecną pozycję i orientację
  Vector3D currentPos = pObj->GetPositoin_m();
  double yaw = pObj->GetAng_Yaw_deg();
  
  // Konwersja kąta Yaw na radiany
  double yawRad = yaw * M_PI / 180.0;
  
  // Oblicz przesunięcie wzdłuż kierunku Yaw (obrót wokół osi Z)
  // Yaw = 0° to kierunek +X, Yaw = 90° to kierunek +Y
  // Dystans może być ujemny (ruch do tyłu)
  double deltaX = _Distance_m * cos(yawRad);
  double deltaY = _Distance_m * sin(yawRad);
  
  // Nowa pozycja
  Vector3D newPos;
  newPos[0] = currentPos[0] + deltaX;
  newPos[1] = currentPos[1] + deltaY;
  newPos[2] = currentPos[2];  // Z bez zmians
  
  cout << "         Start: (" << currentPos[0] << ", " << currentPos[1] << ", " << currentPos[2] << ")" << endl;
  cout << "         Cel:   (" << newPos[0] << ", " << newPos[1] << ", " << newPos[2] << ")" << endl;
  
  // Symulacja ruchu - oblicz czas i ilość kroków
  // Używamy wartości bezwzględnej dystansu do obliczenia czasu
  double timeTotal_s = fabs(_Distance_m) / _Speed_mmS;  // czas całkowity w sekundach
  int steps = (int)(timeTotal_s * 10);  // 10 kroków na sekundę
  if (steps < 1) steps = 1;
  
  // Czas dla każdego kroku musi być zawsze dodatni
  double stepTime_ms = timeTotal_s * 1000.0 / steps;
  
  cout << "         Animacja: " << steps << " kroków po " << (int)stepTime_ms << "ms" << endl;
  
  // Animacja ruchu
  for (int i = 1; i <= steps; i++) {
    pObj->LockAccess();
    double t = (double)i / steps;  // parametr interpolacji [0, 1]
    
    Vector3D interpolatedPos;
    interpolatedPos[0] = currentPos[0] + deltaX * t;
    interpolatedPos[1] = currentPos[1] + deltaY * t;
    interpolatedPos[2] = currentPos[2];
    
    pObj->SetPosition_m(interpolatedPos);

    rComChann.LockAccess();
    
    // Wyślij UpdateObj do serwera
    std::ostringstream cmd;
    cmd << "UpdateObj Name=" << sMobObjName;
    cmd << " Trans_m=(" << interpolatedPos[0] << "," << interpolatedPos[1] << "," << interpolatedPos[2] << ")";
    cmd << " RotXYZ_deg=(" << pObj->GetAng_Roll_deg() << "," 
        << pObj->GetAng_Pitch_deg() << "," << pObj->GetAng_Yaw_deg() << ")\n";
    
    std::string cmdStr = cmd.str();
    write(rComChann.GetSocket(), cmdStr.c_str(), cmdStr.length());

    rComChann.UnlockAccess();
    pObj->UnlockAccess();
    
    // Czekaj między krokami
    std::this_thread::sleep_for(std::chrono::milliseconds((int)stepTime_ms));
  }
  
  cout << "  [Move] ✓ Ruch zakończony" << endl;
  
  return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Czyta parametry: Szybkosc[m/s]  DlugoscDrogi[m]
   */
  double speed, distance;
  
  if (!(Strm_CmdsList >> speed >> distance)) {
    cerr << "!!! Błąd: Niepoprawne parametry dla polecenia Move" << endl;
    return false;
  }
  
  _Speed_mmS = speed;
  _Distance_m = distance;
  
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
