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
  int rc_msgget;                 // RÃ¼ckgabewert (return code) von msgget
  int rc_msgctl;                 // RÃ¼ckgabewert (return code) von msgctl
  int rc_msgrcv;                 // RÃ¼ckgabewert (return code) von msgrcv
  msg sendbuffer, receivebuffer; // Einen Empfangspuffer und einen Sendepuffer anlegen

  // Nachrichtenwarteschlange erzeugen oder auf eine bestehende zugreifen
  // IPC_CREAT = Warteschlange erzeugen, wenn sie noch nicht existiert
  // 0600 = Zugriffsrechte auf die neue Nachrichtenwarteschlange
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
  int rc_msgget2;      // RÃ¼ckgabewert (return code) von msgget
  int rc_msgctl2;      // RÃ¼ckgabewert (return code) von msgctl
  int rc_msgrcv2;      // RÃ¼ckgabewert (return code) von msgrcv



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
  int rc_msgget3;      // RÃ¼ckgabewert (return code) von msgget
  int rc_msgctl3;      // RÃ¼ckgabewert (return code) von msgctl
  int rc_msgrcv3;      // RÃ¼ckgabewert (return code) von msgrcv


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
      

      exit(0);
    }
  }
  //<--------------------------------------------------------------------------------------------------
  //------------------------Kindprozess von stat----------------------------------------------------->

  int stat = fork();
  if (stat == 0)
  {

    receivebuffer.mtype = 1;

    rc_msgrcv = msgrcv(rc_msgget,
                       &receivebuffer,
                       sizeof(receivebuffer.mtext),
                       receivebuffer.mtype,
                       MSG_NOERROR | IPC_NOWAIT);
    if (rc_msgrcv < 0)
    {
      printf("Lesen der Nachricht fehlgeschlagen.\n");
      perror("msgrcv");
      exit(1);
    }
    else
    {
      // for (int i = 0; i < 10; i++)
      // {
      //   //printf("Empfangene Nachricht: %i\n", receivebuffer.mtext[i]);
      // }
    }

    int sum = 0;
    int mittelwert = 0;

    for (int i = 0; i < 10; i++)
    {
      sum += receivebuffer.mtext[i];
      mittelwert = sum / 10;
    }
    // MIttelwert Übergabe
      
      sendbuffer.mtext[11] = mittelwert;
   //  printf("Mittelwert ist %i\n", sendbuffer.mtext[11]);
      sendbuffer.mtext[14] =sum;
  //    printf("Summe ist  %d\n", sendbuffer.mtext[14]);
    
    
   // Mittelwert übergeben---------------------------------------------------------------------------------
         if (msgsnd(rc_msgget3,
                &sendbuffer,
                sizeof(sendbuffer.mtext),
                IPC_NOWAIT) == -1)
     {
       printf("Das Senden des Mittelwert ist fehlgeschlagen.\n");
       perror("msgsnd");
       exit(1);
     }
     else
     {
       
      //    printf("Geschriebene Mittelwert: %i\n", sendbuffer.mtext[11]);
      //    printf("Summe ist %d\n", sendbuffer.mtext[14]);
       
       printf("\n");

    }
    //-------------------------------------------------------------------------------------------

  //  printf("Die Summe ist: %d\n", sum);
   // printf("Mittelwert ist: %d\n", mittelwert);

    // Nachrichtenwarteschlange löschen
    rc_msgctl = msgctl(rc_msgget, IPC_RMID, 0);
    if (rc_msgctl < 0)
    {
      printf("Die Warteschlange konnte nicht gelöscht werden.\n");
      perror("msgctl");

      exit(1);
    }
    else
    {
      printf("Nachrichtenwarteschlange %i mit ID %i wurde gelöscht.\n\n", mq_key, rc_msgget);
    }

    exit(1);
  }

