#ifndef PQUEUE_H // include guard
#define PQUEUE_H
#include "queue.h"

// stage 2 Interface
class pQueue : public Queue
{
public:
    // inherited from Queue and thus get all attributes and member functions from queue(e.g.)
    double muHigh, muMedium, muLow; // 3 new mu defined here
    double last_event_pqueue;

    double cumulative_response_high; 
    double cumulative_response_medium; 
    double cumulative_response_low; 
    double cumulative_waiting_high; 
    double cumulative_waiting_medium; 
    double cumulative_waiting_low; 

    int current_count_high;
    int current_count_medium;
    int current_count_low;
    int departure_count_high;
    int departure_count_medium;
    int departure_count_low;
    int current_waiting_high;
    int current_waiting_medium;
    int current_waiting_low;

    deque <int> free_server;
    // specialized constructor to construct 3 new mu attributes
    pQueue(double muHigh, double muMedium, double muLow, int r);
    double generateServiceTime(QueueNode *elementQ); // generate service time for the given node according to its priority
    // Called when new customer has arrived into the stage 2, it will follow the following procedure 
    // 1. Finds a non-busy server and calls the start service event routine
    // 2. If all the servers are busy it will place the customer in the waiting queue
    void ProcessArrival(QueueNode *elementQ);
    void ProcessStartService(QueueNode *elementQ, int position);
    // Event routine responsible for transitioning the customers from stage 2 to stage 3 by scheduling the arrival
    // of the stage 3 (requesting janitiors to come and clean the room). 
    // After the end service is finished the customer has left the hospital
    void ProcessEndService(QueueNode *elementQ, int position);
    // After process departure is finished the clean up operation should be considered finished
    void ProcessDeparture(QueueNode *elementQ, int position);
};
#endif