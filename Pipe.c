#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

void main()
{
  // Erstellung von 3 Pipes
  int pipe1[2]; // Array der größe 2
  int pipe2[2];
  int pipe3[2];

  //----------------------------------------------Die 1. Pipe ----------------------------------------->
  if (pipe(pipe1) < 0)
  {
    printf("Das Anlegen der Pipe 1 ist fehlgeschlagen. \n");
    exit(1); // Programmabbruch
  }
  else
  {
    // printf("Die Pipe 1 wurde angelegt.\n");
  }
  //---------------------------------------------Die 2. Pipe ------------------------------------------>
  // zweite Pipe erstellen
  if (pipe(pipe2) < 0)
  {
    printf("Das Anlegen der Pipe 2 ist fehlgeschlagen. \n");
    exit(1); // Programmabbruch
  }
  else
  {
    // printf("Die Pipe 1 wurde angelegt.\n");
  }

  //---------------------------------------------3.  Pipe erstellen---------------------------------->
  if (pipe(pipe3) < 0)
  {
    printf("Das Anlegen der Pipe 3 ist fehlgeschlagen. \n");
    exit(1); // Programmabbruch
  }
  else
  {
    // printf("Die Pipe 3 wurde angelegt.\n");
  }

  //------------------------------Ein Elternprozess erzeugen von Conv-------------------------------->
  int conv = fork();
  if (conv > 0)
  {
    int n = 10;
    int arr[n];
    srand(time(NULL));

    // printf("Zufallsvariablen generiert: ");
    for (int i = 0; i < n; i++)
    {
      arr[i] = rand() % 51;
      // printf(" %d", arr[i]);  // Zur Kontrolle Kommentar Zeichen wegmachen
    }

    // Die Zufallsvariabeln in die Pipe schreiben
    write(pipe1[1], &n, sizeof(int));
    write(pipe3[1], &n, sizeof(int));

    // printf("gesendete Anzahl = %d\n", n);
    write(pipe1[1], arr, sizeof(int) * n);
    write(pipe3[1], arr, sizeof(int) * n);
    // printf("Array in die Pipe gesendet\n");
    close(pipe1[0]);
    exit(0);
  }
  //<--------------------------------------------------------------------------------------------------
  //------------------------Kindprozess von stat----------------------------------------------------->
  close(pipe1[1]);
  int stat = fork();
  if (stat == 0)
  {

    close(pipe1[1]);

    int arr[10];
    int n = 10;
    int sum = 0;
    int mittelwert = 0;

    read(pipe1[0], &n, sizeof(int));
    read(pipe1[0], arr, sizeof(int) * 10);

    for (int i = 0; i < n; i++)
    {
      sum += arr[i];
      mittelwert = sum / n;
    }
    // printf("Das Summe ist: %d\n", sum); // Zur Kontrolle
    // printf("Mittelwert ist: %d\n", mittelwert);
    // close(pipe1[0]);

    // Mittelwert und Summe in die Pipe schreiben
    write(pipe2[1], &sum, sizeof(int));
    write(pipe2[1], &mittelwert, sizeof(int));

    close(pipe2[1]);
    exit(0);
  }
  //<-----------------------------------------------------------------------------------------------
  //------------------------------Report Childprozess----------------------------------------------->
  close(pipe1[1]);
  int summe;
  int mittelwert;
  int report = fork();
  if (report == 0)
  {
    read(pipe2[0], &summe, sizeof(int));
    read(pipe2[0], &mittelwert, sizeof(int));

    printf("Die empfangene Summe: %d \n", summe);
    printf("Der empfangene Mittelwert: %d\n", mittelwert);
    printf("\n");

    close(pipe2[0]);
  }
  //<--------------------------------------------------------------------------------------------------
  //---------------------------------Childprozess von log -------------------------------------------->

  int log = fork();
  if (log == 0)
  {
    int arr[10];
    int i, n;

    read(pipe3[0], &n, sizeof(int));
    read(pipe3[0], &arr, sizeof(int) * 10);

    // printf("Die Zahlen in der Datei: %d\n", arr[i]);
    // printf("\n");
    close(pipe1[0]);

    //<-------------------------------------------------------------------------------------------
    //--------------------------Die Zahlen in eine Datei schreiben------------------------------->
    FILE *fp;
    fp = fopen("Zufällige_Zahlen_Pipes.txt", "w"); // Datei namens Zufällige_Zahlen_Pipes wird geöffnet
    if (fp == NULL)
    {
      printf("Die Datei konnte nicht gefunden werden. \n");
    }
    else
    {
      // array Liste schreiben
      for (i = 0; i < n; i++)
      {
        fprintf(fp, " %d: %d\n", i + 1, arr[i]);
      }
    }
    fclose(fp);
    close(0);
  }
  //<----------------------------------------------------------------------------------------------------
}
