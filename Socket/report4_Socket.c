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
  int report = fork();
  if (report == 0)
  {

    int se;
    int portnummer3;
    struct sockaddr_in adressenn;
    char *ip = "127.0.0.6";

    // Portnummer zuweisen
    portnummer3 = 3333;

    // Socket erstellen
    se = socket(AF_INET, SOCK_STREAM, 0);
    if (se < 0)
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
    memset(&adressenn, 0, sizeof(adressenn));

    // Socket-Adresse in der Struktur sockaddr_in speichern
    adressenn.sin_family = AF_INET;
    adressenn.sin_port = htons(portnummer3);
    adressenn.sin_addr.s_addr = inet_addr(ip);

    // Verbindungsanfrage an Server senden
    if (connect(se,
                (struct sockaddr *)&adressenn,
                sizeof(adressenn)) < 0)
    {
      printf("Verbindungsanfrage fehlgeschlagen.\n");
      exit(1);
    }
    else
    {
      printf("Verbindung zum Server aufgebaut.\n\n");
    }

    int received_int1 = 0;
    int return_status1;
    return_status1 = read(se, &received_int1, sizeof(received_int1));
    if (return_status1 > 0)
    {
      fprintf(stdout, "Die Summe ist: %d\n", ntohl(received_int1));
    }
    int received_int = 0;
    int return_status;
    return_status = read(se, &received_int, sizeof(received_int));
    if (return_status > 0)
    {
      fprintf(stdout, "Der Mittelwert ist: %d\n\n", ntohl(received_int));
    }

    // Socket schließen
    if (close(se) == 0)
    {
      printf("Der Socket wurde geschlossen.\n\n");
    }

    exit(0);
  }
  exit(0);
}
