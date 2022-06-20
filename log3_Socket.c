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
  int log = fork();
  if (log == 0)
  {
    int sc;
    int portnummer;
    struct sockaddr_in adressen;
    char *ip = "127.0.0.6";

    // Das zweite Argument nach dem Dateinamen ist die Portnummer
    portnummer = 2222;

    // Socket erstellen
    sc = socket(AF_INET, SOCK_STREAM, 0);
    if (sc < 0)
    {
      printf("Der Socket konnte nicht erzeugt werden.\n");
      exit(1);
    }
    else
    {
      printf("\nDer Socket wurde erzeugt.\n");
    }
    char puffer[1024];
    // Speicherbereich der Struktur sockaddr_in mit Nullen füllen
    memset(&adressen, 0, sizeof(adressen));

    // Socket-Adresse in der Struktur sockaddr_in speichern
    adressen.sin_family = AF_INET;
    adressen.sin_port = htons(portnummer);
    adressen.sin_addr.s_addr = inet_addr(ip);

    // Verbindungsanfrage an Server senden
    if (connect(sc,
                (struct sockaddr *)&adressen,
                sizeof(adressen)) < 0)
    {
      printf("Verbindungsanfrage fehlgeschlagen.\n");
      exit(1);
    }
    else
    {
      printf("Verbindung zum Server aufgebaut.\n\n");
    }

    // Nachricht empfangen
    if (read(sc, puffer, sizeof(puffer)) < 0)
    {
      printf("Der Lesezugriff ist fehlgeschlagen.\n");
      exit(1);
    }

    else
    {
      //---------------------------------------------Zahlen in eine Datei Schreiben----------------------------------------------------------
      FILE *fp;
      fp = fopen("Zufällige_Zahlen.txt", "w"); // Datei namens Zufällige_Zahlen wird geöffnet
      if (fp == NULL)
      {
        printf("Die Datei konnte nicht gefunden werden. \n");
      }
      else
      {
        // array Liste schreiben
        int i;
        int n = 10;
        for (i = 0; i < n; i++)
        {
          fprintf(fp, " %d: %d\n", i + 1, puffer[i]);
        }
        printf("Zahlen wurden in eine Datei gespeichert.\n\n");
      }
      fclose(fp);
      //------------------------------------------------------------------------------------------------------------------------------------------
    }

    // Socket schließen
    if (close(sc) == 0)
    {
      printf("Der Socket wurde geschlossen.\n\n");
    }

    exit(0);
  }
  exit(0);
}
