#include <stdio.h>      // für printf
#include <stdlib.h>     // für atoi, exit
#include <string.h>     // für memset
#include <sys/socket.h> // für socket
#include <netinet/in.h> // für die Struktur sockaddr_in
#include <unistd.h>     // für read, write, close
#include <arpa/inet.h>  // für htons
#include <time.h>       // für rand

int main(int argc, char *argv[])
{
  // ----------------------------------------------------------Elternprocess----------------------------------------------------------------------->
  int conv = fork();
  if (conv > 0)
  {
    int sd;
    int neuer_socket;
    int portnummer;
    int clientadresselength;
    struct sockaddr_in adresse;

    // Puffer
    char puffer[1024];

    // Das Argument nach dem Dateinamen ist die Portnummer
    portnummer = 1111;

    // Speicherbereich der Struktur sockaddr_in mit Nullen fÃ¼llen
    memset(&adresse, 0, sizeof(adresse));

    // Socket-Adresse in der Struktur sockaddr_in speichern
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = INADDR_ANY;
    adresse.sin_port = htons(portnummer);

    // Socket erstellen
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
      printf("Der Socket konnte nicht erzeugt werden.\n");
      exit(1);
    }
    else
    {
      printf("Der Socket wurde erzeugt.\n");
    }

    // Socket an eine Portnummer binden
    if (bind(sd, (struct sockaddr *)&adresse, sizeof(adresse)) < 0)
    {
      printf("Der Port ist nicht verfügbar.\n");
      exit(1);
    }
    else
    {
      printf("Der Socket wurde an den Port gebunden.\n");
    }

    // Eine Warteschlange für bis zu 5 Verbindungsanforderungen einrichten
    if (listen(sd, 5) == 0)
    {
      printf("Warte auf Verbindungsanforderungen.\n");
    }
    else
    {
      printf("Es kam beim listen zu einem Fehler.\n");
      exit(1);
    }

    clientadresselength = sizeof(adresse);
    neuer_socket = accept(sd, (struct sockaddr *)&adresse, &clientadresselength);
    if (neuer_socket < 0)
    {
      printf("Verbindungsanforderung fehlgeschlagen.\n");
      exit(1);
    }
    else
    {
      printf("Verbindung zu einem Client aufgebaut.\n");
    }

    //---------------- Zufällige Zahlen generieren ----------------------------------------->

    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
      puffer[i] = rand() % 51;
    }
    if (write(neuer_socket, puffer, sizeof(puffer)) < 0)
    {
      printf("Der Schreibzugriff ist fehlgeschlagen.\n");
    }
    //<---------------------------------------------------------------------------------------

    // Socket schließen
    if (close(neuer_socket) == 0)
    {
      printf("Der verbundene Socket wurde geschlossen.\n");
    }
    // Socket schließen
    if (close(sd) == 0)
    {
      printf("Der Socket wurde geschlossen.\n");
    }

    //------------------------------------------------ Neuen Socket erstellen ---------------------------------------------->
    int neuer_socket2;
    int portnummer2;
    int clientadresselength2;
    struct sockaddr_in adressen;

    //Portnummer zuweisen
    portnummer2 = 2222;

    // Socket-Adresse in der Struktur sockaddr_in speichern
    adressen.sin_family = AF_INET;
    adressen.sin_addr.s_addr = INADDR_ANY;
    adressen.sin_port = htons(portnummer2);

    // Neuen Socket erstellen
    int sc;
    sc = socket(AF_INET, SOCK_STREAM, 0);
    if (sc < 0)
    {
      printf("Der Socket konnte nicht erzeugt werden.\n");
      exit(1);
    }
    else
    {
      printf("Der Socket wurde erzeugt.\n");
    }
    if (bind(sc, (struct sockaddr *)&adressen, sizeof(adressen)) < 0)
    {
      printf("Der Port ist nicht verfügbar.\n");
      exit(1);
    }
    else
    {
      printf("Der Socket wurde an den Port gebunden.\n");
    }
    // Eine Warteschlange für bis zu 5 Verbindungsanforderungen einrichten
    if (listen(sc, 5) == 0)
    {
      printf("Warte auf Verbindungsanforderungen.\n");
    }
    else
    {
      printf("Es kam beim listen zu einem Fehler.\n");
      exit(1);
    }
    clientadresselength2 = sizeof(adressen);
    neuer_socket2 = accept(sc, (struct sockaddr *)&adressen, &clientadresselength2);
    if (neuer_socket < 0)
    {
      printf("Verbindungsanforderung fehlgeschlagen.\n");
      exit(1);
    }
    else
    {
      printf("Verbindung zu einem Client aufgebaut.\n");
    }
    if (write(neuer_socket2, puffer, sizeof(puffer)) < 0)
    {
      printf("Der Schreibzugriff ist fehlgeschlagen.\n");
    }

    // Socket (neuer_socket) schließen
    if (close(neuer_socket2) == 0)
    {
      printf("Der verbundene Socket wurde geschlossen.\n");
    }

    // Socket (sc)schließen
    if (close(sc) == 0)
    {
      printf("Der Socket wurde geschlossen.\n\n");
    }

    exit(0);
  }
  exit(0);
}
