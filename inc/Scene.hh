#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include <map>
#include <string>

/*!
 * \file
 * \brief Zawiera implementację klasy Scene
 *
 * Zawiera implementację klasy Scene, która przechowuje
 * kolekcję obiektów mobilnych (robotów).
 */

/*!
 * \brief Implementacja sceny zawierającej obiekty mobilne
 *
 * Klasa Scene implementuje interfejs AbstractScene.
 * Przechowuje obiekty mobilne w mapie indeksowanej nazwami obiektów.
 */
class Scene : public AbstractScene {
  /*!
   * \brief Mapa przechowująca obiekty mobilne
   *
   * Klucz: pełna kwalifikowana nazwa obiektu
   * Wartość: wskaźnik na obiekt mobilny
   */
  std::map<std::string, AbstractMobileObj*> _objects;

public:
  /*!
   * \brief Konstruktor domyślny
   */
  Scene() {}

  /*!
   * \brief Destruktor - usuwa wszystkie obiekty
   */
  ~Scene() {
    for (auto& pair : _objects) {
      delete pair.second;
    }
    _objects.clear();
  }

  /*!
   * \brief Wyszukuje obiekt o podanej nazwie
   *
   * \param[in] sName - pełna kwalifikowana nazwa obiektu
   * \return Wskaźnik na obiekt lub nullptr jeśli nie znaleziono
   */
  AbstractMobileObj* FindMobileObj(const char *sName) override {
    auto it = _objects.find(sName);
    return (it != _objects.end()) ? it->second : nullptr;
  }

  /*!
   * \brief Dodaje obiekt mobilny do sceny
   *
   * \param[in] pMobObj - wskaźnik na obiekt do dodania
   */
  void AddMobileObj(AbstractMobileObj *pMobObj) override {
    if (pMobObj) {
      _objects[pMobObj->GetName()] = pMobObj;
    }
  }

  /*!
   * \brief Zwraca liczbę obiektów w scenie
   *
   * \return Liczba obiektów w scenie
   */
  size_t GetObjectCount() const {
    return _objects.size();
  }

  /*!
   * \brief Wyświetla wszystkie obiekty w scenie
   */
  void PrintObjects() const {
    std::cout << "\n[SCENA] Obiekty w scenie (" << _objects.size() << "):" << std::endl;
    for (const auto& pair : _objects) {
      std::cout << "  - " << pair.first << std::endl;
    }
  }
};

#endif
