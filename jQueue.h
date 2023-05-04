#ifndef JQUEUE_H // include guard
#define JQUEUE_H
#include "queue.h"
#include <utility>
#include <tuple>
#include <complex>
using namespace std;

// Interface stage 3
class jQueue : public Queue
{
public:
    double mu;
    double last_event_jqueue;
    deque <int> free_server;
    // This deque stores the mapping between customer and the position of customer in stage 2
    deque <pair<QueueNode*, int>> special_waiting_queue;
    jQueue(double mu, int r);
    // Accept the customer and the room number that it leaves from so that the janitor know which room is waiting 
    // to be cleaned.
    virtual void ProcessArrival(QueueNode *elementQ, int position_roomNumber);
    // Keep remembering the position in stage 3 (janitor #) and the room number in stage 2
    virtual void ProcessStartService(QueueNode *elementQ, int position, int position_roomNumber);
    // Calls the departure of stage 2 to indicate that the room has been cleaned and its ready for next customer
    // Makes the janitor at "position" freed (non-busy) and if we have any room that is waiting in the waiting queue
    // it will be assigned to the janitor
    virtual void ProcessDeparture(QueueNode *elementQ, int position, int position_roomNumber);
};
#endif