#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <math.h>


/* Compile like this:
 gcc --std=c99 -lpthread McCoy-460-Programming-01.c -o brad
 */

int NUM_CARS; //must be even, we release cars in two batches.
int max_cars;
int rc;
int init =0;
int count=0;//record how many cars are on the one way
int went=0; // record total cars that have gone on the one way
int Z=0; //number of cars waiting to go to Bozeman
int R=0; //number of cars waiting to go to Bridger
char *currentdirection="To_Bozeman"; //what direction are we currently going, initialized to Bozeman
int flip=1;

//tell cars to Bozeman to wait/go
pthread_mutex_t cond_Z_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_Z = PTHREAD_COND_INITIALIZER;
//tell cars to Bridger to wait/go
pthread_mutex_t cond_R_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_R = PTHREAD_COND_INITIALIZER;

void ArriveBridgerOneWay(char *message){
    
    int d = pthread_self();
    //Saftey: If current direction is our direction and there are
    //not more than 3 (maxcars going), then go. Else wait.
    if( (strcmp(message, "To_Bozeman")==0 && strcmp(currentdirection, "To_Brider")==0) || count>=max_cars){
        Z=Z+1;
        printf("[thread %d] %s is waiting, there are %d cars waiting in this queue.\n", d, message, Z);
        
        rc = pthread_cond_wait(&cond_Z, &cond_Z_mutex);
        if (rc){
            printf("ERROR; return code from pthread_wait() is %d\n", rc);}
        pthread_mutex_unlock( &cond_Z_mutex );
        d = pthread_self();
        printf("[thread %d] %s woke up! \n", d, message);
    }
    
    if((strcmp(message, "To_Bridger")==0 && strcmp(currentdirection, "To_Bozeman")==0) || count>=max_cars){
        R=R+1;
        printf("[thread %d] %s is waiting, there are %d cars waiting in this queue.\n", d, message, R);
        rc = pthread_cond_wait( &cond_R, &cond_R_mutex);
        if (rc){
            printf("ERROR; return code from pthread_wait() is %d\n", rc);}
        pthread_mutex_unlock( &cond_R_mutex );
        d = pthread_self();
        printf("[thread %d] %s woke up! \n", d, message);
    }
}


void OnBriderOneWay(char *message){

    int d = pthread_self();
    pthread_mutex_lock( &cond_R_mutex );
    pthread_mutex_lock( &cond_Z_mutex );
    
    count = count + 1;//We are on the one way
    went = went + 1;

    sleep(1);//
    printf("[thread %d] %s is now going! There is %d car on the one way.\n", d, message, count);
    printf("There are %d cars waiting to go to Bozeman, and %d cars waiting to go to Bridger. %d cars have gone\n",Z,R, went);
}

void ExitBridgerOneWay(char *message){

    
    count -=1;
    

    pthread_mutex_unlock( &cond_R_mutex );
    pthread_mutex_unlock( &cond_Z_mutex );
    //If cars are waiting to go our current direcion let them go.
    
    if( went%((int)NUM_CARS/3) ==0 && R>0){currentdirection="To_Bridger";
        while(R>0){
            R=R-1;
            rc = pthread_cond_signal(&cond_R);
            if (rc){
                printf("ERROR; return code from pthread_signal() is %d\n", rc);
                exit(-1);
            }
            //printf("Release code %d to Bridger %d %s\n", rc, R,currentdirection);
            
        }
    }
    if(Z>0){currentdirection="To_Bozeman";
        while(Z>0){
            Z=Z-1;
            rc = pthread_cond_signal(&cond_Z);
            if (rc){
                printf("ERROR; return code from pthread_signal() is %d\n", rc);
                exit(-1);
            }
            //printf("Release code %d to Bozeman %d %s\n", rc, R,currentdirection);
        
        }
    }

    
}

void *OneVehicle(void *arg) {
    
    char *message;
    message = (char *) arg;
    
    
    ArriveBridgerOneWay(message);
    
    
    OnBriderOneWay(message);
    ExitBridgerOneWay(message);
   
    pthread_exit(NULL);
    }




int main(int argc, char *argv[]) {

    int a;
    int b;//temps for reading in values
    printf( "Enter a number of cars:");
    
    scanf("%d", &a);
    NUM_CARS=a;
    
    printf( "Enter a max value:");
    scanf("%d", &b);
    max_cars=b;
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
