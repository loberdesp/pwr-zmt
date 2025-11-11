#include <xercesc/util/PlatformUtils.hpp>
#include "xmlinterp.hh"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

/*!
 * Konstruktor klasy. Tutaj należy zainicjalizować wszystkie
 * dodatkowe pola.
 */
XMLInterp4Config::XMLInterp4Config(Configuration &rConfig)
    : _rConfig(rConfig)
{
}

/*!
 * Metoda wywoływana jest bezpośrednio przed rozpoczęciem
 * przetwarzana dokumentu XML.
 */
void XMLInterp4Config::startDocument()
{
    cout << "*** Rozpoczęcie przetwarzania dokumentu XML." << endl;
}

/*!
 * Metoda wywoływana jest bezpośrednio po zakończeniu
 * przetwarzana dokumentu XML.
 */
void XMLInterp4Config::endDocument()
{
    cout << "=== Koniec przetwarzania dokumentu XML." << endl;
}

/*!
 * Analizuje atrybuty elementu XML \p "Lib" i odpowiednio je interpretuje.
 * \param[in] rAttrs - atrybuty elementu XML \p "Lib".
 */
void XMLInterp4Config::ProcessLibAttrs(const xercesc::Attributes  &rAttrs)
{
    if (rAttrs.getLength() != 1) {
        cerr << "Zla ilosc atrybutow dla \"Lib\"" << endl;
        exit(1);
    }

    char* sParamName = xercesc::XMLString::transcode(rAttrs.getQName(0));

    if (strcmp(sParamName,"Name")) {
        cerr << "Zla nazwa atrybutu dla Lib" << endl;
        exit(1);
    }         

    XMLSize_t  Size = 0;
    char* sLibName = xercesc::XMLString::transcode(rAttrs.getValue(Size));

    cout << "  Nazwa biblioteki: " << sLibName << endl;

    // Dodaj bibliotekę do konfiguracji
    _rConfig.AddPluginLibrary(sLibName);

    xercesc::XMLString::release(&sParamName);
    xercesc::XMLString::release(&sLibName);
}

/*!
 * Analizuje atrybuty. Sprawdza czy ich nazwy są poprawne. Jeśli tak,
 * to pobiera wartości atrybutów (w postaci napisów) i przekazuje ...
 * \param[in] rAttrs - atrybuty elementu XML \p "Cube".
 */
void XMLInterp4Config::ProcessCubeAttrs(const xercesc::Attributes  &rAttrs)
{
    if (rAttrs.getLength() < 1) {
        cerr << "Zla ilosc atrybutow dla \"Cube\"" << endl;
        exit(1);
    }

    CubeConfig cubeConf;  // Utworzenie obiektu z wartościami domyślnymi
    
    // Przetwarzanie wszystkich atrybutów
    for (XMLSize_t i = 0; i < rAttrs.getLength(); ++i) {
        char* sAttrName = xercesc::XMLString::transcode(rAttrs.getQName(i));
        char* sAttrValue = xercesc::XMLString::transcode(rAttrs.getValue(i));
        
        istringstream IStrm(sAttrValue);
        
        if (strcmp(sAttrName, "Name") == 0) {
            cubeConf.Name = sAttrValue;
        }
        else if (strcmp(sAttrName, "Shift") == 0) {
            IStrm >> cubeConf.Shift;
        }
        else if (strcmp(sAttrName, "Scale") == 0) {
            IStrm >> cubeConf.Scale;
        }
        else if (strcmp(sAttrName, "RotXYZ_deg") == 0) {
            IStrm >> cubeConf.RotXYZ_deg;
        }
        else if (strcmp(sAttrName, "Trans_m") == 0) {
            IStrm >> cubeConf.Trans_m;
        }
        else if (strcmp(sAttrName, "RGB") == 0) {
            IStrm >> cubeConf.RGB;
        }
        
        xercesc::XMLString::release(&sAttrName);
        xercesc::XMLString::release(&sAttrValue);
    }
    
    // Sprawdź czy nazwa została podana
    if (cubeConf.Name.empty()) {
        cerr << "!!! Błąd: Brak atrybutu Name dla Cube" << endl;
        exit(1);
    }
    
    cout << "  Obiekt: " << cubeConf.Name << " - wczytany" << endl;
    
    // Dodaj obiekt do konfiguracji
    _rConfig.AddCube(cubeConf);
}

/*!
 * Wykonuje operacje związane z wystąpieniem danego elementu XML.
 * W przypadku elementu \p "Action" będzie to utworzenie obiektu
 * reprezentującego odpowiednią działanie robota.
 * W przypadku elementu \p "Parameter" będzie to zapisanie parametrów
 * związanych z danym działaniem.
 * \param[in] rElemName - nazwa elementu XML.
 * \param[in] rAttrs - atrybuty napotkanego elementu XML.
 */
void XMLInterp4Config::WhenStartElement(const std::string           &rElemName,
                                        const xercesc::Attributes   &rAttrs)
{
    if (rElemName == "Lib") {
        ProcessLibAttrs(rAttrs);   
        return;   
    }

    if (rElemName == "Cube") {
        ProcessCubeAttrs(rAttrs);  
        return;
    }
}

/*!
 * Metoda jest wywoływana po napotkaniu nowego elementu XML
 */
void XMLInterp4Config::startElement(const XMLCh* const pURI,
                                    const XMLCh* const pLocalName,
                                    const XMLCh* const pQName,
                                    const xercesc::Attributes& rAttrs)
{
    char* sElemName = xercesc::XMLString::transcode(pLocalName);
    
    if (strcmp(sElemName, "Lib") == 0 || strcmp(sElemName, "Cube") == 0) {
        cout << "+++ Poczatek elementu: " << sElemName << endl;
        WhenStartElement(sElemName, rAttrs);
    }

    xercesc::XMLString::release(&sElemName);
}

/*!
 * Metoda zostaje wywołana w momencie zakończenia danego elementu XML
 */
void XMLInterp4Config::endElement(const XMLCh* const pURI,
                                  const XMLCh* const pLocalName,
                                  const XMLCh* const pQName)
{
    // Logika niepotrzebna
}

/*!
 * Metoda wywoływana jest, gdy napotkany zostanie błąd fatalny
 */
void XMLInterp4Config::fatalError(const xercesc::SAXParseException& rException)
{
    char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
    char* sSystemId = xercesc::XMLString::transcode(rException.getSystemId());

    cerr << "Blad fatalny! " << endl
         << "    Plik:  " << sSystemId << endl
         << "   Linia: " << rException.getLineNumber() << endl
         << " Kolumna: " << rException.getColumnNumber() << endl
         << " Informacja: " << sMessage 
         << endl;

    xercesc::XMLString::release(&sMessage);
    xercesc::XMLString::release(&sSystemId);
}

/*!
 * Metoda jest wywoływana, gdy napotkany zostanie błąd
 */
void XMLInterp4Config::error(const xercesc::SAXParseException& rException)
{
    char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
    cerr << "Blad: " << sMessage << endl;
    xercesc::XMLString::release(&sMessage);
}

/*!
 * Metoda wywołana jest w przypadku błędów, które nie są fatalne
 */
void XMLInterp4Config::warning(const xercesc::SAXParseException& rException)  
{
    char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
    cerr << "Ostrzezenie: " << sMessage << endl;
    xercesc::XMLString::release(&sMessage);
}
