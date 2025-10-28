
#include "preprocessor.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <cctype>

// Usuwa białe znaki z początku stringa
inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// Usuwa białe znaki z końca stringa
inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// Usuwa białe znaki z obu końców stringa
inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}


std::string readFileAndPreprocess(const char* filename) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return "";
    }

    // 1. Wczytaj całą zawartość pliku do bufora
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string content = buffer.str();

    // 2. Usuń komentarze wieloliniowe
    size_t start = content.find("/*");
    while (start != std::string::npos) {
        size_t end = content.find("*/", start + 2);
        if (end == std::string::npos) {
            // Niezamknięty komentarz
            std::cerr << "Warning: Unclosed multi-line comment found." << std::endl;
            break;
        }
        content.erase(start, end - start + 2);
        // Szukaj następnego komentarza
        start = content.find("/*");
    }

    // Mapa do przechowywania makr: klucz -> wartość
    std::map<std::string, std::string> macros;
    // Strumień wyjściowy na przetworzoną zawartość
    std::stringstream processedContent;
    // Strumień wejściowy z zawartością po usunięciu komentarzy
    std::stringstream contentStream(content);
    std::string line;

    // 3. Przetwarzaj plik linia po linii
    while (std::getline(contentStream, line)) {

        // Usuń komentarze jednoliniowe (//)
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // Stwórz tymczasową wersję linii do analizy
        std::string trimmedLine = line;
        trim(trimmedLine);

        // Sprawdź, czy linia jest dyrektywą #define
        if (trimmedLine.rfind("#define", 0) == 0) {
            std::stringstream macroStream(trimmedLine);
            std::string directive, key, value;
            // Wczytaj: "#define", "KLUCZ", "WARTOŚĆ"
            macroStream >> directive >> key >> value;
            
            if (!key.empty() && !value.empty()) {
                // Zapisz makro
                macros[key] = value;
            }
            // Nie dodawaj linii z #define do wynikowego stringa
            continue;
        }

        // Zastosuj zamianę makr
        // Iteruj przez wszystkie zapisane makra
        for (const auto& pair : macros) {
            const std::string& key = pair.first;
            const std::string& value = pair.second;
            
            // Znajdź wystąpienia klucza makra w linii
            size_t pos = line.find(key);
            while (pos != std::string::npos) {
                line.replace(pos, key.length(), value);
                // Szukaj kolejnego wystąpienia (za właśnie wstawioną wartością)
                pos = line.find(key, pos + value.length());
            }
        }

        // Dodaj przetworzoną linię do wyjścia, jeśli nie jest pusta
        trim(line);
        if (!line.empty()) {
            processedContent << line << std::endl;
        }
    }

    return processedContent.str();
}