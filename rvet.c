//Marcus Ryller Fonseca Amado dos Santos
//Guilherme Argolo Queiroz de Freitas
//Victor Santos Chagas

#include <stdio.h>
#include <string.h>  
#include <mpi.h> 

typedef struct Clock { 
  int p[3];
} Clock;

void Event(int pid, Clock *clock) {
  clock->p[pid]++;   
}

void Send(int pidEmissor, int pidRecep, Clock *clock) {
 
  clock->p[pidEmissor]++;

 
  int clockAux[3];
  for (int i = 0; i < 3; i++) clockAux[i] = clock->p[i];

  MPI_Send(clockAux, 3, MPI_INT, pidRecep, pidEmissor, MPI_COMM_WORLD);
}

void Receive(int pidEmissor, int pidRecep, Clock *clockR) {
 
  clockR->p[pidRecep]++;


  int clockE[3];
  MPI_Recv(clockE, 3, MPI_INT, pidEmissor, pidEmissor, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  
  for (int i = 0; i < 3; i++) {
    if (clockR->p[i] < clockE[i]) clockR->p[i] = clockE[i];
  }

 
}

// Representa o processo de rank 0
void process0() {
  Clock clock = {{0,0,0}};
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 1, 0, 0
  Event(0, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 2, 0, 0
  Send(0, 1, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

  // 3, 1, 0
  Receive(1, 0, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 4, 1, 0
  Send(0, 2, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 5, 1, 2
  Receive(2, 0, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 6, 1, 2
  Send(0, 1, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 7, 1, 2
  Event(0, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 1
void process1() {
  Clock clock = {{0,0,0}};
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 0, 1, 0
  Send(1, 0, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

  // 2, 2, 0
  Receive(0, 1, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

  // 6, 3, 2
  Receive(0, 1, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 2
void process2() {
  Clock clock = {{0,0,0}};
  printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
  
  // 0, 0, 1
  Event(2, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

  // 0, 0, 2
  Send(2, 0, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

  // 4, 1, 3
  Receive(0, 2, &clock);
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

int main(void) {
  int my_rank;               

  MPI_Init(NULL, NULL); 
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

  if (my_rank == 0) { 
    process0();
  } 
  
  else if (my_rank == 1) {  
    process1();
  } 
  
  else if (my_rank == 2) {  
    process2();
  }

  // Finaliza MPI
  MPI_Finalize(); 

  return 0;
}  /* main */
