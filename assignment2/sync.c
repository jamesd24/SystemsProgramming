#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "sync.h"

#define bufferSize 256

typedef struct child_args 
{
    char* buffer;
    pthread_mutex_t readLock;
    pthread_mutex_t endLock;
} chargs;


int main()
{
    chargs cargs;    
    pthread_t child;
    pthread_mutex_init(&cargs.readLock, NULL);
    pthread_mutex_init(&cargs.endLock, NULL);
    
    pthread_mutex_lock(&cargs.readLock);
    
    if(pthread_create(&child, NULL, *readLine, (void *) &cargs))
    {
        fprintf(stderr, "Error creating thread\n");
    }
        
    pthread_mutex_unlock(&cargs.readLock);
    pthread_mutex_lock(&cargs.endLock);
    sleep(1);
    pthread_mutex_lock(&cargs.readLock);
    fprintf(stdout, "The string you gave the child was: %s", cargs.buffer);
    
    pthread_mutex_unlock(&cargs.endLock);
          
    
    void* end;
    if(pthread_join(child, &end))
        fprintf(stdout, "Unable to join child: %d\n", (int)child);
    fputs("Child ended\n", stdout);
    
    pthread_mutex_destroy(&cargs.readLock);
    pthread_mutex_destroy(&cargs.endLock);
    
    exit(0);
}

void *readLine(void* ptr)
{
    chargs* args = malloc(sizeof(chargs));
    args = (chargs*) ptr;
    
    pthread_mutex_lock(&args->readLock);
    fputs("Give an input string to the child: ", stdout);
    fgets (args->buffer, bufferSize, stdin);
    
    pthread_mutex_unlock(&args->readLock);
    pthread_mutex_lock(&args->endLock);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    
    fputs("Please hit the enter key to end the program", stdout);
    
    int c;
    
    while( (c = fgetc(stdin)) != '\n');
    {        
    }
    pthread_mutex_unlock(&args->endLock);
    pthread_exit((void*)NULL);
}