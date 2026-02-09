/*CPU SCHEDULING SIMULATOR*/ //--FCFS, SJF, RR algorithms
/*
1. Number of process?                               //--done
2. auto burst or manual? Enter burst if manual      //--done
3. Choose algorithm                                 //--done
4. Display result                                   //--
5. Save file                                        //--
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
typedef struct structure
{
    int pid;
    int wt; 
    int bt;
    int tt;
}pro;
enum algorithm
{
    FCFS= 2,
    SJF=3,
    RR= 5,
    EXIT=6
};
enum inputType
{
    manual=3,
    aut0=5
};
void clear()
{
    FILE* pointer;
    pointer= fopen("simulator.txt", "w");
    if(pointer== NULL)
    {
        printf("Alert!\n");
        return;
    }
    fclose(pointer);
}
int burst()
{
    Sleep(rand()%2000);
    return (rand()%11);
}
void save(struct structure p[], int num)
{
    FILE* pointer;
    pointer= fopen("simulator.txt", "a");
    fprintf(pointer, " simulation:\npid\twt\tbt\ttt\n");
    for(int i=0; i<num; i++)
    {
        fprintf(pointer, "P%d\t%d\t%d\t%d\n", p[i].pid, p[i].wt, p[i].bt, p[i].tt);
    }
    fclose(pointer);
}
void display(struct structure b[], int n)
{
    printf("Simulation result:\npid\twt\tbt\ttt\n");
    for(int i=0; i<n; i++)
    {
        printf("P%d\t%d\t%d\t%d\n", b[i].pid, b[i].wt, b[i].bt, b[i].tt);
    }
    printf("Simulation end\n");
}
void average(struct structure p[], int n)
{
    int w=0, t=0;
    for(int i=0; i<n; i++)
    {
        w= w+ p[i].wt; 
        t= t+ p[i].tt;
    }
    printf("Average waiting time: %f\n", (float)w/n);
    printf("Average turnaround time: %f\n", (float)t/n);
}
void fcfs_sjf(struct structure p[], int num)
{
    p[0].wt=0;
    for(int i=0; i<num; i++)
    {
        p[i].tt= p[i].wt+ p[i].bt;
        if(i==(num-1))
            continue;
        p[i+1].wt= p[i].tt;
    }
}
void fcfs(struct structure p[], int num)
{
    // p[0].wt=0;
    // for(int i=0; i<num; i++)
    // {
    //     p[i].tt= p[i].wt+ p[i].bt;
    //     if(i==(num-1))
    //         continue;
    //     p[i+1].wt= p[i].tt;
    // }
    pro temp[num];
    for(int i=0;i<num;i++)
        temp[i]=p[i];
    fcfs_sjf(temp, num);
    printf("FCFS\n");
    save(temp, num);
    display(temp, num);
    average(temp, num);
}
void sjf(struct structure p[], int n)
{
    // int rembt[n];   //to store ascending order of burst time
    pro temp[n];
    // for(int i=0; i<n; i++)  //copying burst time in rembt array
    // {
    //     rembt[i]= p[i].bt;
    // }
    for(int i=0;i<n;i++)
        temp[i]=p[i];
    struct structure t;
    for(int i=0; i<n-1; i++)
    {
        for(int j=0; j<n-i-1; j++)
        {
            if(temp[j].bt > temp[j+1].bt)
            {
                t= temp[j];
                temp[j]= temp[j+1];
                temp[j+1]= t;
            }
        }
    }
    fcfs_sjf(temp, n);
    printf("SJF\n");
    save(temp, n);
    display(temp, n);
    average(temp, n);
}
void rr(struct structure p[], int n, int q)
{
    printf("Reading simulation results. Please be patient!\n");
    pro temp[n];
    for(int i=0;i<n;i++)
        temp[i]=p[i];
    int time=0;
    int rembt[n];
    for(int i=0; i<n; i++)
    {
        rembt[i]= temp[i].bt;
    }
    while(1)
    {
        int done = 1;
        for(int i=0; i<n; i++)
        {
            if(rembt[i] > 0)
            {
                done = 0;
                if(rembt[i] <= q)
                {
                    time += rembt[i];
                    rembt[i] = 0;
                    temp[i].tt = time;
                }
                else
                {
                    time += q;
                    rembt[i] -= q;
                }
            }
        }
        if(done)
            break;
    }
    for(int i=0; i<n; i++)
    {
        temp[i].wt= temp[i].tt- temp[i].bt;
    }
    printf("RR\n");
    save(temp, n);
    display(temp, n);
    average(temp, n);
}
int main()
{
    int num, data, control, slice;  //number of processes, data entry method, menu control, quantum size
    srand(time(NULL));
    printf("\n --- CPU scheduling simulator --- \n");
    printf("Enter number of processes:\n");
    scanf("%d", &num);
    pro p[num];
    printf("Choose any one:\n3. Manual data\n5. Auto data\n");
    scanf("%d", &data);
    for(int i=0; i<num; i++)    //pid generated, else reset
    {
        p[i].pid= i+1;
        p[i].bt=0;
        p[i].tt=0;
        p[i].wt=0;
    }
    //wt generated
    if(data== manual)
    {
        for(int i=0; i<num; i++)
        {
            printf("Enter burst time for process P%d:\n", i+1);
            scanf("%d", &p[i].bt);
        }
    }
    else if(data== aut0)
    {
        printf("Generating values. Please be patient!\n");
        for(int i=0; i<num; i++)
        {
            p[i].bt= burst();
        }
    }
    else
    {
        printf("Invalid response!\n");
        return 0;
    }

    clear();    //if file exist, format
    //menu loop
    do
    {
        //UI
        printf("2. FCFS simulation\n3. SJF simulation\n5. RR simulation\n6. Exit simulation\nEnter choice:\n");
        scanf("%d", &control);
        //logic
        switch(control)
        {
            case FCFS:  //--done
                fcfs(p, num);
                break;
            case SJF:   //--done
                sjf(p, num);
                break;
            case RR:    //--done
                printf("Enter Quantum size:\n");
                scanf("%d", &slice);
                rr(p, num, slice);
                break;
            case EXIT:
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }while(control!= EXIT);
    //menu loop end
    printf("Exitting...\n");
    return 0;
}

//CPU Scheduling Simulator --Discription
/*
# DEFINITION
CPU Scheduling is an operating system mechanism that decides which process gets the CPU and for how long when multiple processes are present in the ready queue.
# A CPU Scheduling Simulator is a program that imitates the working of different scheduling algorithms and calculates performance parameters such as:
1. Waiting Time (WT)
2. Turnaround Time (TAT)
3. Average Waiting Time
4. Average Turnaround Time

# TERMINOLOGIES
(a) Burst Time (BT):
The total time required by a process to execute on the CPU.
(b) Waiting Time (WT):
The total time a process waits in the ready queue before getting CPU.
WT=TAT−BT
(c) Turnaround Time (TAT)
Total time taken from process arrival to completion.
𝑇𝐴𝑇=𝐶𝑜𝑚𝑝𝑙𝑒𝑡𝑖𝑜𝑛 𝑇𝑖𝑚𝑒−𝐴𝑟𝑟𝑖𝑣𝑎𝑙 𝑇𝑖𝑚𝑒TAT=Completion Time−Arrival Time
(Arrival time is assumed as 0 in this simulator.)

# ROUND ROBIN
Each process gets a fixed time quantum
After quantum expires, process goes back to ready queue

#ALGORITHM IMPLEMENTED
FCFS (First Come First Serve)
SJF (Shortest Job First – Non-Preemptive)
Round Robin

#CONCEPTS USED
pid, burst time
Waiting Time & Turnaround Time calculation
*/