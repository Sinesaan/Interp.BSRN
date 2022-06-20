#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h> // für strcpy
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

typedef struct msgbuf
{                 // Template eines Puffers für msgsnd und msgrcv
  long mtype;     // Nachrichtentyp
  int mtext[80]; // Sendepuffer
} msg;

int main(int argc, char **argv)
{
  // Erstellung von 3 MQ
  //----------------------------------------------Die 1.MQ ----------------------------------------->
  int mq_key = 12341;            // Message Queue Key
  int rc_msgget;                 // Rückgabewert (return code) von msgget
  int rc_msgctl;                 // Rückgabewert (return code) von msgctl
  int rc_msgrcv;                 // Rückgabewert (return code) von msgrcv
  msg sendbuffer, receivebuffer; // Einen Empfangspuffer und einen Sendepuffer anlegen

  rc_msgget = msgget(mq_key, IPC_CREAT | 0600);
  if (rc_msgget < 0)
  {
    printf("Die Warteschlange konnte nicht erstellt werden.\n");
    perror("msgget");
    exit(1);
  }
  else
  {
    printf("Nachrichtenwarteschlange %i mit ID %i ist bereit.\n",
           mq_key, rc_msgget);
  }

  sendbuffer.mtype = 1; // Nachrichtentyp festlegen
//---------------------------------------------Die 2. MQ ------------------------------------------>
  int mq_key2 = 12342; // Message Queue Key
  int rc_msgget2;      // Rückgabewert (return code) von msgget2
  int rc_msgctl2;      // Rückgabewert (return code) von msgctl2
  int rc_msgrcv2;      // Rückgabewert (return code) von msgrcv2
  
  rc_msgget2 = msgget(mq_key2, IPC_CREAT | 0600);
  if (rc_msgget2 < 0)
  {
    printf("Die Warteschlange konnte nicht erstellt werden.\n");
    perror("msgget");
    exit(1);
  }
  else
  {
    printf("Nachrichtenwarteschlange %i mit ID %i ist bereit.\n",
           mq_key2, rc_msgget2);
  }

  sendbuffer.mtype = 1; // Nachrichtentyp festlegen
  
  //---------------------------------------------3.  MQ erstellen---------------------------------->
  int mq_key3 = 12343; // Message Queue Key
  int rc_msgget3;      // Rückgabewert (return code) von msgget3
  int rc_msgctl3;      // Rückgabewert (return code) von msgctl3
  int rc_msgrcv3;      // Rückgabewert (return code) von msgrcv3
  
  rc_msgget3 = msgget(mq_key3, IPC_CREAT | 0600);
  if (rc_msgget3 < 0)
  {
    printf("Die Warteschlange konnte nicht erstellt werden.\n");
    perror("msgget");
    exit(1);
  }
  else
  {
    printf("Nachrichtenwarteschlange %i mit ID %i ist bereit.\n",
           mq_key3, rc_msgget3);
  }

  sendbuffer.mtype = 1; // Nachrichtentyp festlegen
    //------------------------------Ein Elternprozess erzeugen von Conv-------------------------------->
  int conv = fork();
  if (conv > 0)
  {
    sendbuffer.mtype = 1; // Nachrichtentyp festlegen
   
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
      sendbuffer.mtext[i] = rand() % 51;
    }

    // Eine Nachricht in die Nachrichtenwarteschlange schreiben
    // (senden)
    if (msgsnd(rc_msgget,
               &sendbuffer,
               sizeof(sendbuffer.mtext),
               IPC_NOWAIT) == -1)

    {
      printf("Das Senden der Nachricht ist fehlgeschlagen.\n");
      perror("msgsnd");
      exit(1);
    }
    else
    {
    //  for (int i = 0; i < 10; i++)
    //  {
     //   printf("Geschriebene Nachricht: %i\n", sendbuffer.mtext[i]);        
     // }
    }
    if (msgsnd(rc_msgget2,
               &sendbuffer,
               sizeof(sendbuffer.mtext),
               IPC_NOWAIT) == -1)
    {
      printf("Das Senden der Nachricht ist fehlgeschlagen.\n");
      perror("msgsnd");
      exit(1);
    }
    else
    {
      for (int i = 0; i < 10; i++)
      {
        // printf("Geschriebene Nachrichten: %i\n", sendbuffer.mtext[i]);
      }
  //    printf("\n\n");

      exit(0);
    }
  }
