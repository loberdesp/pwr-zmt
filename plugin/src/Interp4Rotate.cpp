#include <iostream>
#include <cctype>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <thread>
#include <chrono>
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
  // Znajdź obiekt w scenie
  AbstractMobileObj* pObj = rScn.FindMobileObj(sMobObjName);
  if (!pObj) {
    cerr << "!!! Błąd: Nie znaleziono obiektu: " << sMobObjName << endl;
    return false;
  }

  cout << "  [Rotate] Obrót obiektu: " << sMobObjName << endl;
  cout << "           Oś: " << _Axis << ", Prędkość: " << _AngSpeed_degS << " deg/s, Kąt: " << _Angle_deg << " deg" << endl;

  // Pobierz obecną orientację
  double currentRoll = pObj->GetAng_Roll_deg();
  double currentPitch = pObj->GetAng_Pitch_deg();
  double currentYaw = pObj->GetAng_Yaw_deg();
  
  // Ustal która oś jest obracana (konwersja na wielkie litery)
  std::string axis = _Axis;
  for (auto& c : axis) c = toupper(c);
  
  double startAngle = 0;
  if (axis == "OX" || axis == "X") {
    startAngle = currentRoll;
    cout << "           Roll: " << startAngle << " → " << (startAngle + _Angle_deg) << " deg" << endl;
  } else if (axis == "OY" || axis == "Y") {
    startAngle = currentPitch;
    cout << "           Pitch: " << startAngle << " → " << (startAngle + _Angle_deg) << " deg" << endl;
  } else if (axis == "OZ" || axis == "Z") {
    startAngle = currentYaw;
    cout << "           Yaw: " << startAngle << " → " << (startAngle + _Angle_deg) << " deg" << endl;
  } else {
    cerr << "!!! Błąd: Nieprawidłowa oś: " << _Axis << " (użyj OX, OY lub OZ)" << endl;
    return false;
  }
  
  // Symulacja obrotu - oblicz czas i ilość kroków
  // Używamy wartości bezwzględnej kąta do obliczenia czasu
  double timeTotal_s = fabs(_Angle_deg) / _AngSpeed_degS;  // czas całkowity w sekundach
  int steps = (int)(timeTotal_s * 10);  // 10 kroków na sekundę
  if (steps < 1) steps = 1;
  
  double stepTime_ms = fabs(timeTotal_s * 1000.0) / steps;
  
  cout << "           Animacja: " << steps << " kroków po " << (int)stepTime_ms << "ms" << endl;
  
  // Animacja obrotu
  for (int i = 1; i <= steps; i++) {
    // KROK 1: Zablokuj obiekt
    pObj->LockAccess();
    
    double t = (double)i / steps;  // parametr interpolacji [0, 1]
    
    double newAngle = startAngle + _Angle_deg * t;
    
    // Ustaw odpowiedni kąt w zależności od osi
    if (axis == "OX" || axis == "X") {
      pObj->SetAng_Roll_deg(newAngle);
    } else if (axis == "OY" || axis == "Y") {
      pObj->SetAng_Pitch_deg(newAngle);
    } else if (axis == "OZ" || axis == "Z") {
      pObj->SetAng_Yaw_deg(newAngle);
    }
    
    // KROK 2: Zablokuj kanał komunikacyjny
    rComChann.LockAccess();
    
    // Wyślij UpdateObj do serwera
    Vector3D pos = pObj->GetPositoin_m();
    std::ostringstream cmd;
    cmd << "UpdateObj Name=" << sMobObjName;
    cmd << " Trans_m=(" << pos[0] << "," << pos[1] << "," << pos[2] << ")";
    cmd << " RotXYZ_deg=(" << pObj->GetAng_Roll_deg() << "," 
        << pObj->GetAng_Pitch_deg() << "," << pObj->GetAng_Yaw_deg() << ")\n";
    
    std::string cmdStr = cmd.str();
    write(rComChann.GetSocket(), cmdStr.c_str(), cmdStr.length());
    
    // KROK 3: Odblokuj obie zasoby
    rComChann.UnlockAccess();
    pObj->UnlockAccess();
    
    // Czekaj między krokami
    std::this_thread::sleep_for(std::chrono::milliseconds((int)stepTime_ms));
  }
  
  cout << "  [Rotate] Obrót zakończony" << endl;
  
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
