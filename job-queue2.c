#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_JOBS    10
#define NUM_THREADS 3

struct job {
  /* Link field for linked list.  */
  struct job* next;

  /* Other fields describing work to be done... */
  int id;
};

/* A linked list of pending jobs.  */
struct job* job_queue;

/* A mutex protecting job_queue.  */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* A trivial job: report which thread handled it, then simulate work.  */
void process_job (struct job* job)
{
  printf ("thread %lu: processing job %d\n",
          (unsigned long) pthread_self (), job->id);
  usleep (100000);  /* 100 ms */
}

/* Allocate a new job and push it onto the front of the queue.  */
void enqueue_job (int id)
{
  struct job* new_job = malloc (sizeof (struct job));
  if (new_job == NULL) {
    perror ("malloc");
    exit (EXIT_FAILURE);
  }
  new_job->id = id;

  pthread_mutex_lock (&job_queue_mutex);
  new_job->next = job_queue;
  job_queue = new_job;
  pthread_mutex_unlock (&job_queue_mutex);
}

/* Process queued jobs until the queue is empty.  */

void* thread_function (void* arg)
{
  (void) arg;  /* unused */
  while (1) {
    struct job* next_job;

    /* Lock the mutex on the job queue.  */
    pthread_mutex_lock (&job_queue_mutex);
    /* Now it's safe to check if the queue is empty.  */
    if (job_queue == NULL)
      next_job = NULL;
    else {
      /* Get the next available job.  */
      next_job = job_queue;
      /* Remove this job from the list.  */
      job_queue = job_queue->next;
    }
    /* Unlock the mutex on the job queue, since we're done with the
       queue for now.  */
    pthread_mutex_unlock (&job_queue_mutex);

    /* Was the queue empty?  If so, end the thread.  */
    if (next_job == NULL)
      break;

    /* Carry out the work.  */
    process_job (next_job);
    /* Clean up.  */
    free (next_job);
  }
  return NULL;
}

int main (void)
{
  pthread_t threads[NUM_THREADS];
  int i;

  /* Fill the queue BEFORE starting the threads: a worker exits as soon
     as it finds the queue empty.  */
  for (i = 1; i <= NUM_JOBS; i++)
    enqueue_job (i);

  for (i = 0; i < NUM_THREADS; i++) {
    if (pthread_create (&threads[i], NULL, thread_function, NULL) != 0) {
      fprintf (stderr, "pthread_create failed\n");
      return EXIT_FAILURE;
    }
  }

  for (i = 0; i < NUM_THREADS; i++)
    pthread_join (threads[i], NULL);

  printf ("all jobs done\n");
  return EXIT_SUCCESS;
}
