//--------------------------------------------------------------------------------
//             ThreadMentor: Thread Visualization System
//           (c)2000-2001 Michigan Technological University
//
//  File: 
//     Threadclass.h
//  Program Description:
//     Thread library header file which includes the definition for thread class 
//     and all the other synchronization classes, which includes Semaphore, Mutex, 
//     Monitor (condition variable), Reader-Writer Locks, Barrier and Channel class.
//--------------------------------------------------------------------------------
#ifndef _THREADCLASS_H
#define _THREADCLASS_H
#ifndef _CPP_BACKWARD_BACKWARD_WARNING_H
#define _CPP_BACKWARD_BACKWARD_WARNING_H 1
#endif

#include "ThreadClassException.h"   // error message and exception handling
#include "ThreadClassPrototype.h"   // cross platform calls prototype

//--------------------------------------------------------------------------------
//                          OS dependent header files
//--------------------------------------------------------------------------------

#ifdef SOLARIS_THREAD   // SOLARIS_THREAD thread
#include <thread.h>
#include <synch.h>
#endif

#ifdef PTHREAD          // POSIX thread
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <semaphore.h>
#endif

#ifdef WIN32_THREAD            // Win32 Thread
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#endif

//#ifdef mtuTHREAD        // mtu Thread
//#include "mtuThread.h"
//#endif

//--------------------------------------------------------------------------------
//                          OS independent header files
//--------------------------------------------------------------------------------

#include <sys/types.h>
#include <stdlib.h>
#include <iostream>
#include <queue>

#include "ThreadType.h"     // platfrom dependent type redefinitions
#include "mtuStrstream.h"

using namespace std;

//--------------------------------------------------------------------------------
//                          macro defines
//--------------------------------------------------------------------------------

// define success as 0 is because all thread system call will return
// 0 as successful complettion
#define SUCCESS                 0   // For all synchronization primities
#define FAILURE                 1

#define NO_OWNER                0   // Mutex or RWLock's owner
#define FIND                    1   // RWLock::InReaderQueue
#define NOTFIND                 0
#define BARRIER_LAST            1   //Barrier::Wait
#define BARRIER_NORMAL          0
#define DEFAULT_RANDOMNUMBER    10  // the default random number used inthread delay
#define MAX_SEMVALUE            500 // The max value of a WIN32_THREAD semaphore

// following constants for Channel
#define EMPTY_CHANNEL          -1   // empty channel
#define NOT_SET_SENDER          0
#define NOT_SET_RECVER          0
#define NOT_SENDER              4   // sending thread does not own this channel
#define NOT_RECVER              5   // receiving thread does not own this channel
#define NO_MESSAGE              6


#define ZERO_CAPACITY 0      //capacity is ZERO
#define INFINITE_CAPACITY -1 // capacity is INFINITE

#ifndef _TM_MONITOR_TYPE_
#define _TM_MONITOR_TYPE_
#define HOARE    0
#define MESA     1
#endif

//--------------------------------------------------------------------------------
//                          data types
//--------------------------------------------------------------------------------

typedef void* Channel_Message_t;
enum channelstatus_t {IDLE, BUSY};

//--------------------------------------------------------------------------------
//                          function prototypes
//--------------------------------------------------------------------------------
    
void Exit(); // main thread exit function


//--------------------------------------------------------------------------------
//                          Class definition
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Thread class                 -> The class of normal thread
// It's members:
//      Exit()                  -> Exit this thread.
//      Join()                  -> Calling thread join this thread
//      Suspend()                -> suspend this thread
//      Resume()                -> resume a suspended thread
//      ThrYield()              -> Yield the thread to others
//      Delay()                 -> Delay the thread for a random time
//      GetID()                 -> Get the Id of this thread
//      Begin()                 -> Begin a thread with function Real_func
//      ThreadFunc()            -> The function which need to be rewritten
//                                         by user as a thread's body.
//      Thread()                -> The constructor of a thread, get a 
//                                 message queue(communicate with visual).
//      ~Thread()               -> The destructor of a thread, do nothing
//      Real_func()             -> The function which calls the thread body
//      Id                      -> The id of this thread
//--------------------------------------------------------------------------------

class Thread 
{
    // method members
public:
    // thread operation
    void Begin  (int, char*)   ;//throw(ThreadProblem);
    int  Suspend(int, char*) ;//throw(ThreadProblem);
    int  Resume (int, char*)  ;//throw(ThreadProblem);
    int  Join   (int, char*);
    void ThrYield(void);
    void Delay(unsigned int Number = DEFAULT_RANDOMNUMBER);
    void Exit(void);

    // send user specified message
    void SendUserMessage(char *message, int color, int, char*);

public:
    Thread_t GetID() const;
    int GetUserID();                // get user defined thread ID
    int GetNumThreads();            // get the number of user created threads    

protected:
    Thread(const char *name = "noname");  // constructor
    ~Thread();                      // destructor

protected:
    virtual void ThreadFunc();        // pure virtual funtion

    // data members
protected:
    Thread_t RealThreadId;          // thread ID
    Thread_t ParentThreadId;        // parent thread ID
    int UserDefinedThreadId;        // user defined thread ID

public:
    mtu_ostrstream ThreadName;      // thread name

private:
    int destroyed;
    int started;

#if defined(WIN32_THREAD)
    HANDLE hThread;
    static unsigned _stdcall Real_func(void *t);
#elif defined(SOLARIS_THREAD) || defined(PTHREAD) || defined(mtuTHREAD)
    static void Real_func(void *);
#endif

private:
    void start();

};

//--------------------------------------------------------------------------------
// Mutex class                  -> The mutex lock class
// It's members:
//      Mutex()                 -> The constructor of mutex
//      ~Mutex()                -> The destructor of mutex
//      Lock()                  -> To lock the mutex
//      Unlock()                -> Unlock the mutex
//      IsLocked()              -> Check if the mutex is in lock
//      mutex                   -> The real mutex lock
//      MutexName               -> Mutex's name
//--------------------------------------------------------------------------------

class Mutex
{
public:
    Mutex(const char *name="noname", int Locked=0);
    ~Mutex();
    void SetMutexName(const char *name);

    int  Lock(int, char*); 
    int  Unlock(int, char*);

    bool IsLocked();

private:
    mtu_strstream MutexName;
    int  MutexId;
    bool bIsLocked;
    TM_MUTEX_DECLARE(mutex);
};

//--------------------------------------------------------------------------------
// Semaphore class          -> The class of semaphore
// It's members:
//      Semaphore()     	-> The Semaphore's constructor with initial
//                                             semaphore value defaults 0.
//      ~Semaphore()        -> The Semaphore's destructor 
//      Signal()            -> Post a semaphore
//      Wait()              -> Wait a semaphore
//      SemaphoreId         -> Semaphore's id
//      SemaName            -> Semaphore's name
//      semaphore           -> The real semaphore
//--------------------------------------------------------------------------------

class Semaphore
{
public:
    Semaphore(const char* name="noname", int value=0);
    ~Semaphore();

    int Wait(int, char*);
    int Signal(int, char*);

private:
    mtu_strstream SemaName;
    int SemaphoreId;
    TM_SEMAPHORE_DECLARE(semaphore);
};

//--------------------------------------------------------------------------------
// Monitor class                -> The monitor class
// It's memebers:
//     Monitor()                -> Constructor
//     ~Monitor()               -> Destructor
//     void MonitorBegin(int ThreadID) -> Begin Monitor area
//     void MonitorEnd(int ThreadID)   -> End of Monitor area
//     void Destroy()           -> Destroy the monitor
//     int MonitorId            -> The id of the monitor
//     mtu_strstream MonitorName-> The name of this monitor
//     int MonitorType          -> The type of the monitor, HOARE or MESA
//     sem_t MutexLock          -> The mutex lock of this monitor
//     int Destroyed            -> If the monitor was detroied 
//     sem_t Next;              -> semaphore used to switch among threads
//     int NextCount;           -> the threads waiting after call condition signal, the 
//                              calling thread waits after signal and 
//                              switch to other thread
//     NOTE: Next and NextCount only used for Hoare style monitor
//     static Monitor* ParentAdd -> The address of this Monitor, so that
//                                      the Condition's parent pointer
//                                      will point to this address
//     static void SetParent()  -> Set Condition's parent pointer point to ParentAdd
//          
// Monitor::Condition class     -> The condition class belongs to Monitor
// It's members:
//       Condition(char *Name)  -> The constructor of Condition
//      ~Condition()            -> The destructor of Condition
//      Signal()                -> Signal a condition variable
//      Wait()                  -> Wait at a condition variable
//      Empty()                 -> return true if no threads waiting
//      Destroy()               -> Destroy a condition variable
//      parent                  -> Pointer point to its Monitor
//      Id                      -> The id of the condition variable
//      ConditionName           -> The name of this condition variable
//      condition               -> The real condition variable
//      ConditionWaitingCount   -> The threads waiting at condition wait
//      Destroyed               -> If the Condition variable is destroied
//--------------------------------------------------------------------------------

class Monitor 
{
protected:
    class Condition     // nested condition variable class
    {   
    public:
	    Condition(const char *name);    // force to use constructor
	    ~Condition();

        void Broadcast(int, char *);    // only for Mesa style monitor
        void Signal(int, char *);
	    void Wait(int, char *);
        bool IsEmpty(int, char *);      // return true if no threads waiting

    private:
        mtu_strstream ConditionName;
        //Monitor* pParent;
	    int ConditionId;
        int ConditionWaitingCount;  // the threads waiting at condition

        TM_SEMAPHORE_DECLARE(condition);
    };

public:
    Monitor(const char *name="noname", int type=HOARE);
    ~Monitor();

    void MonitorBegin(int, char*);
    void MonitorEnd(int, char*);

private:
    int MonitorType;
    int MonitorId;

    mtu_strstream MonitorName;

    TM_SEMAPHORE_DECLARE(monitor_sem);

    friend class Monitor::Condition;
    //friend class Monitor::PriorityCondition;

};      

//class Monitor 
//{
//public:
//    Monitor(const char *name="noname", int Type = HOARE);
//    ~Monitor();
//    void MonitorBegin(int, char*);
//    void MonitorEnd  (int, char*);
//    void Destroy();
//    int GetID();
//
//protected:
//    class Condition     // nested condition variable class
//    {   
//    public:
//	    Condition(const char *name="noname");
//	    ~Condition();
//	    void Signal   (int, char*);
//	    void Wait     (int, char*);
//        void Broadcast(int, char*); // only for Mesa style monitor
//        bool Empty    (int, char*); // return true if no threads waiting
//        int  parentMonitorID;
//    private:
//	    mtu_strstream ConditionName;
//        int ConditionId;
//	    int ConditionWaitingCount;  // the threads waiting at condition
//							        // wait
//	    Monitor* parent;
//        TM_SEMAPHORE_DECLARE(condition);
//    };
//
//private:
//    int MonitorType;
//    
//    TM_SEMAPHORE_DECLARE(MutexLock);
//    TM_SEMAPHORE_DECLARE(Next);
//
//    int NextCount;
//    int MonitorId;
//    mtu_strstream MonitorName;
//
//    friend class Monitor::Condition;
//    //friend class Monitor::PriorityCondition;
//    static Monitor* ParentAddr;         // the address of "this" pointer
//                    					// of Monitor, the Condition's
//					                    // parent should point to here
//    static void SetParent(Monitor **);  // set the Condition's parent
//					                    // pointer point to ParentAdd
//};


//------------------------------------------------------------------------------
// class ChannelMsgQueue:
//    
// Member Function:
//     ChannelMsgQueue()    -->	 constructor
//    ~OneToOneChannel()    -->	 destructor
//------------------------------------------------------------------------------

typedef struct _ChannelMsg_t // message in the channel
{
    Thread_t senderThreadId;
    size_t   msg_size;
    char    *msg_data;

} ChannelMsg_t;

class ChannelMsgQueue
{
public:
    ChannelMsgQueue(char *name, int capacity);
    ~ChannelMsgQueue();

    int Send   (void *, size_t, int, char *);
    int Receive(void *, size_t, int, char *);

    int Capacity();
    bool isEmpty();

private:
    std::queue<ChannelMsg_t> msg_queue;
    int capacity;

    TM_SEMAPHORE_DECLARE(msg_notfull);
    TM_SEMAPHORE_DECLARE(msg_notempty);
    TM_SEMAPHORE_DECLARE(msg_accesslock);

    int ChannelMsgQueueId;
    mtu_strstream MessageQName;
};


// Abstract Channel Class
class Channel
{
public:
    Channel(const char *name, int capacity); // constructor
    Channel(const char *name, int sender_uid, int recver_uid, int capacity);
    ~Channel();				           // destructor

      // message passing interface 
    virtual int Send   (void *, size_t, int, char *) {return 0;}
    virtual int Receive(void *, size_t, int, char *) {return 0;}

    // channel status query function
    int  Capacity();
    bool IsChannelEmpty();
    void SetChannelName(const char *name);

protected:
    mtu_strstream ChannelName;

protected:
    int ChannelId;

    int sender_thread_uid;
    int recver_thread_uid;
	
    bool is_sender_set;
	bool is_recver_set;

    ChannelMsgQueue *channelMsqQueue;
};


//-----------------------------------------------------------------------------------
// class SynOneToOneChannel:
//    support synchronous communication (blocking send, blocking recv) between two 
//    threads, it's like a one directional "pipe" with capacity 1, i.e., the channel
//    can hold only one message at any instant.
//    
// Member Function:
//    SynOneToOneChannel() 	-->	 constructor
//    ~SynOneToOneChannel()	-->	 destructor
//    SendMessage()		--> 	 send message throught channel
//    RecvMessage()		--> 	 recv message from channel
//    IsChannelEmpty()		--> 	 channel status query function, return true 
//    					 means YES, false means NO.
// Data Member:
//    Semaphore recver_arrived  -->      initial semaphore value = 0
//-----------------------------------------------------------------------------------
class SynOneToOneChannel : public Channel  // blocking send, blocking recv
{
public:
    SynOneToOneChannel(const char *name= "noname");	// constructor
    SynOneToOneChannel(const char *name, int sender_uid, int recver_uid);
    ~SynOneToOneChannel();					// destructor

      // message passing interface 
    int Send   (void *, size_t, int, char *);
    int Receive(void *, size_t, int, char *);

private:
    TM_SEMAPHORE_DECLARE(sem_recver_arrived);
};

//-----------------------------------------------------------------------------------
// class SynOneToOneChannel:
//    support synchronous communication (blocking send, blocking recv) between two 
//    threads, it's like a one directional "pipe" with capacity 1, i.e., the channel
//    can hold only one message at any instant.
//    
// Member Function:
//    SynOneToOneChannel() 	-->	 constructor
//    ~SynOneToOneChannel()	-->	 destructor
//    SendMessage()		--> 	 send message throught channel
//    RecvMessage()		--> 	 recv message from channel
//    IsChannelEmpty()		--> 	 channel status query function, return true 
//    					 means YES, false means NO.
// Data Member:
//    Semaphore recver_arrived  -->      initial semaphore value = 0
//-----------------------------------------------------------------------------------
class AsynOneToOneChannel : public Channel  // blocking send, blocking recv
{
public:
    AsynOneToOneChannel(const char *name= "noname", int capacity=INFINITE_CAPACITY);	// constructor
    AsynOneToOneChannel(const char *name, int sender_uid, int recver_uid, int capacity=INFINITE_CAPACITY);
    ~AsynOneToOneChannel();					// destructor

      // message passing interface 
    int Send   (void *, size_t, int, char *);
    int Receive(void *, size_t, int, char *);

private:
};








//------------------------------------------------------------------------------------
//                   Macro Definition
//------------------------------------------------------------------------------------                   

#ifdef TM_IN_DEVELOPEMENT // for development only..... /*REMOVE WHILE RELEASE*/
#define _THREADCLASS_CPP
#endif

#ifndef _THREADCLASS_CPP        // if this file is not used by ThreadClass.cpp
                                // we will add linenumber and filename for visual
#define Begin()         Begin(__LINE__, __FILE__)
#define Join()          Join(__LINE__, __FILE__)
#define Suspend()       Suspend(__LINE__, __FILE__)
#define Resume()        Resume(__LINE__, __FILE__)
#define SendUserMessage(MESSAGE, COLOR) SendUserMessage(MESSAGE, COLOR,__LINE__, __FILE__)

#define Signal()        Signal(__LINE__, __FILE__)
#define Broadcast()     Broadcast(__LINE__, __FILE__)
#define Trywait()       Trywait(__LINE__, __FILE__)
#define Wait()          Wait(__LINE__, __FILE__)
#define PriorityWait(Priority)  PriorityWait(Priority, __LINE__, __FILE__)
#define PrioritySignal()          PrioritySignal(__LINE__, __FILE__)
#define IsEmpty()       IsEmpty(__LINE__, __FILE__)

#define Lock()          Lock(__LINE__, __FILE__)
#define Trylock()       Trylock(__LINE__, __FILE__)
#define Unlock()        Unlock(__LINE__, __FILE__)

#define Readerlock()    Readerlock(__LINE__, __FILE__)
#define Writerlock()    Writerlock(__LINE__, __FILE__)
#define WriterUnlock()  WriterUnlock(__LINE__, __FILE__)
#define ReaderUnlock()  ReaderUnlock(__LINE__, __FILE__)

#define MonitorBegin()  MonitorBegin(__LINE__, __FILE__)
#define MonitorEnd()    MonitorEnd(__LINE__, __FILE__)

#define Send(x, y)              Send(x, y, __LINE__, __FILE__)
#define Receive(x, y)           Receive(x, y, __LINE__, __FILE__)
#define NonBlockingRecv(x, y)   NonBlockingRecv(x, y, __LINE__, __FILE__)

#define TopologySend(X,Y,Z)     TopologySend(X,Y,Z, __LINE__, __FILE__)
#define TopologyReceive(X,Y,Z)  TopologyReceive(X,Y,Z, __LINE__, __FILE__)

#define UserDefinedThreadID     UserDefinedThreadId

#endif

#endif          // for _THREADCLASS_H
