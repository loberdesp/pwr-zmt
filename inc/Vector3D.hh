#ifndef VECTOR3D_HH
#define VECTOR3D_HH

#include "geomVector.hh"
#include <sstream>

/*!
 * \file
 * \brief Deklaracja instacji szablonu geom::Vector
 */

 /*!
  * \brief Instacja szablonu geom::Vector<typename,int> dla przestrzeni 3D.
  */
 typedef geom::Vector<double,3>  Vector3D;

 /*!
  * \brief Operator wczytywania Vector3D ze strumienia
  *
  * Wczytuje trzy liczby oddzielone spacjami i przypisuje je do współrzędnych wektora.
  * \param[in,out] IStrm - strumień wejściowy
  * \param[out] rVec - wektor do wypełnienia
  * \return Referencja do strumienia
  */
 inline std::istream& operator>>(std::istream& IStrm, Vector3D& rVec)
 {
     double x, y, z;
     if (IStrm >> x >> y >> z) {
         rVec[0] = x;
         rVec[1] = y;
         rVec[2] = z;
     }
     return IStrm;
 }

#endif
