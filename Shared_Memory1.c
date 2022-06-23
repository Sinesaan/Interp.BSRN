#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#define SIZE 11

int main()
{
  int key;
  int shmid;
  int *shm;

  if ((key = ftok("/tmp", 'y')) == -1)// aus anme in ein Schlüssel bei -1 Fehlermeldung
  {
    perror("Fehler!!!\n");
    exit(EXIT_FAILURE);
  }

  shmid = shmget(key, SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid == -1)
  {
    perror("Das Segment konnte nicht erstellt werden.\n");
    exit(EXIT_FAILURE);
  }

  shm = (int *)shmat(shmid, NULL, 0);
  if (shm == (int *)-1)
  {
    perror("Das Segment konnte nicht angehängt werden. \n");
    exit(EXIT_FAILURE);
  }
  //-------------------------------------------Conv in Parentprocess---------------------------------------------->
  int conv = fork();
  if (conv > 0)
  {
    srand(time(NULL));
    int i;
    int n = 10;

    printf("Zufällige Zahlen wurden generiert: \n");
    for (i = 0; i < 10; i++)
    {
      
      shm[i] = rand() % 51;
      printf("%d ", shm[i]);
      
    }
    printf("\n\n");
  }
  else
  {
    exit(0);
  }
  //----------------------------Log Childprocess----------------------------------------------------->
  int log = fork();
  if (log == 0)
  {
    FILE *fp;
    fp = fopen("Zufällige_Zahlen_Shared_Memory.txt", "w"); // Datei namens Zufällige_Zahlen_Shared_Memory wird geöffnet
    if (fp == NULL)
    {
      printf("Die Datei konnte nicht gefunden werden. \n");
    }
    else
    {
      int i;
      int n = 10;

      for (i = 0; i < n; i++)
      {
        fprintf(fp, " %d: %d\n", i + 1, shm[i]);
      }
      printf("Zahlen wurden in der Datei gespeichert. \n\n");
    }
    fclose(fp);
  }
  else
  {
    exit(0);
  }
  //<-----------------------------------------------------------------------------------------------------

  shmdt(shm);
  return 0;
  exit(0);
}
