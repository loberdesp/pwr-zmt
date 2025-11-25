#include "klient.hpp"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

bool OpenConnection(int &rSocket)
{
  struct sockaddr_in  DaneAdSerw;

  bzero((char *)&DaneAdSerw, sizeof(DaneAdSerw));

  DaneAdSerw.sin_family = AF_INET;
  DaneAdSerw.sin_addr.s_addr = inet_addr("127.0.0.1");
  DaneAdSerw.sin_port = htons(PORT);

  rSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (rSocket < 0) {
     cerr << "*** Błąd otwarcia gniazda." << endl;
     return false;
  }

  if (connect(rSocket, (struct sockaddr*)&DaneAdSerw, sizeof(DaneAdSerw)) < 0) {
     cerr << "*** Brak możliwości połączenia do portu: " << PORT << endl;
     cerr << "    (Upewnij się, że serwer graficzny jest uruchomiony)" << endl;
     return false;
   }
  
  cout << "[SIEĆ] ✓ Połączono z serwerem graficznym (port " << PORT << ")" << endl;
  return true;
}


int Send(int Sk2Server, const char *sMesg)
{
  ssize_t  IlWyslanych;
  ssize_t  IlDoWyslania = (ssize_t) strlen(sMesg);

  while ((IlWyslanych = write(Sk2Server, sMesg, IlDoWyslania)) > 0) {
    IlDoWyslania -= IlWyslanych;
    sMesg += IlWyslanych;
  }
  
  if (IlWyslanych < 0) {
    cerr << "*** Błąd przesłania napisu." << endl;
    return -1;
  }
  
  return 0;
}
