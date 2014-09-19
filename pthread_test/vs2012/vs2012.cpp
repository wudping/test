// server_test.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"

//#include <ServerBase.h>
//#include <Server.h>
#include <iostream>
#include <cstdlib>

#include <pthread.h>

#include <Windows.h>

using namespace std;




#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count2 = 0;
int     thread_ids[3] = {0,1,2};
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t) 
{
  int i;
  long my_id = (long)t;

  for (i=0; i<TCOUNT; i++) {
    pthread_mutex_lock(&count_mutex);
    count2++;

    /* 
    Check the value of count2 and signal waiting thread when condition is
    reached.  Note that this occurs while mutex is locked. 
    */
    if (count2 == COUNT_LIMIT) {
      pthread_cond_signal(&count_threshold_cv);
      printf("inc_count(): thread %ld, count2 = %d  Threshold reached.\n", 
             my_id, count2);
      }
    printf("inc_count(): thread %ld, count2 = %d, unlocking mutex\n", 
	   my_id, count2);
    pthread_mutex_unlock(&count_mutex);

    /* Do some "work" so threads can alternate on mutex lock */
    Sleep(1);
    }
  pthread_exit(NULL);

  return	0;
}

void *watch_count(void *t) 
{
  long my_id = (long)t;

  printf("Starting watch_count(): thread %ld\n", my_id);

  /*
  Lock mutex and wait for signal.  Note that the pthread_cond_wait 
  routine will automatically and atomically unlock mutex while it waits. 
  Also, note that if COUNT_LIMIT is reached before this routine is run by
  the waiting thread, the loop will be skipped to prevent pthread_cond_wait
  from never returning. 
  */
  pthread_mutex_lock(&count_mutex);
  while (count2<COUNT_LIMIT) {
    pthread_cond_wait(&count_threshold_cv, &count_mutex);
    printf("watch_count(): thread %ld Condition signal received.\n", my_id);
    count2 += 125;
    printf("watch_count(): thread %ld count2 now = %d.\n", my_id, count2);
    }
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);

  return 0;
}








int _tmain(int argc, _TCHAR* argv[])
{
  int i, rc;
  long t1=1, t2=2, t3=3;
  pthread_t threads[3];
  pthread_attr_t attr;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_threshold_cv, NULL);

  /* For portability, explicitly create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&threads[0], &attr, watch_count, (void *)t1);
  pthread_create(&threads[1], &attr, inc_count, (void *)t2);
  pthread_create(&threads[2], &attr, inc_count, (void *)t3);

  /* Wait for all threads to complete */
  for (i=0; i<NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  
  system("PAUSE");

  pthread_exit(NULL);


	/*{
		tutk::Server	sv;
		sv.init();
		sv.run();
	}*/

	system("PAUSE");
	return 0;
}

