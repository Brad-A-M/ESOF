#include <stdio.h>
#include <stdbool.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
//to run
//gcc csci-460-mccoy-02.c -lgsl -lgslcblas -lm -o hey


int n=10; //number of prcesses
int k = 20; // time interval during which processes may arrive uniformly at random
int d =4; //the mean of the normal distributions of cpu time
int v =1; //the variancew of the normal distribution of cpu time

typedef struct process process;
/* Declare the struct with integer members x, y */
struct process {
    int    p;//process
    bool   v;//active 1=active
    int    a;//arrival time uniform distribution from 0 to k
    int    t;// cpu time chossen from N(d,v)
    int    r;//remaining cpu time initilized to t
    int    tt;//turnaround time.
    
};


void printp(process *c)
{
     printf("(P = %d, v = %d, a = %d, cpu = %d, r = %d, tt = %d)\n", c->p, c->v, c->a, c->t, c->r, c->tt);
}


int main(void)
{

    process data[n]; //array of processes
    
    const gsl_rng_type * T;//for the random number generation
    gsl_rng * r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    
    int i=0;//generate our table
    for(i=0; i<n; i++)
        {
            data[i].p=i;
            data[i].v=0;
            double x = gsl_ran_flat(r, 0, k);
            int w =x;
            data[i].a=w;
            double y = gsl_ran_gaussian(r, v)+d;
            int z =y;
            data[i].t=z;
            data[i].r=z;
            data[i].tt=0;
        
        }
    
 
    //print our table
    for(int i=0;i<n;i++){
        printp(&data[i]);
    }
   
    int R=0;//find total remaining time
    for(int i=0;i<n;i++){R = R + data[i].r;}
    
    //FIFO
    int t = 0;
    while(R!=0){
        int V = 0;
        for(int i=0;i<n;i++){V = V + data[i].v;}
        //while(V==0){
            t=t+1;
            int x=-1;
            int l=k+1;//find the earliest
            for(int i=0; i<n;i++){
                //arrived past current time, there is time remaining
                // and the earliest
                if(data[i].a<=t && data[i].r!=0 && data[i].a<l)
                {l=data[i].a; x=i;}
            }
            
        printf("%d, %d\n", x ,t);
        if(x!=-1){data[x].r=data[x].r-1;}
        //}
        R=0;
        for(int i=0;i<n;i++){R = R + data[i].r;}
    }
    gsl_rng_free (r);
    return 0;
}
