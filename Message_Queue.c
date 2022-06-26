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
   

