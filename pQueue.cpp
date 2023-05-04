#include "pQueue.h"
#include "eQueue.h"
#include "main.h"
#include <cstdlib>

// The comparator for the waiting priority queue
auto cmp = [](QueueNode *left, QueueNode *right)
{    if (left->priority != right->priority)
      return (left->priority) > (right->priority);
    else
      return left->arrival_time > right->arrival_time;
};
std::priority_queue<QueueNode *, std::vector<QueueNode *>, decltype(cmp)> waiting_priority_queue(cmp);

pQueue::pQueue(double muHigh, double muMedium, double muLow, int r) : Queue(r)
{
  this->muHigh = muHigh;
  this->muMedium = muMedium;
  this->muLow = muLow;
  
  last_event_pqueue = 0;
  cumulative_response_high = 0;
  cumulative_response_medium = 0;
  cumulative_response_low = 0;
  cumulative_waiting_high = 0;
  cumulative_waiting_medium = 0;
  cumulative_waiting_low = 0;
  departure_count_high = 0;
  departure_count_medium = 0;
  departure_count_low = 0;
  current_count_high = 0;
  current_count_medium = 0;
  current_count_low = 0;
  current_waiting_high = 0;
  current_waiting_low = 0;
  current_waiting_medium = 0;
  for (int k =0; k < r; k++)
  {
      free_server.push_back(k);
  }
}
// generate service time for the given node according to its priority
double pQueue::generateServiceTime(QueueNode *elementQ) 
{

  // 3 low, 2 medium, 1 high
  if (elementQ->priority == 3)
  {
    double low = possion(muLow);
    return low;
  }
  else if (elementQ->priority == 2)
  {
    double medium = possion(muMedium);
    return medium;
  }
  else
  {

    double high = possion(muHigh);
    return high;
  }
}
void pQueue::ProcessArrival(struct QueueNode *elementQ) // waiting priority queue
{
  current_number++; // Incrementing total elemnt in the system
  // 3 low, 2 medium, 1 high
  if (elementQ->priority == 3)
  {
    current_count_low++;
  }
  else if (elementQ->priority == 2)
  {
    current_count_medium++;
  }
  else
  {
    current_count_high++;
  }
  if(free_server.size() > 0)
  {
      int temp = free_server.front();
      free_server.pop_front();
      ProcessStartService(elementQ, temp);
  }
  else
  {
      current_waiting++; // waiting count
      if (elementQ->priority == 1)
        current_waiting_high++;
    else if (elementQ->priority == 2)
        current_waiting_medium++;
    else
      current_waiting_low++;
    waiting_priority_queue.push(elementQ);

  }

}
void pQueue::ProcessStartService(struct QueueNode *elementQ, int position)
{
  
  double generate_service = generateServiceTime(elementQ);
  double time = event_queue->current_time + generate_service;
  

  event_queue->InsertEventNode(3, time, elementQ, position, 2); // end service
}
void pQueue::ProcessEndService(struct QueueNode *elementQ, int position)
{
  departure_count++; // the patient leaves the room and the cleaner takes over the room
  // 3 low, 2 medium, 1 high
  if (elementQ->priority == 3)
  {
    departure_count_low++;
    current_count_low--;
  }
  else if (elementQ->priority == 2)
  {
    departure_count_medium++;
    current_count_medium--;
  }
  else
  {
    departure_count_high++;
    current_count_high--;
  }

  current_number--; // counting total elemnt in the system
  event_queue->counter--;
  // Refresh the arrival time of the node for next stage
  elementQ->arrival_time = event_queue->current_time;
  // 1 means arrival, 3 means the event is for stage3. Namely an arrival for next stage is triggered
  event_queue->InsertEventNode(1, event_queue->current_time, elementQ, position, 3); 
  
}
void pQueue::ProcessDeparture(struct QueueNode *elementQ, int position)
{
  // the cleaner has finished cleaning and the room is ready for waiting customers
  
  if (current_waiting > 0) // if a node waits in stage2, pick it from waiting queue
  {
    current_waiting--;
    QueueNode *t = waiting_priority_queue.top();
    waiting_priority_queue.pop();
     
    if (t->priority == 1)
      current_waiting_high--;
    else if (t->priority == 2)
      current_waiting_medium--;
    else
      current_waiting_low--;
    // Let next patient go into the current room
    ProcessStartService(t, position);
  }
  else 
  {
    free_server.push_back(position);
  }
}