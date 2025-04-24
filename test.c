#define _POSIX_C_SOURCE 200000L
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int signo, siginfo_t *info, void *_)
{
  printf("Wiadomosc otrzymana w procesie potomnym: %d", info->si_value.sival_int);
}

int main(int argc, char *argv[])
{

  if (argc != 3)
  {
    printf("Not a suitable number of program parameters\n");
    return 1;
  }

  struct sigaction action;
  action.sa_handler = &sighandler;

  //..........

  int child = fork();
  if (child == 0)
  {
    sigset_t set;
    // zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
    sigfillset(&set);
    sigdelset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    // zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
    // na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
    sigaction(SIGUSR1, &action, NULL);
  }
  else
  {
    // wyslij do procesu potomnego sygnal przekazany jako argv[2]
    // wraz z wartoscia przekazana jako argv[1]
    int val = atoi(argv[1]);
    int sig = atoi(argv[2]);
    union sigval sv;
    sv.sival_int = val;
    sigqueue(child, sig, sv);
  }

  return 0;
}
