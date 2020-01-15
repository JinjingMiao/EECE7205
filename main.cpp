

/*      Libraries to include            */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iterator>

/*      Macros                          */
#define DEBUG false                      // Prints debugging information
#define EXAMPLE 1                        // Defines which example to run: 1, 2, 3

#if EXAMPLE == 1
#define R_s 5                           // Sending rate to cloud
#define R_r 5                            // Receiving rate from cloud
#define P_1 6                            // Power to execute task on core 1
#define P_2 7                            // Power to execute task on core 2
#define P_3 8                            // Power to execute task on core 3
#define P_s 7                            // Power to send task to cloud
#endif





// define 
typedef struct Char_* Char;
typedef struct Pre_* Pre;
typedef struct task* Node;
typedef struct Graph_* Graph;



// struct 
struct Char_{
    int dst;
    Char next;
};


struct Pre_{
    int src;
    Pre nextSucc;
};


struct task{
    int uploadWeight,downloadWeight;
    int cloud, core1, core2, core3;
    int cloud_task;
    float T_re, priority;
    Char head;
    Pre otherhead;
};

struct Graph_{
    int length;
    Node array;
};



Char newNodeList(int sendingTime){
    // local
    Char newNode;
    newNode = (Char) malloc(sizeof(struct Char_));
    (*newNode).dst = sendingTime;
    (*newNode).next = NULL;
    return newNode;
}

Pre newPredeccessors(int localTime){
    Pre newSucc;
    newSucc = (Pre) malloc(sizeof(struct Pre_));
    (*newSucc).src = localTime;
    (*newSucc).nextSucc = NULL;
    return newSucc;
}

// Creates the graph
Graph createGraph(int length){
    // Local Variables
    Graph graph;
    int i;
    graph = (Graph) malloc(sizeof(struct Graph_));
    (*graph).length = length;
    (*graph).array = (Node) malloc(sizeof(struct task) * length);
    for(i = 0; i < length; i++){
        (*graph).array[i].head = NULL;
        (*graph).array[i].uploadWeight = 0;
        (*graph).array[i].downloadWeight = 0;
        (*graph).array[i].cloud = 0;
        (*graph).array[i].core1 = 0;
        (*graph).array[i].core2 = 0;
        (*graph).array[i].core3 = 0;
        (*graph).array[i].priority = 0;
    }
    return graph;
}



void addPredeccessors(Graph graph, int src, int dst){
    // Local Variables
    Pre newSucc;
    newSucc = newPredeccessors(src);
    (*newSucc).nextSucc = (*graph).array[dst].otherhead;
    (*graph).array[dst].otherhead = newSucc;
}

// Add point to graph
void addPoint(Graph graph, int src, int dst){
    Char newNode;
    newNode = newNodeList(dst);
    (*graph).array[src].head = newNode;
    (*newNode).next = (*graph).array[src].head;
    addPredeccessors(graph, src, dst);
}

// Add weight to node
void addWeight(Graph graph, int src, int upload_weight, int download_weight, int core1, int core2, int core3, int cloud_weight){
    (*graph).array[src].uploadWeight = upload_weight;
    (*graph).array[src].downloadWeight = download_weight;
    (*graph).array[src].cloud = cloud_weight;
    (*graph).array[src].core1 = core1;
    (*graph).array[src].core2 = core2;
    (*graph).array[src].core3 = core3;
    (*graph).array[src].cloud_task = 0;
    (*graph).array[src].T_re = 0;
}



/*      Utility functions               */

// Initial print 
void initialPrint(Graph graph){

    int i;
    Graph part;
    Char characteristics;

    // Print graph as adjacency list
    std::cout << std::endl;
    std::cout << "\tFigure 1: \n";
    for(i = 1; i < (*graph).length; i++){
        std::cout << "{" << i << ":";
        characteristics = (*graph).array[i].head;
        while(characteristics != NULL){
            std::cout << " " << (*characteristics).dst;
            characteristics = (*characteristics).next;
        }
        std::cout << "}  \n";
    }

    // Print task table
    std::cout << "\tTask Table\n";
    std::cout << "Task" << "\tCore1" << "\tCore2" << "\tCore3" << std::endl;
    for(i = 1; i < (*graph).length; i++){

        std::cout << i << "\t";
        std::cout << (*graph).array[i].core1 << "\t" ;
        std::cout << (*graph).array[i].core2 << "\t" ;
        std::cout << (*graph).array[i].core3 << "\n" ;
    }
}


void printTimeline(std::vector<std::vector<float> > core){
    std::vector<char> time_line;
    int flag = 0, prev_end = 0, start_time, total_time, end_time, j = 0;
    float addon = 0.0;

    std::setprecision(0);
    for(auto i : core){
        start_time = static_cast<int>(i.at(1)*100);
        total_time = static_cast<int>(i.at(2)*100);
        end_time = start_time + total_time;

        if (time_line.size() > 0 && prev_end == start_time){
           
            time_line[time_line.size() -1] = 'I';
        }

        while(j <= end_time){
            if(abs(j - start_time) <= 5){
                time_line.push_back('[');
                if(flag > 0){
                    flag--;
                }
                else{
                    std::cout << " ";
                }
            }
            else if (abs(j - end_time) <= 10){
                time_line.push_back(']');
                if(flag > 0){
                    flag--;
                }
                else {
                    std::cout << " ";
                }
            }
            else {
                time_line.push_back(' ');
                if(flag > 0){
                    flag--;
                }
                else {
                    std::cout << " ";
                }
            }

            if (abs(j - ((end_time + start_time) / 2) + 40) <= 5) {
                std::cout << std::setprecision(0) <<  "Task " << i.at(0);
                flag = 6;
            }
            j += 10;
            prev_end = end_time;
        }
    }
    std::cout << std::endl;

    // Print saved timeline
    for(auto i : time_line){
        std::cout << i;
    }
    std::cout << std::endl;


    std::cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|--------->\n";
    std::cout << "0         1         2         3         4         5         6         7         8         9         10        11        12        13        14        15        16       17         18        19        20        21        22        23        24        25        26        27        28        29 (unit time)\n\n";
}

// Print out Figure 3 scheduling
void fig3Print(std::vector<std::vector<float>> core1, std::vector<std::vector<float>> core2, std::vector<std::vector<float>> core3, std::vector<std::vector<float>> wireless_send_ch, std::vector<std::vector<float>> cloud, std::vector<std::vector<float>> wireless_recv_ch){
    // Local Variables
    std::vector<char> time_line;
    int flag;
    float addon;

    // Print start and end times of each medium
    if(DEBUG == true){
        std::cout << "\t\t\tIntermediate Execution Times\n";
        std::cout << "\t\t\t\tCore1\n";
        for(auto i : core1){
            std::cout << std::setprecision(0) << "Task: " << i.at(0) << "  \t\t";
            std::cout << std::setprecision(1) << "Start time: " << i.at(1) << "  \t\t";
            std::cout << "End time: " << i.at(2) + i.at(1) << "    \t\t";
            std::cout << "Total time: " << i.at(2)  << std::endl;
        }
        std::cout << "\t\t\t\tCore2\n";
        for(auto i : core2){
            std::cout << std::setprecision(0) << "Task: " << i.at(0) << "  \t\t";
            std::cout << std::setprecision(1) << "Start time: " << i.at(1) << "  \t\t";
            std::cout << "End time: " << i.at(2) + i.at(1) << "    \t\t";
            std::cout << "Total time: " << i.at(2)  << std::endl;
        }
        std::cout << "\t\t\t\tCore3\n";
        for(auto i : core3){
            std::cout << std::setprecision(0) << "Task: " << i.at(0) << "  \t\t";
            std::cout << std::setprecision(1) << "Start time: " << i.at(1) << "  \t\t";
            std::cout << "End time: " << i.at(2) + i.at(1) << "    \t\t";
            std::cout << "Total time: " << i.at(2)  << std::endl;
        }
        std::cout << "\t\t\t\tWireless Sending Channel\n";
        for(auto i : wireless_send_ch){
            std::cout << std::setprecision(0) << "Task: " << i.at(0) << "  \t\t";
            std::cout << std::setprecision(1) << "Start time: " << i.at(1) << "  \t\t";
            std::cout << "End time: " << i.at(2) + i.at(1) << "    \t\t";
            std::cout << "Total time: " << i.at(2)  << std::endl;
        }
        std::cout << "\t\t\t\tCloud\n";
        for(auto i : cloud){
            std::cout << std::setprecision(0) << "Task: " << i.at(0) << "  \t\t";
            std::cout << std::setprecision(1) << "Start time: " << i.at(1) << "  \t\t";
            std::cout << "End time: " << i.at(2) + i.at(1) << "    \t\t";
            std::cout << "Total time: " << i.at(2)  << std::endl;
        }
        std::cout << "\t\t\t\tWireless Receiving Channel\n";
        for(auto i : wireless_recv_ch){
            std::cout << std::setprecision(0) << "Task: " << i.at(0) << "  \t\t";
            std::cout << std::setprecision(1) << "Start time: " << i.at(1) << "  \t\t";
            std::cout << "End time: " << i.at(2) + i.at(1) << "    \t\t";
            std::cout << "Total time: " << i.at(2)  << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "\n\tFigure 3: Task scheduling result by the initial scheduling algorithm\n";
    std::cout << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";


    addon = 0.0;
    flag = 0;

    // For each task selected on core 1
    std::cout << "\t\t\t\t\t\t\t\tCore 1 Timeline\n";
    printTimeline(core1);

    // For each task selected on core 2
    std::cout << "\t\t\t\t\t\t\t\tCore 2 Timeline\n";
    printTimeline(core2);

    // For each task selected on core 3
    std::cout << "\t\t\t\t\t\t\t\tCore 3 Timeline\n";
    printTimeline(core3);

    // For each task selected on Wireless Send Channel
    std::cout << "\t\t\t\t\t\t\t\tWireless Send Timeline\n";
    printTimeline(wireless_send_ch);

    // For each task selected on Wireless Cloud Channel
    std::cout << "\t\t\t\t\t\t\t\tCloud Timeline\n";
    printTimeline(cloud);

    // For each task selected on Wireless Receive Channel
    std::cout << "\t\t\t\t\t\t\t\tWireless Receive Timeline\n";
    printTimeline(wireless_recv_ch);

    std::cout << "=====================================================================================================================================================================\n\n";
}


bool sortSecond(const std::pair<int,float> &a, const std::pair<int,float> &b){
    return (a.second < b.second);
}


float max_ele(Graph graph, int i){
    // Local varaibles
    float max = 0;
    int count = 0;
    Char characteristics;

    // Compare successors
    characteristics = (*graph).array[i].head;
    while(characteristics != NULL){
        if(count == 0){
            count++;
            max = (*graph).array[(*characteristics).dst].priority;
        }
        else{
            if((*graph).array[(*characteristics).dst].priority > max){
                max = (*graph).array[(*characteristics).dst].priority;
            }
        }
        characteristics = (*characteristics).next;
    }

    return max;
}


int primary_assign(Graph graph, int i){
    // Local Variables
    float T_l, T_re, T_c, T_s, T_r;
    Char characteristics;

    // Determine minimum execution time locally
    T_l = (*graph).array[i].core1;
    if((*graph).array[i].core2 < T_l){
        T_l = (*graph).array[i].core2;
    }
    if((*graph).array[i].core3 < T_l){
        T_l = (*graph).array[i].core3;
    }

    // Determine remove execution time
    T_s = (float) (*graph).array[i].uploadWeight / (float) R_s;
    T_r = (float) (*graph).array[i].downloadWeight / (float) R_r;
    T_c = (float) (*graph).array[i].cloud;
    T_re = T_s + T_r + T_c;
    (*graph).array[i].T_re = T_re;
    std::cout << std::fixed << std::setprecision(1) << i << "\t" << T_s << "\t" << T_c << "\t" << T_r << "\t" << T_re << std::endl;

    // Compare local versus remote execution speeds
    if(T_re < T_l){

        // Return 1 for remote execution
        (*graph).array[i].cloud_task = 1;
        return 1;
    }
    else{

        // Return 2 for local execution
        return 0;
    }
}

// Caculate the priority of each task, similar to HEFT algorithm
float task_priority(Graph graph, int i, int cloud_task){
    // Local Variables
    float weight, priority;

    // If cloud task, weight = T_re
    if(cloud_task == 1){
        weight = ((float) (*graph).array[i].uploadWeight / (float) R_s) + ((float) (*graph).array[i].downloadWeight / (float) R_r) + ((float) (*graph).array[i].cloud);
    }

        // Else if not a cloud task, weight = average of local execution
    else{
        weight = (((float) (*graph).array[i].core1) + ((float) (*graph).array[i].core2) + ((float) (*graph).array[i].core3)) / (float) 3;
    }

    // Return weight for priority calculations
    return weight;
}

// Select tasks and schedule in descending order of priorities
void selection(Graph graph){
    // Local Variables
    Pre predeccessors;
    int i, index, final_i, task, min, j;
    float max, t_1_availability = 0, t_2_availability = 0, t_3_availability = 0, t_cloud_availability = 0, T_s, T_c, T_r, send_start_time, min_start_time = 0;
    float exec_time[4];

    std::vector<int>::iterator it;
    std::vector<int> queue;
    std::vector<std::pair<int, float>> priority_queue;
    std::vector<std::vector<float>> core1;
    std::vector<std::vector<float>> core2;
    std::vector<std::vector<float>> core3;
    std::vector<std::vector<float>> wireless_send_ch;
    std::vector<std::vector<float>> wireless_recv_ch;
    std::vector<std::vector<float>> cloud;

    // Sort priority array by second element such that most expensive task is first
    // Initialize queue
    for(i = 1; i < (*graph).length; i++){
        queue.push_back(i);
    }
    while(queue.size() != 0){
        max = 0;
        for(i = 1; i < (*graph).length; i++){
            // if priority is max and i exists within queue, return new max and index to be removed from queue
            if((*graph).array[i].priority > max && (std::find(queue.begin(), queue.end(), i) != queue.end())){
                max = (*graph).array[i].priority;
                it = std::find(queue.begin(), queue.end(), i);
                index = std::distance(queue.begin(), it);
                final_i = i;
            }
        }
        // Pop value from queue and add value to priority queue
        queue.erase(queue.begin() + index,queue.begin() + index + 1);
        priority_queue.push_back(std::make_pair(final_i, max));
    }

    if(DEBUG == true){
        std::cout << "\t\t\tIntermediate Priority Task List" << std::endl;
        std::cout << "Task" << "\tCloud Task" << "\tPriority" << "\tT_l_1" << "\tT_l_2" << "\tT_l_3" << "\tT_re" << "\tPredeccessors\n";
        for(auto i : priority_queue){
            index = i.first;
            std::cout << index << "\t" << (*graph).array[index].cloud_task << "\t\t";
            std::cout << (*graph).array[index].priority << "\t\t";
            std::cout << (*graph).array[index].core1 << "\t" << (*graph).array[index].core2 << "\t";
            std::cout << (*graph).array[index].core3 << "\t" << (*graph).array[index].T_re << "\t";
            predeccessors = (*graph).array[index].otherhead;
            while(predeccessors != NULL){
                std::cout << (*predeccessors).src << " ";
                predeccessors = (*predeccessors).nextSucc;
            }
            std::cout << std::endl;
        }

    }

    // Scheduler
    for(auto i : priority_queue){
        min_start_time = 0;
        task = i.first;

        // Find max predeccessor
        predeccessors = (*graph).array[task].otherhead;
        while(predeccessors != NULL){
            // Search for max time of predeccessors
            // Search core1
            for(auto j : core1){
                if((*predeccessors).src == j.at(0)){
                    if(min_start_time < j.at(1) + j.at(2)){
                        min_start_time = j.at(1) + j.at(2);
                    }
                }
            }
            // Search core2
            for(auto j : core2){
                if((*predeccessors).src == j.at(0)){
                    if(min_start_time < j.at(1) + j.at(2)){
                        min_start_time = j.at(1) + j.at(2);
                    }
                }
            }
            // Search core3
            for(auto j : core3){
                if((*predeccessors).src == j.at(0)){
                    if(min_start_time < j.at(1) + j.at(2)){
                        min_start_time = j.at(1) + j.at(2);
                    }
                }
            }
            // Search cloud
            for(auto j : wireless_recv_ch){
                if((*predeccessors).src == j.at(0)){
                    if(min_start_time < j.at(1) + j.at(2)){
                        min_start_time = j.at(1) + j.at(2);
                    }
                }
            }
            predeccessors = (*predeccessors).nextSucc;
        }

        T_s = (float) (*graph).array[task].uploadWeight / (float) R_s;
        T_r = (float) (*graph).array[task].downloadWeight / (float) R_r;
        T_c = (float) (*graph).array[task].cloud;
        send_start_time = 0;

        // Determine if task is a cloud task
        // Cloud task
        if((*graph).array[task].cloud_task == 1){

            // Need to check cloud availability as well as min start time
            // go by whichever is larger
            t_cloud_availability = (t_cloud_availability>min_start_time) ? t_cloud_availability : min_start_time ;

            // Calculate execution time
            if(t_cloud_availability == 0){
                t_cloud_availability = T_s;
            }
            else{
                send_start_time = t_cloud_availability - T_s;
            }

            // t_cloud_availability may need to include T_s. depends on if a predecessor is in receiving
            if (send_start_time < min_start_time){
                send_start_time = min_start_time;
                t_cloud_availability += T_s;
            }

            wireless_send_ch.push_back(std::vector<float>{(float) task, (float) send_start_time, (float) T_s});
            cloud.push_back(std::vector<float>{(float) task, t_cloud_availability, (float) T_c});

            // add weight to start time for next task
            t_cloud_availability += T_c;
            wireless_recv_ch.push_back(std::vector<float>{(float) task, t_cloud_availability,(float) T_r});
        }
            // Not a cloud task
        else{
            // Create execution time array
            exec_time[0] = ((t_1_availability > min_start_time) ? t_1_availability : min_start_time) + (*graph).array[task].core1;
            exec_time[1] = ((t_2_availability > min_start_time) ? t_2_availability : min_start_time) + (*graph).array[task].core2;
            exec_time[2] = ((t_3_availability > min_start_time) ? t_3_availability : min_start_time) + (*graph).array[task].core3;
            exec_time[3] = ((t_cloud_availability > min_start_time) ? t_cloud_availability : min_start_time) + (*graph).array[task].cloud;

            // Compare execution times to find min
            min = 0;
            for(j = 1; j < 4; j++){
                if(exec_time[j] < exec_time[min]){
                    min = j;
                }
            }

            // Assign to optimal execution medium
            switch(min){
                case 0:
                    // append to core1 vector
                    core1.push_back(std::vector<float>{(float) task, exec_time[0] - (*graph).array[task].core1, (float) (*graph).array[task].core1});
                    // add weight to start time for next task
                    t_1_availability = min_start_time + (*graph).array[task].core1;
                    break;
                case 1:
                    // append to core2 vector
                    core2.push_back(std::vector<float>{(float) task, exec_time[1] - (*graph).array[task].core2, (float) (*graph).array[task].core2});
                    // add weight to start time for next task
                    t_2_availability = min_start_time + (*graph).array[task].core2;
                    break;
                case 2:
                    // append to core3 vector
                    core3.push_back(std::vector<float>{(float) task, exec_time[2] - (*graph).array[task].core3, (float) (*graph).array[task].core3});
                    // add weight to start time for next task
                    t_3_availability = min_start_time + (*graph).array[task].core3;
                    break;
                case 3:
                    // Need to check cloud availability as well as min start time
                    // go by whichever is larger
                    t_cloud_availability = (t_cloud_availability>min_start_time) ? t_cloud_availability : min_start_time ;

                    // Calculate execution time
                    if(t_cloud_availability == 0){
                        t_cloud_availability = T_s;
                    }
                    else{
                        send_start_time = t_cloud_availability - T_s;
                    }

                    if (send_start_time < min_start_time){
                        send_start_time = min_start_time;
                        t_cloud_availability += T_s;
                    }

                    wireless_send_ch.push_back(std::vector<float>{(float) task, (float) send_start_time, (float) T_s});
                    cloud.push_back(std::vector<float>{(float) task, t_cloud_availability, (float) T_c});

                    // add weight to start time for next task
                    t_cloud_availability += T_c;
                    wireless_recv_ch.push_back(std::vector<float>{(float) task, t_cloud_availability,(float) T_r});

                    break;
            }
        }
    }
    fig3Print(core1, core2, core3, wireless_send_ch, cloud, wireless_recv_ch);

    // Energy Consumption and Application Completion Time
    std::cout << "\nEnergy Consumption and Application Completion Time\n";
    std::cout << "---------------------------------------------------\n";
    float energy_1 = 0, energy_2 = 0, energy_3 = 0, energy_send = 0, energy_total = 0, completion_time = 0;

    for(auto i : core1){
        energy_1 += i.at(2) * P_1;
        if(i.at(2) + i.at(1) > completion_time){
            completion_time = i.at(2) + i.at(1);
        }
    }
    for(auto i : core2){
        energy_2 += i.at(2) * P_2;
        if(i.at(2) + i.at(1) > completion_time){
            completion_time = i.at(2) + i.at(1);
        }
    }
    for(auto i : core3){
        energy_3 += i.at(2) * P_3;
        if(i.at(2) + i.at(1) > completion_time){
            completion_time = i.at(2) + i.at(1);
        }
    }
    for(auto i : wireless_send_ch){
        energy_send += i.at(2) * P_s;
        if(i.at(2) + i.at(1) > completion_time){
            completion_time = i.at(2) + i.at(1);
        }
    }
    for(auto i : wireless_recv_ch){
        energy_send += i.at(2) * P_s;
        if(i.at(2) + i.at(1) > completion_time){
            completion_time = i.at(2) + i.at(1);
        }
    }

    energy_total = energy_1 + energy_2 + energy_3 + energy_send;

    std::cout << "Power of core 1:\t" << P_1 << std::endl;
    std::cout << "Power of core 2:\t" << P_2 << std::endl;
    std::cout << "Power of core 3:\t" << P_3 << std::endl;
    std::cout << "Power to send task:\t" << P_s << std::endl << std::endl;
    std::cout << "Total energy for initial scheduling algorithm:\t\t" << energy_total << std::endl;
    std::cout << std::setprecision(1) << "Completion Time for initial scheduling algorithm:\t" << completion_time << std::endl << std::endl;

    return;
}

// Generates min-delay scheduling w/o considering energy consumption
void stepOne(Graph graph, int length){
    // Local Variables
    int i, j, weight_i, index, cloud_task, size;
    float weight;
    Char characteristics;
    std::vector<float> succ;
    std::vector<std::pair<int, float>> weight_array;

    // Initialize vector succ to hold priorities of successors
    for(j = 0; j < length; j++){
        succ.push_back(0);
    }

    // Part A: Primary assignment
    std::cout << "\tExecution Time\n";
    std::cout << "Task" << "\tT_s" << "\tT_c" << "\tT_r" << "\tT_re" << std::endl;
    for(i = 1; i < (*graph).length; i++){
        // cloud_task = 1 for remote execution; assignment = 0 for local execution
        cloud_task = primary_assign(graph, i);

        // Part B: Task prioritizing
        weight = task_priority(graph, i, cloud_task);
        weight_array.push_back(std::make_pair(i, weight));
    }
    std::cout << "==============================================\n";
    std::cout << std::endl;

    // Define priority for the last element
    size = weight_array.size();
    (*graph).array[size].priority = weight_array[size - 1].second;

    // Traverse task graph and define priority array recursively
    // Begins on second to last element
    for(i = (*graph).length - 2; i > 0; i--){
        (*graph).array[i].priority = weight_array[i - 1].second + max_ele(graph, i);
    }

    // Part C: Execution unit selection
    selection(graph);

    return;
}

/*      Task Migration Algorithm        */
// Minimize energy consumption under the application completion time constraint
void stepTwo(Graph graph){
    // Local Variables
    int i;

    return;
}

/*      Main function                   */
int main(int argc, char **argv){
    // Local Variables
    Graph graph;
    Char characteristics;
    int i, length;

    if(EXAMPLE == 1){
        length = 11;

        // Initialize graph
        graph = createGraph(length);
        // Add weight to nodes with the following parameters:
        // graph, src, upload weight, download weight
        // core1 weight, core2 weight, core3 weight, cloud_weight
        addWeight(graph, 1, 5, 5, 9, 7, 5, 1);
        addWeight(graph, 2, 4, 3, 5, 6, 8, 2);
        addWeight(graph, 3, 4, 4, 6, 4, 5, 3);
        addWeight(graph, 4, 3, 3, 7, 5, 3, 4);
        addWeight(graph, 5, 3, 4, 2, 5, 4, 5);
        addWeight(graph, 6, 2, 3, 6, 4, 7, 4);
        addWeight(graph, 7, 3, 2, 8, 5, 3, 3);
        addWeight(graph, 8, 2, 2, 2, 4, 6, 2);
        addWeight(graph, 9, 3, 4, 3, 2, 5, 1);
        addWeight(graph, 10, 4, 5, 7, 4, 2, 1);


        addPoint(graph, 1, 2);
        addPoint(graph, 1, 3);
        addPoint(graph, 1, 4);
        addPoint(graph, 1, 5);
        addPoint(graph, 1, 6);
        addPoint(graph, 2, 8);
        addPoint(graph, 2, 9);
        addPoint(graph, 3, 7);
        addPoint(graph, 4, 8);
        addPoint(graph, 4, 9);
        addPoint(graph, 5, 9);
        addPoint(graph, 6, 8);
        addPoint(graph, 7, 10);
        addPoint(graph, 8, 10);
        addPoint(graph, 9, 10);
    }

//

    // Print task graph and task table
    initialPrint(graph);

    // Initial scheduling algorithm
    stepOne(graph, length);

    // Task migration algorithm
    stepTwo(graph);

    // Gracefully terminate
//    destroy(graph);

    // Return function
    return 0;

}
