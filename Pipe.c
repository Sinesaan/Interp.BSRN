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
