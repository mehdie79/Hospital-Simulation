#include "queue.h"

QueueNode *CreateQueueNode(double arrival_time, double service_time, int priority)
{
  QueueNode *t = (QueueNode *)malloc(sizeof(QueueNode));
  t->arrival_time = arrival_time;
  t->service_time = service_time;
  t->next = NULL;
  t->priority = priority;
  return t;
}


double Queue::possion(double Lambda)
{
  double u = rand() / (RAND_MAX + 1.0);
  double l = (-log(1 - u)) / (Lambda);
  return l;
}

Queue::Queue(){}
Queue::Queue(int r)
{
  this->total_number_patients_fullCapcity = 0;
  this->cumulative_idle_times = 0;
  this->cumulative_response = 0;
  this->cumulative_waiting = 0;
  this->current_waiting = 0;
  this->current_number = 0;
  this->server_number = r;
  this->departure_count = 0;
}
