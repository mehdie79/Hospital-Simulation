#ifndef QUEUE_H
#define QUEUE_H
#include <queue>
#include <time.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <functional>
#include <deque>
#include <iostream>
using namespace std;

// We are using public member vars to avoid getters and setters.
// Encapsulation problems are not quite a big deal in the project.
class QueueNode
{
public:
  int index;           //start from 1
  int priority;        // 3 low, 2 medium, 1 high
  double arrival_time; // customer arrival time, measured from time t=0, inter-arrival times exponential
  double service_time; // customer service time (exponential)
  QueueNode *next;     // next element in line; NULL if this is the last element
};

// a base class for stage1/2/3 queue

class Queue
{
public:
  deque<QueueNode *> waiting_queue;
  
  int current_waiting, current_number;   // Current number of customers waiting for service/in system
  int server_number;                     // number of servers
  int total_number_patients_fullCapcity; // Number of patinets leaving the system due to the full capacity
  int departure_count;
  double cumulative_response;   // Accumulated response time for all effective departures
  double cumulative_waiting;    // Accumulated waiting time for all effective departures
  double cumulative_idle_times; // Accumulated times when the system is idle, i.e., no customers in the system
  double cumulative_number;     // Accumulated number of customers in the system
 
  Queue();
  Queue(int r); // initialize member vars

  // generate exponentially distributed number based on the given mean. (e.g. generate 0.8/1.2 with 1 passed in )
  double possion(double Lambda);
  virtual void ProcessArrival(QueueNode *elementQ) {}
  virtual void ProcessStartSerivce(QueueNode *elementQ, int position) {}
  virtual void ProcessEndService(QueueNode *elementQ, int position) {}
  virtual void ProcessDeparture(QueueNode *elementQ, int position) {}
};
// C style non-member constuctors for nodes to return ptrs to nodes on heap,which is more convenient.
//  With C++ constructor,only the node itself is returned.
QueueNode *CreateQueueNode(double arrival_time, double service_time, int priority);

#endif
