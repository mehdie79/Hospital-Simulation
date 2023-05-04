#include "pQueue.h"
#include "jQueue.h"
#include "eQueue.h"
#include "main.h"
#include <iostream>
using namespace std;
EventQueue *event_queue;

int main(int argc, char *argv[])
{
    if (argc >= 14)
    {

        double arrivalHigh = atof(argv[1]);
        double arrivalMedium = atof(argv[2]);
        double arrivalLow = atof(argv[3]);
        double muEval = atof(argv[4]);
        double muHigh = atof(argv[5]);
        double muMedium = atof(argv[6]);
        double muLow = atof(argv[7]);
        double muClean = atof(argv[8]);
        double max_system_occupancy = atoi(argv[9]);
        double pServers = atoi(argv[10]);
        double eServers = atoi(argv[11]);
        double jServers = atoi(argv[12]);
        double random_seed = atoi(argv[13]);
        double max_queue_occupancy = max_system_occupancy - pServers;  
        // Input validation
        if (arrivalHigh < 0 || arrivalMedium < 0 || arrivalLow < 0 || muEval <= 0 || muHigh <= 0 || muMedium <= 0 || muLow <= 0 || muClean <= 0 || max_system_occupancy <= 0 || pServers <= 0 || eServers <= 0 || jServers <= 0 || max_queue_occupancy <= 0)
        {
            printf("For a valid simuation, please enter values for mu,lambda, and servers that are greater than 0.\n");
            return 0;
        }

        std::cout << "Simulating Hospital System with:\nMax System Occupancy = " << max_system_occupancy << "\nMax Queue Occupancy = " << max_queue_occupancy << "\nP Servers = " << pServers << "\nE Servers = " << eServers << "\nJ Servers = " << jServers << "\nHigh Priority Arrival = " << arrivalHigh << "\nHigh Priority Mean = " << muHigh << "\nMedium Priority Arrival = " << arrivalMedium << "\nMedium Priority Mean = " << muMedium << "\nLow Priority Arrival = " << arrivalLow << "\nLow Priority Mean = " << muLow << "\nMean Evalution = " << muEval << "\nMean Clean up = " << muClean << "\nRandom_seed = " << random_seed << "\n";
        double max_time = 1440;
        // 1. Initiallize stages and event list
        EventQueue scheduler = EventQueue(max_system_occupancy, max_time);
        event_queue = &scheduler;
        // treat the node generation process at the beginning as part of the initialization of stage1
        // That's why seed and arrival rates are passed into stage1 not other stages
        // Since the number of arrival-rate and service_rate for each stage is different,
        // No common attribute of arrival rate or service rate can be abstracted into the base class queue.
        // That's why the constructor of the following derived classes looks different.
        
        eQueue stage1 = eQueue(random_seed, arrivalHigh, arrivalMedium, arrivalLow, muEval, eServers, max_time);
        pQueue stage2 = pQueue(muHigh, muMedium, muLow, pServers);
        jQueue stage3 = jQueue(muClean, jServers);
        //  2. Run Simulation
        event_queue->ScheduleSimulation(&stage1, &stage2, &stage3);
        // 3. Free Allocated Memory, should be done by destructor of eQueue
    }
    else
    {
        cout << "Insufficient number of arguments provided" << endl;
    }
    return 0;
}
