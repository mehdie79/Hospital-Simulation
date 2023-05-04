#ifndef EQUEUE_H // include guard
#define EQUEUE_H
#include "queue.h"

// Interface Stage 1
class eQueue : public Queue
{
public:
    // Store the generated node in the container so that they can be freed by the destructor
    deque<QueueNode*> container;
    // Stores index of existed free servers
    deque <int> free_server;

    double mu, arrivalHigh, arrivalMedium, arrivalLow, last_event_equeue;
    
    // Frees any used dynamic memory
    ~eQueue();
    eQueue(int seed, double arrivalHigh, double arrivalMedium, double arrivalLow, double mu, int r, double max_time);
    // Event routine responsible for arriving customers into the system. 
    // If the system is at the full capacity it will direct the customers into a different hospital (System).
    // If not it assign a non busy server to the customer. 
    // This routine also take the responsibility of scheduling the next arrival. 
    void ProcessArrival(struct QueueNode *elementQ); // waiting priority queue
    // It will start the service of the customer (elementQ). The customer will be assigned to the server which is 
    // determined by the passed position variable
    void ProcessStartService(struct QueueNode *elementQ, int position);
    void ProcessEndService(struct QueueNode *elementQ, int position);
    
    // 1. responsible for transitioning the customer from stage 1 to 2 by calling the arrival function
    // of stage 2
    // 2. If any customers is waiting in the waiting queue will be released and assigns to the server indicated
    // by the position
    void ProcessDeparture(struct QueueNode *elementQ, int position);
};
#endif