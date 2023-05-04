#ifndef EVENTQUEUE_H // include guard
#define EVENTQUEUE_H
#include "queue.h"
#include "pQueue.h"
#include "eQueue.h"
#include "jQueue.h"
class EventQueueNode
{
public:
    int position;         // server of the event. arrival into server1, 2.etc
    int position_roomNumber; // Special for stage 3
    int stage;            // event in stage1(MMM queue) or 2 (priority queue)
    double event_time;    // event start time
    int event_type;       // Event type. 1: Arrival; 2: Start Service; 3: End service 4: Departure
    QueueNode *qnode;     // corresponding q node
    EventQueueNode *next; // pointer to next event
};
class EventQueue 
{
public:
    double cumulative_response; 
    double cumulative_waiting;
    int buffer_capacity;  // B: capacity of the entrie system
    double last_event;
    double current_time; 
    eQueue *stage1; // "E" queue stage 1
    pQueue *stage2; // "P" queue stage 2
    jQueue *stage3; // janitors ("J" queue) stage 3
    double total_cumulative_response; // The entrie system's cumulative response time
    double total_cumulative_waiting; // The entrie system's cumulative waiting time
    int counter;  // Counter for counting the number of customers in the system
    double max_time;  // Max_time allowed for running the simulation
    EventQueueNode *head; // The first event in the event list
    EventQueueNode *tail;// The last event in the event list
    EventQueue(int bufferCapacity, double max_time_arg);
    ~EventQueue();  
    // Inserts the new event node at the end of event list
    void InsertEventNode(int type, double time, QueueNode *q, int position, int stage); 
    // Specialized Overloaded version of the InsertEventNode, which will insert the new event node at the end of the list
    void InsertEventNode(int type, double time, QueueNode *q, int position, int position_roomNumber, int stage); 
    void DeleteEventNodeFromHead();
    void PrintStatistics(); // Prints the statistics each hour the program is running
    void print_helper();    // to encalsulate repeated, ugly bunch of counts
    // The main simulation program which based on the event node located at the head does the following
    // 1. Determines the event stage (which could be either stage -1 (the end of simulation), 1, 2, or 3)
    // 2. Calls the appropirate event routine based on the event type
    // 3. Deletes the event node located at the head 
    // 4. Repeates the procedure if any further events is at the event list (if we haven't still reach the max allowed
    // time for running the simulation)
    void ScheduleSimulation(eQueue *stage1, pQueue *stage2, jQueue *stage3);
};
// To return the pointer of the event node since it can't be the constructor of the event node class 
EventQueueNode *CreateEventQueueNode(int type, double time, struct QueueNode *q, int position, int stage);

#endif