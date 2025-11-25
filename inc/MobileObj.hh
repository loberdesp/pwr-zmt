#ifndef MOBILEOBJ_HH
#define MOBILEOBJ_HH

#include "AbstractMobileObj.hh"
#include "Configuration.hh"
#include <string>

/*!
 * \file
 * \brief Zawiera implementację klasy MobileObj
 *
 * Zawiera implementację klasy MobileObj, która reprezentuje
 * obiekt mobilny na scenie (robota).
 */

/*!
 * \brief Implementacja obiektu mobilnego
 *
 * Klasa MobileObj implementuje interfejs AbstractMobileObj.
 * Przechowuje pozycję, orientację i nazwę obiektu.
 */
class MobileObj : public AbstractMobileObj {
  std::string _name;        ///< Nazwa obiektu
  Vector3D _position;       ///< Pozycja obiektu w metrach
  double _roll_deg;         ///< Kąt Roll w stopniach (obrót wokół OX)
  double _pitch_deg;        ///< Kąt Pitch w stopniach (obrót wokół OY)
  double _yaw_deg;          ///< Kąt Yaw w stopniach (obrót wokół OZ)

public:
  /*!
   * \brief Konstruktor domyślny
   */
  MobileObj() 
    : _roll_deg(0), _pitch_deg(0), _yaw_deg(0) 
  {
    _position[0] = 0;
    _position[1] = 0;
    _position[2] = 0;
  }

  /*!
   * \brief Konstruktor z konfiguracją z XML
   *
   * \param[in] config - konfiguracja obiektu z pliku XML
   */
  MobileObj(const CubeConfig& config) 
    : _name(config.Name)
  {
    _position = config.Trans_m;
    _roll_deg = config.RotXYZ_deg[0];
    _pitch_deg = config.RotXYZ_deg[1];
    _yaw_deg = config.RotXYZ_deg[2];
  }

  /*!
   * \brief Destruktor
   */
  virtual ~MobileObj() {}

  // ===== Implementacja metod z AbstractMobileObj =====

  double GetAng_Roll_deg() const override {
    return _roll_deg;
  }

  double GetAng_Pitch_deg() const override {
    return _pitch_deg;
  }

  double GetAng_Yaw_deg() const override {
    return _yaw_deg;
  }

  void SetAng_Roll_deg(double Ang_Roll_deg) override {
    _roll_deg = Ang_Roll_deg;
  }

  void SetAng_Pitch_deg(double Ang_Pitch_deg) override {
    _pitch_deg = Ang_Pitch_deg;
  }

  void SetAng_Yaw_deg(double Ang_Yaw_deg) override {
    _yaw_deg = Ang_Yaw_deg;
  }

  const Vector3D& GetPositoin_m() const override {
    return _position;
  }

  void SetPosition_m(const Vector3D& rPos) override {
    _position = rPos;
  }

  void SetName(const char* sName) override {
    _name = sName;
  }

  const std::string& GetName() const override {
    return _name;
  }
};

#endif
