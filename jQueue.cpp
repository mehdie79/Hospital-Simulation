#include "jQueue.h"
#include "main.h"
#include <iostream>
using namespace std;
jQueue::jQueue(double mu, int r) : Queue(r)
{

    this->mu = mu;
    last_event_jqueue = 0;
    for (int k =0; k < r; k++)
    {
        free_server.push_back(k);
    }
}
void jQueue::ProcessArrival(struct QueueNode *elementQ, int position_roomNumber) 
{
    current_number++; // Incrementing total element in the system
    if(free_server.size() > 0)
    {
        // Fetch a free server and give it to the current arrival room
        int temp = free_server.front();
        free_server.pop_front();
        ProcessStartService(elementQ, temp, position_roomNumber);
    }
    else
    {
        current_waiting++; 
        special_waiting_queue.push_back(make_pair(elementQ, position_roomNumber));
    }
}
void jQueue::ProcessStartService(struct QueueNode *elementQ, int position, int position_roomNumber)
{
    // schedule departure for stage 3 and passing information
    event_queue->InsertEventNode(4, event_queue->current_time + possion(mu), elementQ, position, position_roomNumber, 3); 
}
void jQueue::ProcessDeparture(struct QueueNode *elementQ, int position, int position_roomNumber)
{
    departure_count++;
    current_number--; // Decrementing total element in the system
    // Schedule departure for stage2 to free the room
    event_queue->InsertEventNode(4, event_queue->current_time, elementQ, position_roomNumber, 2); 
    if (current_waiting > 0)
    {
        current_waiting--;
        pair<QueueNode *, int> t = special_waiting_queue.front();
        special_waiting_queue.pop_front();
        ProcessStartService(t.first, position, t.second);
    }
    else
    {
        free_server.push_back(position);
    }
}