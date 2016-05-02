// ----------------------------------------------------------- 
// NAME : Kyler Witting                      User ID: kmwittin 
// DUE DATE : 04/22/2016
// PROGRAM ASSIGNMENT 6
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//      Head file for class Thread classes definitions
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

// header file
#include "ThreadClass.h"

class PrimeThread: public Thread 
{
    public: 
        PrimeThread( int threadID, int prev, int count );
        SynOneToOneChannel  *Channel;
    private: 
        void ThreadFunc();
        int ThreadID;
        int PreviousThread;
        int Count;
        PrimeThread *Neighbor;
};

class MasterThread: public Thread
{
    public:
        MasterThread( int ThreadID, int n );
    private:
        void ThreadFunc();
        int Number;
        PrimeThread *P2;
};

#endif
