/**
 * Compilação: mpicc -o rvet_completo rvet_completo.c
 * Execução:   mpiexec -n 3 ./rvet_completo
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     
#include <time.h>

const int MAX_SIZE =3;

typedef struct Clock { 
   int p[3];
} Clock;

void swap(int* a, int* b);


void Event(int pid, Clock *clock){
   clock->p[pid]++;   
}


void Send(int my_pid,int pid, Clock *clock){
   Event(my_pid,clock);
   MPI_Send(clock->p, MAX_SIZE, MPI_INT, pid, 0, MPI_COMM_WORLD); 
   
}

void Receive(int my_pid,int pid ,Clock *clock){
   int vectorR[3];
   MPI_Recv(vectorR, MAX_SIZE, MPI_INT, pid, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   Event(my_pid,clock);
   for(int i=0;i<MAX_SIZE;i++){
      if(vectorR[i]>clock->p[i]){
         swap(&vectorR[i],&clock->p[i]);
      }
   }

}

// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Receive(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,2,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Receive(0,2,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);


}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Send(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Receive(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Receive(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);

}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Event(2, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Send(2,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Receive(2,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);

}

int main(void) {
   int my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */

void swap(int* a, int* b) {
   int t = *a;
   *a = *b;
   *b = t;
}
