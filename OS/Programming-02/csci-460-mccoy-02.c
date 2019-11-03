#include <stdio.h>
#include <stdbool.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
//to run
//gcc csci-460-mccoy-02.c -lgsl -lgslcblas -lm -o hey


int n=10; //number of prcesses
int k = 10; // time interval during which processes may arrive uniformly at random
int d =4; //the mean of the normal distributions of cpu time
int v =1; //the variancew of the normal distribution of cpu time

typedef struct process process;
/* Declare the struct with integer members x, y */
struct process {
    int    p;//process
    bool   v;//active 1=active
    int    a;//arrival time uniform distribution from 0 to k
    int    u;// cpu time chossen from N(d,v)
    int    r;//remaining cpu time initilized to u
    int    tt;//turnaround time.
    
};


void printp(process *c)
{
     printf("(P = %d, v = %d, a = %d, cpu = %d, r = %d, tt = %d)\n", c->p, c->v, c->a, c->u, c->r, c->tt);
}


int main(void)
{

    process data[n]; //array of processes
    process data1[n];
    process data2[n];
    
    const gsl_rng_type * T;//for the random number generation
    gsl_rng * r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    
    int i=0;//generate our tables
    for(i=0; i<n; i++)
        {
            data[i].p=i;data1[i].p=i;data2[i].p=i;
            data[i].v=0;data1[i].v=0;data2[i].v=0;
            double x = gsl_ran_flat(r, 0, k);
            int w =x;
            data[i].a=w;data1[i].a=w;data2[i].a=w;
            double y = gsl_ran_gaussian(r, v)+d;
            int z = y;
            data[i].u=z;data1[i].u=z;data2[i].u=z;
            data[i].r=z;data1[i].r=z;data2[i].r=z;
            data[i].tt=0;data1[i].tt=0;data2[i].tt=0;
        
        }
    
 
    //print our table
    for(int i=0;i<n;i++){
        printp(&data[i]);
    }
   
    int R=0;//find total remaining time
    int R1;
    int R2;
    for(int i=0;i<n;i++){R = R + data[i].r;}
    R1=R;
    R2=R;
    //////////////////////////////////////////////////////
    //Begin FIFO update data here
    //////////////////////////////////////////////////////
    int t = 0;
    int V = 0;
    while(R!=0){
       
            t=t+1;
            int x=-1;//store the index we will process
            int l=k+1;//find the earliest
            for(int i=0; i<n;i++){
                //arrived past current time, there is time remaining
                // and the earliest
                if(data[i].a<=t && data[i].r!=0 && data[i].a<l)
                {l=data[i].a; x=i;}
            }
            
            printf("%d, %d\n", x ,t);
            if(x!=-1){
                
                    while(data[x].r>0){
                        data[x].r=data[x].r-1;
                        t=t+1;
                    }
               data[x].tt=t-data[x].a;
            }
        
        R=0;
        for(int i=0;i<n;i++){R = R + data[i].r;}
    }
    
    
    for(int i=0;i<n;i++){
        printp(&data[i]);
    }
    double fifo=0;
    for(int i=0;i<n;i++){fifo=fifo+data[i].tt;}
    fifo=fifo/n;
    printf("%f\n", fifo);
    
    //////////////////////////////////////////////////////
    // End FIFO 
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////
    // Begin SJF using data1[]
    //////////////////////////////////////////////////////
    for(int i=0;i<n;i++){
        printp(&data1[i]);
    }
    
    t = 0;
    V = 0;
    while(R1!=0){
        
        t=t+1;
        int x=-1;//store the index we will process
        int l=k+1;//find the earliest
        for(int i=0; i<n;i++){
            //arrived past current time, there is time remaining
            // and the earliest
            if(data1[i].a<=t && data1[i].r!=0 && data1[i].u<l)
            {l=data1[i].a; x=i;}
        }
        
        printf("%d, %d\n", x ,t);
        if(x!=-1){
            
            while(data1[x].r>0){
                data1[x].r=data1[x].r-1;
                t=t+1;
            }
            data1[x].tt=t-data1[x].a;
        }
        
        R1=0;
        for(int i=0;i<n;i++){R1 = R1 + data1[i].r;}
    }
    
    
    for(int i=0;i<n;i++){
        printp(&data1[i]);
    }
    double dude=0;
    for(int i=0;i<n;i++){dude=dude+data1[i].tt;}
    dude=dude/n;
    printf("%f\n", dude);
    
    
    //////////////////////////////////////////////////////
    // End SJF
    //////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////
    // Begin SRT using data2[]
    //////////////////////////////////////////////////////
    for(int i=0;i<n;i++){
        printp(&data2[i]);
    }
    
    t = 0;
    V = 0;
    while(R2!=0){
        
        t=t+1;
        int x=-1;//store the index we will process
        int l=k+1;//find the earliest
        for(int i=0; i<n;i++){
            //arrived past current time, there is time remaining
            // and the earliest
            if(data2[i].a<=t && data2[i].r!=0 && data2[i].r<l)
            {l=data2[i].r; x=i;}
        }
        
        printf("%d, %d\n", x ,t);
        if(x!=-1){
          
            data2[x].r=data2[x].r-1;
            if(data2[x].r==0){
                data2[x].tt=t-data2[x].a+1;
                
            }
            
        }
        
        R2=0;
        for(int i=0;i<n;i++){R2 = R2 + data2[i].r;}
    }
    
    
    for(int i=0;i<n;i++){
        printp(&data2[i]);
    }
    double bro=0;
    for(int i=0;i<n;i++){bro=bro+data2[i].tt;}
    bro=bro/n;
    printf("%f\n", bro);
    
    
    //////////////////////////////////////////////////////
    // End SRT
    //////////////////////////////////////////////////////
    gsl_rng_free (r);
    return 0;
}
