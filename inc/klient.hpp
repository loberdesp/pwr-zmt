#ifndef KLIENT_HPP
#define KLIENT_HPP

/*!
 * \file
 * \brief Funkcje do komunikacji z serwerem graficznym
 *
 * Zawiera funkcje umożliwiające połączenie z serwerem graficznym
 * i wysyłanie poleceń wizualizacji obiektów 3D.
 */

/*!
 * \brief Port komunikacji z serwerem graficznym
 */
#define PORT 6217

/*!
 * \brief Otwiera połączenie sieciowe z serwerem graficznym
 *
 * Łączy się z serwerem graficznym na localhost:6217.
 * Serwer musi być uruchomiony przed wywołaniem tej funkcji.
 * 
 * \param[out] rSocket - deskryptor gniazda (wypełniany przez funkcję)
 * \retval true - połączenie udane
 * \retval false - błąd połączenia (serwer nie działa lub inny problem)
 */
bool OpenConnection(int &rSocket);

/*!
 * \brief Wysyła napis poprzez gniazdo sieciowe do serwera
 *
 * Wysyła polecenie do serwera graficznego. Polecenie musi być
 * zakończone znakiem nowej linii '\n'.
 * 
 * Przykłady poleceń:
 * - "Clear\n" - wyczyść scenę
 * - "AddObj Name=Robot1 RGB=(200,0,0) Scale=(2,2,1) Trans_m=(0,0,0)\n"
 * - "UpdateObj Name=Robot1 RotXYZ_deg=(0,45,0)\n"
 * - "Close\n" - zamknij połączenie
 * 
 * \param[in] Sk2Server - deskryptor gniazda sieciowego
 * \param[in] sMesg - napis do wysłania (musi być zakończony \0)
 * \return 0 przy sukcesie, wartość ujemna przy błędzie
 */
int Send(int Sk2Server, const char *sMesg);

#endif
