#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int size;   /* size equals the number of threads */
int *data;  /* shared data, as many ints as size */
pthread_barrier_t our_barrier; /* to synchronize */

void *fun ( void *args )
{
   int *id = (int*) args;
   int r = 1 + (rand() % 6);
   int k;
   char strd[size+1];

   sleep(r);
   printf("Hilo %d dormira %d segundos ...\n", *id, r);
   data[*id] = r;

   pthread_barrier_wait(&our_barrier);

   for(k=0; k<size; k++) strd[k] = '0' + ((char) data[k]);
   strd[size] = '\0';

   printf("Hilo %d tiene : %s\n", *id, strd);
}

int main ( int argc, char* argv[] )
{
   printf("Dar el numero de hilos: "); scanf("%d", &size);
   data = (int*) calloc(size, sizeof(int));
   {
      pthread_t t[size];
      pthread_attr_t a;
      int id[size], i;

      pthread_barrier_init(&our_barrier, NULL, size);

      for(i=0; i<size; i++)
      {
         id[i] = i;
         pthread_attr_init(&a);
         if(pthread_create(&t[i], &a, fun, (void*)&id[i]) != 0)
            printf("No se puede crear el hilo %d!\n", i);
      }
      printf("Creando %d hilos ...\n", size);
      for(i=0; i<size; i++) pthread_join(t[i], NULL);

      pthread_barrier_destroy(&our_barrier);
   }
   return 0;
}
