#include "eventQueue.h"
#include <iostream>
using namespace std;
EventQueueNode *CreateEventNode(int type, double time, struct QueueNode *q, int position, int stage)
{
    EventQueueNode *t = (EventQueueNode *)malloc(sizeof(EventQueueNode));
    t->event_type = type;
    t->event_time = time;
    t->qnode = q;
    t->stage = stage;
    t->position = position;
    t->position_roomNumber = -1;
    t->next = NULL;
    return t;
}
EventQueue::EventQueue(int bufferCapacity, double max_time_arg)
{

    buffer_capacity = bufferCapacity;
    cumulative_response = 0;
    cumulative_waiting = 0;
    total_cumulative_response = 0;
    total_cumulative_waiting = 0;
    max_time = max_time_arg;
    counter = 0;
    current_time = 0;
    last_event = 0;
    head = NULL;
    tail = NULL;
}
void EventQueue::PrintStatistics()
{
    
    int last = (int)last_event / 60;
    int curr = (int)current_time / 60;

    if (current_time != max_time && last == curr) // neither finished nor passing an integral hour, no printing needed
        return;
    if (current_time == max_time)
    {
        printf("End of Simulation At 12 AM\n");
        print_helper();
    }
    // e.g. the simulation has run for 119 min, we get 119/60=1, which means only an integral hour has passed
    // e.g. the simulation has run for 121 min, we get 121/60=2, which means 2 integral hours has passed
    // e.g. if 1!=2, which means the current event passes the boundary of integral hours (e.g. 00:59-->01:01, 01:59-->04:03)
    else if (last != curr)
    {
        // using for loop, otherwise cannot handle events passing several hours
        // e.g. 01:59-->04:03
        for (int i = last + 1; i <= curr; i++) // e.g.  01:59-->04:03, i=2,3,4
        {
            if (curr <= 12)
                printf("At %d PM \n", i);
            else
                printf("At %d AM \n", curr - 12); // if the simulation has run for 23 hrs, we get 11AM which is correct
            print_helper();
        }
    }
}
void EventQueue::print_helper()
{
    cout << "Total number of departures =" << stage2->departure_count << " (Simulated)\n";
    cout << "Average number of patients in system = " << total_cumulative_response / current_time << " (Simulated)" << endl;
    cout << "Average response time for all patients = " << total_cumulative_response / stage2->departure_count << " (Simulated)" << endl;
    cout << "Average response time for patients of High-priority group =" << stage2->cumulative_response_high / stage2->departure_count_high << " (Simulated)" << endl;
    cout << "Average response time for patients of Medium-priority group  = " << (stage2->cumulative_response_medium) / stage2->departure_count_medium << " (Simulated)" << endl;
    cout << "Average response time for patients of Low-priority group  = " << (stage2->cumulative_response_low) / stage2->departure_count_low << " (Simulated)" << endl;
    cout << "Average waiting time in E queue = " << stage1->cumulative_waiting / stage1->departure_count << " (Simulated)" << endl;
    cout << "Average waiting time for the P queue for all patients = " << stage2->cumulative_waiting / stage2->departure_count << " (Simulated)" << endl;
    cout << "Average waiting time for patients of High-priority group = " << stage2->cumulative_waiting_high / stage2->departure_count_high << " (Simulated)" << endl;
    cout << "Average waiting time for patients of Medium-priority group = " << stage2->cumulative_waiting_medium / stage2->departure_count_medium << " (Simulated)" << endl;
    cout << "Average waiting time for patients of Low-priority group = " << stage2->cumulative_waiting_low / stage2->departure_count_low << " (Simulated)" << endl;
    cout << "Average cleanup time for each room = " << stage3->cumulative_response / stage3->departure_count << " (Simulated)" << endl;
    cout << "Number of patinets leaving the system due to full capacity = " << stage1->total_number_patients_fullCapcity << "(Simulated)" << endl;
}
void EventQueue::ScheduleSimulation(eQueue *stage1, pQueue *stage2, jQueue *stage3)
{
    this->stage1 = stage1;
    this->stage2 = stage2;
    this->stage3 = stage3;
    InsertEventNode(-1, max_time, NULL, -1, -1); // event node represents end of simulation
    Queue *current;                              // using base class ptr to achieve polymorphism behavior
        
    while (head)                                 // schedule everything in the event list till no event node is inserted
    {
        current_time = head->event_time; // move into next time point
        PrintStatistics();
        total_cumulative_response += (current_time - last_event) * (counter);
        total_cumulative_waiting += (current_time - last_event) * (stage1->current_waiting + stage2->current_waiting);
        last_event = current_time;
        
        if (head->stage == 1) // parse stage of the new time point
        {
            current = stage1;
            (stage1)->cumulative_response += (current_time - (stage1)->last_event_equeue) * ((stage1)->current_number);
            (stage1)->cumulative_waiting += (current_time - (stage1)->last_event_equeue) * ((stage1)->current_waiting);
            (stage1)->last_event_equeue = current_time;
        }
        else if (head->stage == 2)
        {
            current = stage2;

            (stage2)->cumulative_response += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_number);
            (stage2)->cumulative_waiting += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_waiting);
            (stage2)->cumulative_waiting_low += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_waiting_low);
            (stage2)->cumulative_response_low += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_count_low);
            (stage2)->cumulative_waiting_medium += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_waiting_medium);
            (stage2)->cumulative_response_medium += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_count_medium);
            (stage2)->cumulative_waiting_high += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_waiting_high);
            (stage2)->cumulative_response_high += (current_time - (stage2)->last_event_pqueue) * ((stage2)->current_count_high);
            (stage2)->last_event_pqueue = current_time;
        }
        else if (head->stage == 3)
        {
            current = stage3;
            (stage3)->cumulative_response += (current_time - (stage3)->last_event_jqueue) * ((stage3)->current_number);
            (stage3)->cumulative_waiting += (current_time - (stage3)->last_event_jqueue) * ((stage3)->current_waiting);
            (stage3)->last_event_jqueue = current_time;
        }
        else // head->stage==-1, end of simulation
        {
            // stop simulation to not count departure after deadline
            return;
        }
        if (head->event_type == 1) // parse event type
        {
            if (head->stage == 1 || head->stage == 2)
            {
                current->ProcessArrival(head->qnode); // call corresponding routine
            }
            else
            {
                ((jQueue *)current)->ProcessArrival(head->qnode, head->position);
            }
        }
        else if (head->event_type == 2)
        {
            if (head->stage != 3)
            {
                current->ProcessStartSerivce(head->qnode, head->position);
            }
            else
            {
                ((jQueue *)current)->ProcessStartService(head->qnode, head->position, head->position_roomNumber);
            }
        }
        else if (head->event_type == 3)
            current->ProcessEndService(head->qnode, head->position);
        else
        {
            if (head->stage == 2 || head->stage == 1)
                current->ProcessDeparture(head->qnode, head->position);
            else if (head->stage == 3)
                ((jQueue *)current)->ProcessDeparture(head->qnode, head->position, head->position_roomNumber);
        }
        // remove consumed node
        DeleteEventNodeFromHead();
    }
}

void EventQueue::InsertEventNode(int type, double time, struct QueueNode *q, int position, int stage)
{
    struct EventQueueNode *t = CreateEventNode(type, time, q, position, stage);
    if (head == NULL)
    {
        head = t;
        tail = t;
    }
    else
    {
        struct EventQueueNode *cursor = head;
        while (cursor->next && cursor->next->event_time <= t->event_time)
            cursor = cursor->next;
        t->next = cursor->next;
        cursor->next = t;
        if (t->next == NULL)
            tail = t;
    }
}
// Specialized version of InsertEventNode to remember to room number of stage 2 (The room that is being cleaned)
void EventQueue::InsertEventNode(int type, double time, struct QueueNode *q, int position, int position_roomNumber, int stage)
{
    struct EventQueueNode *t = CreateEventNode(type, time, q, position, stage);
    t->position_roomNumber = position_roomNumber; // Assign the room number
    if (head == NULL)
    {
        head = t;
        tail = t;
    }
    else
    {
        struct EventQueueNode *cursor = head;
        while (cursor->next && cursor->next->event_time <= t->event_time)
            cursor = cursor->next;
        t->next = cursor->next;
        cursor->next = t;
        if (t->next == NULL)
            tail = t;
    }
}
void EventQueue::DeleteEventNodeFromHead()
{
    if (head == NULL)
    {
        return;
    }
    struct EventQueueNode *l = head;
    head = head->next;
    free(l);
}
EventQueue::~EventQueue()
{
    struct EventQueueNode *cursor = head;
    while (cursor)
    {
        struct EventQueueNode *t = cursor->next;
        free(cursor);
        cursor = t;
    }
}