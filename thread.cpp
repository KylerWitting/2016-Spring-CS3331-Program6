// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/22/2016
// PROGRAM ASSIGNMENT 6
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//      Create Master and Prime threads to find every 
//      prime number between 1 and n. 
// -----------------------------------------------------------


#include <stdio.h> //Used for sprintf
#include <string.h> //Used for strlen
#include "thread.h"

static int PrimeNumbers[100];

//---------------------------------------------------------------//
//  Master Class: 
//     constructor
//---------------------------------------------------------------//

MasterThread::MasterThread( int ThreadID, int n )
{
    char buffer[1024];

    UserDefinedThreadID = ThreadID;
    Number = n;

    sprintf( buffer, "Master starts\n" );
    write( 1, buffer, strlen( buffer ) );
}

//---------------------------------------------------------------//
//  Master Class: 
//     Starts Prime Thread 2, then sends each number between 3 and 
//     Number to it. Then it sends -1 to END, and then waits for
//     Prime 2 to end. Then Exits
//---------------------------------------------------------------//
void MasterThread::ThreadFunc()
{
    Thread::ThreadFunc();
    //Buffer and iterator vars
    int i, lineCount = 0;
    char buffer[1024];

    //Sets all of PrimeNumbers to 0
    for( i = 0; i < 100; i++ )
        PrimeNumbers[i] = 0;

    //PrimeThread *pThread;
    P2 = new PrimeThread ( 2, 1, 0 );
    P2->Begin();

    //Sends Every Number between 2 and n to P2
    for( i = 3; i <= Number; i++ ){
        sprintf( buffer, "Master Sends %d to P2\n", i );
        write( 1, buffer, strlen( buffer ) );

        P2->Channel->Send( &i, sizeof(int) );
    }

    //Sends the End of Numbers indicator 
    i = -1;
    sprintf( buffer, "Master Sends END\n" );
    write( 1, buffer, strlen( buffer ) );
    P2->Channel->Send( &i, sizeof(int) );

    //Waits until all threads are done to print array 
    P2->Join();

    //Master prints the prime numbers betwee 1 and n
    sprintf( buffer, "Master prints the complete result:\n" );
    for( i = 0; i < Number; i++ ) {
        if( lineCount > 17 ) { 
            sprintf( buffer + strlen( buffer ), "\n" );
            lineCount = 0;    
        }
        if( PrimeNumbers[i] !=0 ) {
            sprintf( buffer + strlen( buffer ), "%4d", PrimeNumbers[i] );
            lineCount+=1;
        }
    }
    sprintf( buffer + strlen( buffer ), "\n" );
    write( 1, buffer, strlen( buffer ) );

    //Master Ends it's Thread
    sprintf( buffer, "Master Terminates\n" );
    write( 1, buffer, strlen( buffer ) );
    Exit();
}

//---------------------------------------------------------------//
//  Prime Class: 
//     constructor
//---------------------------------------------------------------//
PrimeThread::PrimeThread( int threadID, int prev, int count) 
{
    char buffer[1024];
    int i;
    //Gives this thread ID for creating channels
    UserDefinedThreadID = threadID;

    //Adds thread ID to the prime number list
    PrimeNumbers[count] = threadID;

    //Initilizes variables 
    Neighbor = NULL;
    ThreadID = threadID;
    PreviousThread = prev;
    Count = count;

    //Creates channel to previous thread 
    sprintf( buffer, "%d: Channel%d-%d", threadID, prev, threadID );
    Channel = new SynOneToOneChannel( buffer, prev, threadID );

    //-------------------------------------------------------------------------
    sprintf( buffer, "  " );
    for( i = 0; i < count; i++ ) {
        sprintf( buffer+strlen(buffer), "  " );
    }
    sprintf( buffer+strlen(buffer), 
            "P%d starts and memorizes %d\n", threadID, threadID );
    write( 1, buffer, strlen( buffer ) );
    //-------------------------------------------------------------------------
}

//---------------------------------------------------------------//
//  Prime  Class: 
//      Prime Thread puts the number of it's thread ID into the 
//      PrimeNumbers, Then waits and evaluates new numbers being 
//      passed to it. If they are prime it passes it, if not 
//      then it ignores it. 
//---------------------------------------------------------------//
void PrimeThread::ThreadFunc()
{
    Thread::ThreadFunc();
    //newNumber is the number passed to thread
    //i is iterator, buffer is a char buffer for writing 
    int newNumber, i;
    char buffer[1024];

    //Keeps waiting for next number until END is recieved 
    for( ;; ) {
        //Channel waits until it recieves a number from previous thread
        Channel->Receive(&newNumber, sizeof(int));

        //If END, exit infinite loop 
        if ( newNumber == -1 )
            break;

        //---------------------------------------------------------------------
        sprintf( buffer, "  " );
        for( i = 0; i < Count; i++ ) {
            sprintf( buffer+strlen(buffer), "  " );
        }
        sprintf( buffer+strlen(buffer), 
                "P%d recieves %d\n", ThreadID, newNumber );
        write( 1, buffer, strlen( buffer ) );
        //---------------------------------------------------------------------

        //If it is a prime number, it passes it along to next thread
        if ( ( newNumber % ThreadID ) != 0 ) {
            //If there is no neighbor yet, create it. 
            if ( Neighbor == NULL ) {

                //-------------------------------------------------------------
                sprintf( buffer, "  " );
                for( i = 0; i < Count; i++ ) {
                    sprintf( buffer+strlen(buffer), "  " );
                }
                sprintf( buffer+strlen(buffer), 
                        "P%d creates P%d\n", ThreadID, newNumber );
                write( 1, buffer, strlen( buffer ) );
                //-------------------------------------------------------------

                Neighbor = new PrimeThread( newNumber, ThreadID, Count+1);
                Neighbor->Begin();
            }
            //If there is a neighbor, pass the number through the channel
            else { 
                //-------------------------------------------------------------
                sprintf( buffer, "  " );
                for( i = 0; i < Count; i++ ) {
                    sprintf( buffer+strlen(buffer), "  " );
                }
                sprintf( buffer+strlen(buffer), 
                        "P%d sends %d to P%d\n", 
                        ThreadID, newNumber, PrimeNumbers[Count+1] );
                write( 1, buffer, strlen( buffer ) );
                //-------------------------------------------------------------

                Neighbor->Channel->Send(&newNumber, sizeof(int));
            }

        }
        //Number is not prime, so ignore it
        else { 
            //-----------------------------------------------------------------
            sprintf( buffer, "  " );
            for( i = 0; i < Count; i++ ) {
                sprintf( buffer+strlen(buffer), "  " );
            }
            sprintf( buffer+strlen(buffer), 
                    "P%d ignores %d\n", ThreadID, newNumber );
            write( 1, buffer, strlen( buffer ) );
            //-----------------------------------------------------------------
        }
    }

    //-------------------------------------------------------------------------
    sprintf( buffer, "  " );
    for( i = 0; i < Count; i++ ) {
        sprintf( buffer+strlen(buffer), "  " );
    }
    sprintf( buffer+strlen(buffer), "P%d recieves END\n", ThreadID );
    write( 1, buffer, strlen( buffer ) );
    //-------------------------------------------------------------------------

    //If there is a neighbor, send it the END notification
    //Once it Exits(), this thread can exit. Cascading style. 
    if ( Neighbor != NULL ) {
        Neighbor->Channel->Send(&newNumber, sizeof(int));
        Neighbor->Join();
    }
    Exit();
}
