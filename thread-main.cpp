// ---------------------------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/22/2016
// PROGRAM ASSIGNMENT 6
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//      This is the main thread program. It takes the 
//      arguments from the command line. If there are none, the 
//      default is going to be 30. It then start Master thread.
//      It waits for master thread to finish, then exits
// ----------------------------------------------------------------------------

#include <stdio.h>
#include "thread.h"

//---------------------------------------------------------------//
//  main program 
//---------------------------------------------------------------//

int main( int argc, char *argv[] ) {
    //Program Variables
    int n;

    //If no arguments, set n to 30
    if ( argc == 1 ) 
        n = 30;
    else 
        n = atoi( argv[1] );

    //Creates, starts and then waits for master thread to end
    MasterThread *mThread;
    mThread = new MasterThread( 1, n );
    mThread->Begin();
    mThread->Join();

    Exit();
    return 0;
}


