/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Compilação: mpicc -o rvet rvet.c
 * Execução:   mpiexec -n 3 ./rvet
 */
 
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
  printf("Process: %d, Clock: (%d, %d, %d)\n", pid, clock->p[0], clock->p[1], clock->p[2]);
}

void Send(int dest_rank, Clock *clock) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  clock->p[my_rank]++;

  // Vetor auxiliar
  int clockAux[3];
  for (int i = 0; i < 3; i++) clockAux[i] = clock->p[i];

  // Utilizar MPI
  MPI_Send(clockAux, 3, MPI_INT, dest_rank, my_rank, MPI_COMM_WORLD);

  // Exibir alterações
  printf("Process: %d, Clock: (%d, %d, %d)\n", my_rank, clock->p[0], clock->p[1], clock->p[2]);
}

void Receive(Clock *clock) {
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  clock->p[my_rank]++;


  int clockE[3];
  MPI_Recv(clockE, 3, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  for (int i = 0; i < 3; i++) {
    if (clock->p[i] < clockE[i]) clock->p[i] = clockE[i];
  }

  printf("Process: %d, Clock: (%d, %d, %d)\n", my_rank, clock->p[0], clock->p[1], clock->p[2]);
}

// Representa o processo de rank 0
void process0() {
  Clock clock = {{0,0,0}};
  
  // 1, 0, 0
  Event(0, &clock);
  
  // 2, 0, 0
  Send(1, &clock);

  // 3, 1, 0
  Receive(&clock);
  
  // (4, 1, 0)
  Send(2, &clock);
  
  // 5, 1, 2
  Receive(&clock);
  
  // 6, 1, 2
  Send(1, &clock);
  
  // 7, 1, 2
  Event(0, &clock);
}

// Representa o processo de rank 1
void process1() {
  Clock clock = {{0,0,0}};
  printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
  
  // 0, 1, 0
  Send(0, &clock);

  // 2, 2, 0
  Receive(&clock);

  // 6, 3, 2
  Receive(&clock);
}

// Representa o processo de rank 2
void process2() {
  Clock clock = {{0,0,0}};
  printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
  
  // 0, 0, 1
  Event(2, &clock);

  // 0, 0, 2
  Send(0, &clock);

  // 4, 1, 3
  Receive(&clock);
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
}
