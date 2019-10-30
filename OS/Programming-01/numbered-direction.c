#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <pthread.h>
#include <limits.h>
#include <string.h>


/* Compile like this:
 gcc --std=c99 -lpthread McCoy-460-Programming-01.c -o hi
 */

#define NUM_CARS 20
#define max_cars 3
int rc;
int count=0;//record how many cars are on the one way
int went=0; // record total cars that have gone on the oneway
int Z=0; //number of cars waiting to go to Bozeman
int R=0; //number of cars waiting to go to Bridger
char *currentdirection; //what direction are we currently going, initialized to the first car through

//tell cars to Bozeman to wait/go
pthread_mutex_t cond_Z_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_Z = PTHREAD_COND_INITIALIZER;
//tell cars to Bridger to wait/go
pthread_mutex_t cond_R_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_R = PTHREAD_COND_INITIALIZER;

void ArriveBridgerOneWay(char *message){
    
    int d = pthread_self();
    
    //liveness: if no one is going then go!
    if(count==0){currentdirection=message;
        printf("Direction is set to %s set when count = 0\n", message);
    }
    
   
    
    //Saftey: current direction must be our direction and there are
    //not more than 3 (maxcars going). Else wait.
    if( (strcmp(message, "To_Bozeman")==0 && strcmp(currentdirection, "To_Brider")==0) || count>=max_cars){
        Z=Z+1;
        rc = pthread_cond_wait(&cond_Z, &cond_Z_mutex);
        printf("[thread %d] %s is waiting, code %d, there are %d cars waiting to go this way.\n", d, message, rc, Z);
        
        
    }
    if((strcmp(message, "To_Bridger")==0 && strcmp(currentdirection, "To_Bozeman")==0) || count>=max_cars){
        R=R+1;
        rc = pthread_cond_wait( &cond_R, &cond_R_mutex);
        printf("[thread %d] %s is waiting, code %d, there are %d cars waiting to go this way.\n", d, message, rc, R);
    }
    
}



void OnBriderOneWay(char *message){
    
    
    int d = pthread_self();
    
    count = count + 1;
    went = went + 1;
    sleep(1);
    printf("[thread %d] %s is now going! There is/are %d car(s) on the one way. The current direction is %s\n", d, message, count, currentdirection);
    printf("There are %d cars waiting to go to Bozeman, and %d cars waiting to go to Bridger. So far, %d cars have gone\n",Z,R, went);
    
}

void ExitBridgerOneWay(char *message){
    
    count -=1;
    
    
    if( Z==0 && count==0 && R>0)
    {
        currentdirection = "To_Bridger";
        printf("Dirction Flipped it is now %s\n", currentdirection);
        rc = pthread_cond_signal(&cond_R);
        if(rc==0){R=R-1;}
        
    }
    if( R==0 && count==0 && Z>0)
    {
        currentdirection = "To_Bozeman";
        printf("Dirction Flipped it is now %s\n", currentdirection);
        rc = pthread_cond_signal(&cond_Z);
        if(rc==0){Z=Z-1;}
        
    }
    
    
}

void *OneVehicle(void *arg) {
    
    char *message;
    message = (char *) arg;
    
    pthread_mutex_lock( &cond_R_mutex );
    pthread_mutex_lock( &cond_Z_mutex );
    
    ArriveBridgerOneWay(message);
    
    
    OnBriderOneWay(message);
    ExitBridgerOneWay(message);
    pthread_mutex_unlock( &cond_R_mutex );
    pthread_mutex_unlock( &cond_Z_mutex );
    pthread_exit(NULL);
}




int main(int argc, char *argv[]) {
    
    pthread_t threads[NUM_CARS];
    int rc;//return code
    long t;//iterate over threads
    int dirc;//used to assign direction
    char *message1 = "To_Bozeman";
    char *message2 = "To_Bridger";
    char *message;//temp
    
    
    
    // create threads (pthread_create)
    for(t=0; t<NUM_CARS; t++){
        //assign a direction to each thread at random
        dirc = rand() % 2;
        if(dirc==0){message=message2;}
        else{message=message1;}
        
        //<<<<<<<<<<<<<<<<<<<< create threads
        rc = pthread_create(&threads[t], NULL, OneVehicle, (void *) message);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    
    
    // wait for the threads to finish (pthread_join)
    // pthread_join() blocks the calling thread until the specified thread terminates.
    for(t=0; t<NUM_CARS; t++){
        
        rc = pthread_join(threads[t], NULL); //<<<<<<<<<<<<<<<<<<<< wait for threads to finish
        if (rc){
            fprintf(stderr, "ERROR; return code from join() is %d\n", rc);
            exit(-2);
        }
    }
    
    pthread_mutex_destroy(&cond_R_mutex); //<<<<<<<<<<<<<<<<<<<< clean up
    pthread_mutex_destroy(&cond_Z_mutex);
    pthread_cond_destroy(&cond_R);
    pthread_cond_destroy(&cond_Z);
    pthread_exit(NULL);
    
    return 0;
}

