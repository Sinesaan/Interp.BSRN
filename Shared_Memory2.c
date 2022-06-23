#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h> // for sem_init, sem_post, sem_wait, sem_destroy
#include <fcntl.h>     // for O_CREAT
#define SIZE 11

int main()
{
  int key;
  int sum = 0;
  int shmid;
  int *shm;
  int n = 10;

  key = ftok("/tmp", 'y');

  shmid = shmget(key, 0, 0600);
  if (shmid == -1)
  {
    perror("Das Segment konnte nicht erstellt werden.\n ");
    exit(EXIT_FAILURE);
  }
  const char sem1_name[] = "Semaphor1";
  const char sem2_name[] = "Semaphor2";
  int returncode_close, returncode_unlink;
  int output;

  sem_t *sem1, *sem2;

  // Das Puffern Standardausgabe (stdout) unterbinden
  setbuf(stdout, NULL);

  // Neue benannte Semaphore /mysem1 erstellen die den initialen Wert 1 hat
  sem1 = sem_open(sem1_name, O_CREAT, 0600, 1);
  if (sem1 == SEM_FAILED)
  {
    printf("Die Semaphore konnte nicht erstellt werden.\n");
    perror("sem_open");
    exit(1); // Programmabbruch
  }
  else
  {
    printf(" %s wurde erstellt.\n", sem1_name);
  }
  // Neue benannte Semaphore /mysem2 erstellen die den initialen Wert 0 hat
  sem2 = sem_open(sem2_name, O_CREAT, 0600, 0);
  if (sem2 == SEM_FAILED)
  {
    printf("Die Semaphore konnte nicht erstellt werden.\n");
    perror("sem_open");
    exit(1); // Programmabbruch
  }
  else
  {
    printf(" %s wurde erstellt.\n", sem2_name);
  }

  // Initialen Wert der Semaphore /mysem1 zur Kontrolle ausgeben
  sem_getvalue(sem1, &output);
  // printf("Wert von %s: %i\n", sem1_name, output);

  shm = shmat(shmid, NULL, 0);
  //-------------------------------------------Stat in Childprocess ------------------------------------------------->
  int stat;
  stat = fork();
  if (stat < 0)
  {
    exit(EXIT_FAILURE);
  }

  if (stat == 0)
  {

    for (int i = 0; i < 3; i++)
    {
      sem_wait(sem2); // P-Operation Semaphore /mysem2
      // Kritischer Abschnitt (Anfang)
      // Pause. Zwischen 0 und 2 Sekunden warten
      sleep(rand() % 3);
      // Kritischer Abschnitt (Ende)
      sem_post(sem1); // V-Operation Semaphore /mysem1

      for (int i = 0; i < n; i++)
      {
        shm[SIZE - 1] = shm[i] + shm[SIZE - 1]; // Summe
      }
      // }exit(1);
    }
  }

  //---------------------------------------Report in CHildprocess---------------------------------------------->
  int report = fork();
  if (report == 0)
  {
    for (int i = 0; i < 2; i++)
    {
      sem_wait(sem1); // P-Operation Semaphore /mysem1
      // Kritischer Abschnitt (Anfang)
      // Pause. Zwischen 0 und 2 Sekunden warten
      sleep(rand() % 3);
      // Kritischer Abschnitt (Ende)
      sem_post(sem2); // V-Operation Semaphore /mysem2
    }
    printf("\n\n");
    printf("***********************************\n");
    printf("Die Summe ist: %d\n", shm[SIZE - 1]);
    printf("Der Mittelwert ist: %d\n", shm[SIZE - 1] / n);
    printf("***********************************\n\n");
  }
  else
  {
    exit(0);
  }
  // wait(NULL);

  // Semaphore sem1 schliessen
  returncode_close = sem_close(sem1);
  if (returncode_close < 0)
  {
    printf("%s konnte nicht geschlossen werden.\n", sem1_name);
    exit(1); // Programmabbruch
  }
  else
  {
    printf("%s wurde geschlossen.\n", sem1_name);
  }

  // Semaphore sem2 schliessen
  returncode_close = sem_close(sem2);
  if (returncode_close < 0)
  {
    printf("%s konnte nicht geschlossen werden.\n", sem2_name);
    exit(1); // Programmabbruch
  }
  else
  {
    printf("%s wurde geschlossen.\n", sem2_name);
  }

  // Semaphore /mysem1 entfernen
  returncode_unlink = sem_unlink(sem1_name);
  if (returncode_unlink < 0)
  {
    printf("%s konnte nicht entfernt werden.\n", sem1_name);
    exit(1); // Programmabbruch
  }else{
    printf("%s wurde entfernt.\n", sem1_name);
  // Semaphore /mysem2 entfernen
  returncode_unlink = sem_unlink(sem2_name);
  if (returncode_unlink < 0)
  {
    printf("%s konnte nicht entfernt werden.\n", sem2_name);
    exit(1); // Programmabbruch
  }else{
    printf("%s wurde entfernt.\n", sem2_name);
  }
  // Gemeinsames Speichersegment lösen
  int returncode_shmdt;
  returncode_shmdt = shmdt(shm);
  if (returncode_shmdt < 0)
  {
    printf("Das Segment konnte nicht gelöst werden.\n");
    exit(1);
  }else{
    printf("Das Segment wurde vom Prozess gelöst.\n");
  }
  // // Gemeinsames Speichersegment löschen
  int returncode_shmctl;
  returncode_shmctl = shmctl(shmid, IPC_RMID, 0);
  if (returncode_shmctl == -1)
  {
    printf("Das Segment konnte nicht gelöscht werden.\n");
    perror("shmctl");
    exit(1);
  } else{
    printf("Das Segment wurde gelöscht.\n");
  }
  exit(0);
}
}
