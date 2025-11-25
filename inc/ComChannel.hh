#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"
#include <mutex>

/*!
 * \file
 * \brief Zawiera implementację klasy ComChannel
 *
 * Zawiera implementację klasy ComChannel, która reprezentuje
 * kanał komunikacyjny z serwerem graficznym.
 */

/*!
 * \brief Implementacja kanału komunikacyjnego
 *
 * Klasa ComChannel implementuje interfejs AbstractComChannel.
 * Przechowuje deskryptor socketu i zapewnia synchronizację dostępu.
 */
class ComChannel : public AbstractComChannel {
  int _Socket;           ///< Deskryptor gniazda sieciowego
  mutable std::mutex _Mutex;  ///< Mutex do synchronizacji dostępu

public:
  /*!
   * \brief Konstruktor domyślny
   */
  ComChannel() : _Socket(-1) {}

  /*!
   * \brief Konstruktor z deskryptorem
   * \param[in] Socket - deskryptor gniazda
   */
  ComChannel(int Socket) : _Socket(Socket) {}

  /*!
   * \brief Destruktor
   */
  virtual ~ComChannel() {}

  /*!
   * \brief Inicjalizuje deskryptor gniazda
   * \param[in] Socket - deskryptor gniazda
   */
  void Init(int Socket) override {
    _Socket = Socket;
  }

  /*!
   * \brief Udostępnia deskryptor gniazda
   * \return Deskryptor gniazda
   */
  int GetSocket() const override {
    return _Socket;
  }

  /*!
   * \brief Blokuje dostęp do gniazda
   */
  void LockAccess() override {
    _Mutex.lock();
  }

  /*!
   * \brief Odblokowuje dostęp do gniazda
   */
  void UnlockAccess() override {
    _Mutex.unlock();
  }

  /*!
   * \brief Udostępnia mutex
   * \return Referencja do mutex
   */
  std::mutex& UseGuard() override {
    return _Mutex;
  }
};

#endif
