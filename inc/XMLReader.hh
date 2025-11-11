#ifndef XMLREADER_HH
#define XMLREADER_HH

/*!
 * \file
 * \brief Zawiera funkcję do wczytywania konfiguracji z pliku XML
 */

#include "Configuration.hh"

/*!
 * \brief Czyta z pliku konfigurację i wypełnia obiekt Configuration
 *
 * \param[in] sFileName - nazwa pliku z konfiguracją XML
 * \param[out] rConfig - obiekt konfiguracji do wypełnienia
 * \retval true - jeśli wczytanie zostało zrealizowane poprawnie
 * \retval false - w przeciwnym przypadku
 */
bool ReadXMLConfiguration(const char* sFileName, Configuration &rConfig);

#endif
