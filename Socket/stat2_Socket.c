#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main()
{
  // ----------------------------------------------------------Childprocess----------------------------------------------------------------------->
  int stat = fork();
  if (stat == 0)
  {
    int sd;
    int portnummer;
    struct sockaddr_in adresse;
    char *ip = "127.0.0.6";

    // Inhalt des Puffers mit Null-Bytes füllen
    char puffer[1024] = {0};

    // Portnummer zuweisen
    portnummer = 1111;

    // Socket erstellen
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
      printf("Der Socket konnte nicht erzeugt werden.\n");
      exit(1);
    }
    else
    {
      printf("\nDer Socket wurde erzeugt.\n");
    }

    // Speicherbereich der Struktur sockaddr_in mit Nullen füllen
    memset(&adresse, 0, sizeof(adresse));

    // Socket-Adresse in der Struktur sockaddr_in speichern
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(portnummer);
    adresse.sin_addr.s_addr = inet_addr(ip);

    // Verbindungsanfrage an Server senden
    if (connect(sd, (struct sockaddr *)&adresse, sizeof(adresse)) < 0)
    {
      printf("Verbindungsanfrage fehlgeschlagen.\n");
      exit(1);
    }
    else
    {
      printf("Verbindung zum Server aufgebaut.\n\n");
    }

    // Inhalt des Puffers mit Null-Bytes füllen
    memset(puffer, 0, sizeof(puffer));

    // Nachricht empfangen
    if (read(sd, puffer, sizeof(puffer)) < 0)
    {
      printf("Der Lesezugriff ist fehlgeschlagen.\n");
      exit(1);
    }

    int mittelwert = 0;
    int sum = 0;
    int n = 10;

    for (int i = 0; i < n; i++)
    {
      // printf("Empfangene Nachricht: %d\n", puffer[i]);
      sum += puffer[i];
      mittelwert = sum / n;
    }
    // printf("Summe: %.d\n", sum);        //Zur Kontrolle
    // printf("Mittelwert: %.d\n", mittelwert);

    // Socket schließen
    if (close(sd) == 0)
    {
      printf("Der Socket wurde geschlossen.\n\n");
    }
    printf("Mittelwert und Summe wurden ermittelt.\n\n");

    //--------------------------------------------------------------------------------------------------------

    int neuer_socket3;
    int portnummer3;
    int clientadresselength3;
    struct sockaddr_in adressenn;

    // Das Argument nach dem Dateinamen ist die Portnummer
    portnummer3 = 3333;

    // Socket-Adresse in der Struktur sockaddr_in speichern
    adressenn.sin_family = AF_INET;
    adressenn.sin_addr.s_addr = INADDR_ANY;
    adressenn.sin_port = htons(portnummer3);

    // Neuen Socket erstellen
    int se;
    se = socket(AF_INET, SOCK_STREAM, 0);
    if (se < 0)
    {
      printf("Der Socket konnte nicht erzeugt werden.\n");
      exit(1);
    }
    else
    {
      printf("Der Socket wurde erzeugt.\n");
    }
    if (bind(se, (struct sockaddr *)&adressenn, sizeof(adressenn)) < 0)
    {
      printf("Der Port ist nicht verfügbar.\n");
      exit(1);
    }
    else
    {
      printf("Der Socket wurde an den Port gebunden.\n");
    }
    // Eine Warteschlange für bis zu 5  Verbindungsanforderungen einrichten
    if (listen(se, 5) == 0)
    {
      printf("Warte auf Verbindungsanforderungen.\n");
    }
    else
    {
      printf("Es kam beim listen zu einem Fehler.\n");
      exit(1);
    }
    clientadresselength3 = sizeof(adressenn);
    neuer_socket3 = accept(se, (struct sockaddr *)&adressenn, &clientadresselength3);
    if (neuer_socket3 < 0)
    {
      printf("Verbindungsanforderung fehlgeschlagen.\n");
      exit(1);
    }
    else
    {
      printf("Verbindung zu einem Client aufgebaut.\n");
    }

    // Summe reinschreiben
    int tmp1 = htonl(sum);
    if (write(neuer_socket3, &tmp1, sizeof(tmp1)) < 0)
    {
      printf("Der Lesezugriff ist fehlgeschlagen.\n");
      exit(1);
    }
    // Mittelwert reinschreiben
    int tmp = htonl(mittelwert);
    if (write(neuer_socket3, &tmp, sizeof(tmp)) < 0)
    {
      printf("Der Lesezugriff ist fehlgeschlagen.\n");
      exit(1);
    }
    if (write(neuer_socket3, puffer, sizeof(sum)) < 0)
    {
      printf("Der Lesezugriff ist fehlgeschlagen.\n");
      exit(1);
    }
    // Socket schließen
    if (close(neuer_socket3) == 0)
    {
      printf("Der verbundene Socket wurde geschlossen.\n");
    }

    // Socket schließen
    if (close(se) == 0)
    {
      printf("Der Socket wurde geschlossen.\n");
    }

    exit(0);
  }
  exit(0);
}
