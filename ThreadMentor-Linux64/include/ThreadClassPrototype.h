//----------------------------------------------------------------------------------------
//  ThreadMentor: Thread Visualization System
//  (c)2000-2001 Michigan Technological University
//
// File: 
//    ThreadClassPrototype.h
// Program Description:
//      1. This file contains most platform dependent
//         code, we use "generic" macros to provide
//         a uniform interface for thread class.
//      2. There are 4 major categories of macro
//         <1>. thread related macro.
//         <2>. Message passing macro.
//         <3>. System initialization macro.
//         <4>. Message Queue initilization macro.
//      3. This file also contains some global variables
//----------------------------------------------------------------------------------------
#ifndef _TC_PROTOTYPE_H
#define _TC_PROTOTYPE_H

#include <exception>
#include "ThreadClassException.h"

//----------------------------------------------------------------------------------------
// Using "generic" functions of thread operation to hide the underlying
// platform-dependent thread functions


//----------------------------------------------------------------------------------------
// Using "generic" functions of thread operation to hide the underlying
// platform-dependent thread functions

#define mtu_Thread_creation()   __mtu_THREAD_CREATION
#define mtu_Thread_exit()       __mtu_THREAD_EXIT
#define mtu_Thread_join()       __mtu_THREAD_JOIN
#define mtu_Thread_suspend()    __mtu_THREAD_SUSPEND
#define mtu_Thread_resume()     __mtu_THREAD_RESUME

//---------------------------------------------------------------------------------------
// Macros for Thread Operation
// NOTE:
//    1. For "thread suspend operation":
//       Solaris thread: "thr_suspend()", thr_continue()
//           - success: return 0; 
//           - failure: return non-zero to indicate error
//       Win32 thread  : SuspendThread(), ResumeThread():
//           - success: return previous suspend count;
//           - failure: return -1
//       Posix thread  : "sigwait()"
//           - success: return 0;
//           - failure: return -1;
//       Mtu Thread    : "THREAD_SUSPEND()"
//           - success : return 0
//           - failure : return -1
// -------------------------------------------------------------------------------------          

// solaris macro

#if defined(SOLARIS_THREAD)

    #define __mtu_THREAD_CREATION                   \
    {                                               \
        if(thr_create(NULL, 0, (void *(*)(void *))Real_func, (void*)this, 0, &RealThreadId))   \
            throw ThreadProblem(0);                 \
    }
    #define __mtu_THREAD_EXIT       thr_exit(NULL)
    #define __mtu_THREAD_JOIN       return thr_join(RealThreadId, 0, 0)
    #define __mtu_THREAD_SUSPEND                    \
    {                                               \
        if(ret_value = thr_suspend(RealThreadId))   \
            throw ThreadProblem(2);                 \
    }
    #define __mtu_THREAD_RESUME                     \
    {                                               \
        if(ret_value = (int)thr_continue(RealThreadId)) \
            throw ThreadProblem(3);                 \
    }

// pthread macro

#elif defined(PTHREAD)

    #define __mtu_THREAD_CREATION                   \
    {                                               \
        if(pthread_create(&RealThreadId, NULL,(void *(*)(void *))Real_func, (void *)this) )   \
            throw ThreadProblem(0);                 \
    }
    #define __mtu_THREAD_EXIT       pthread_exit(NULL)
    #define __mtu_THREAD_JOIN       return pthread_join(RealThreadId, 0)
    #define __mtu_THREAD_SUSPEND                    \
    {                                               \
        fprintf(stderr, "SORRY, we didn't support thread_suspend for Pthread\n");   \
        exit(-1);                                   \
    }
    #define __mtu_THREAD_RESUME                     \
    {                                               \
        fprintf(stderr, "SORRY, we didn't support thread_resume for Pthread\n");    \
        exit(-1);                                   \
    }

// WIN32_THREAD macro

#elif defined(WIN32_THREAD)

    #define __mtu_THREAD_CREATION                   \
    {                                               \
        if(!(hThread = (HANDLE)_beginthreadex(NULL, 0, Real_func, (LPVOID)this, 0, &RealThreadId)))  \
            throw ThreadProblem(0);                 \
    }
    #define __mtu_THREAD_EXIT       _endthreadex(RealThreadId)
    #define __mtu_THREAD_JOIN                       \
    {                                               \
        if(WaitForSingleObject(hThread, INFINITE)== WAIT_FAILED)    \
           return FAILURE;                          \
        else                                        \
           return SUCCESS;                          \
    }
    #define __mtu_THREAD_SUSPEND                    \
    {                                               \
        if(SuspendThread(hThread) < 0)              \
            throw ThreadProblem(2);                 \
    }
    #define __mtu_THREAD_RESUME                     \
    {                                               \
        if(ResumeThread(hThread) < 0)               \
            throw ThreadProblem(3);                 \
    }

#endif // end of create/ exit/ join/ suspend/ resume macros

// --------------------------------------------------------------
// For Message passing between thread library and visual system
// --------------------------------------------------------------

#ifdef WIN32_THREAD

    #define TM_MSGQ_DECLARE()                       \
        TMsg buf;                                   \
        mtuMessageQueue *winmsg;                    

    #define TM_MSGQ_INIT()                          \
        if (!(winmsg=new mtuMessageQueue("ThrToVis")))   \
        {                                           \
            printf("Cannot create MsgQueue!\n");    \
            exit(-1);                               \
        }                                           

#elif defined(SOLARIS_THREAD) || defined(PTHREAD)

    #define TM_MSGQ_DECLARE()                       \
        TMsg buf;                                   \
        key_t msg_key;                              \
        int msg_qID;                                \
        TM_MUTEX_DECLARE(msg_lock);

    #define TM_MSGQ_INIT()                          \
    {                                               \
        TM_MUTEX_CREATE(msg_lock);                  \
        msg_key = ftok(".", 's');                   \
		/* clear previous msg queue (if exist) */	\
		if ((msg_qID = msgget(msg_key, IPC_CREAT | 0666))!=-1) \
			msgctl(msg_qID, IPC_RMID, 0);			\
		/* create the msg queue */					\
        if ((msg_qID = msgget(msg_key, IPC_CREAT | 0666)) < 0)  \
        {                                           \
            TmMsgQueueExist = 0;                    \
            printf("Cannot Create MsgQueue!\n");    \
        }                                           \
        else                                        \
            TmMsgQueueExist = 1;                    \
    }

#endif // end msg queue stuff

// --------------------------------------------------------------
// synchronization blinding : Mutex
// --------------------------------------------------------------

#ifndef _TM_MUTEX_
#define _TM_MUTEX_

#ifdef WIN32_THREAD
    #define TM_MUTEX_DECLARE(x) HANDLE x
    #define TM_MUTEX_CREATE(x)  {if (!(x = CreateMutex(NULL, FALSE, NULL))) throw MutexProblem(0);}
    #define TM_MUTEX_DESTROY(x) CloseHandle(x)
    #define TM_MUTEX_LOCK(x)    {if (x!=NULL) WaitForSingleObject(x, INFINITE);}
    #define TM_MUTEX_UNLOCK(x)  {if (x!=NULL) ReleaseMutex(x);}

#elif defined(SOLARIS_THREAD)        
    #define TM_MUTEX_DECLARE(x) mutex_t x
    #define TM_MUTEX_CREATE(x)  {if (mutex_init(&x, USYNC_THREAD, (void *) NULL)) throw MutexProblem(0);}
    #define TM_MUTEX_DESTROY(x) mutex_destroy(&x)
    #define TM_MUTEX_LOCK(x)    mutex_lock(&x)
    #define TM_MUTEX_UNLOCK(x)  mutex_unlock(&x)

#elif defined(PTHREAD)        
    #define TM_MUTEX_DECLARE(x) pthread_mutex_t x
    #define TM_MUTEX_CREATE(x)  {if (pthread_mutex_init(&x, 0)) throw MutexProblem(0);}
    #define TM_MUTEX_DESTROY(x) pthread_mutex_destroy(&x)
    #define TM_MUTEX_LOCK(x)    pthread_mutex_lock(&x)
    #define TM_MUTEX_UNLOCK(x)  pthread_mutex_unlock(&x)
#endif                    

#endif // end of _TM_MUTEX_

// --------------------------------------------------------------
// synchronization blinding : Semaphore
// --------------------------------------------------------------

#ifndef _TM_SEMAPHORE_
#define _TM_SEMAPHORE_

#ifdef WIN32_THREAD
    #define TM_SEMAPHORE_DECLARE(x)     HANDLE x
    #define TM_SEMAPHORE_CREATE(x,v)    {if (!(x = CreateSemaphore(NULL, v, MAX_SEMVALUE, NULL))) throw SemaphoreProblem(0);}
    #define TM_SEMAPHORE_DESTROY(x)     CloseHandle(x)
    #define TM_SEMAPHORE_WAIT(x)        {if (x!=NULL) WaitForSingleObject(x, INFINITE);}
    #define TM_SEMAPHORE_SIGNAL(x)      {if (x!=NULL) ReleaseSemaphore(x, 1, NULL);}

#elif defined(SOLARIS_THREAD)        
    #define TM_SEMAPHORE_DECLARE(x)     sem_t x
    #define TM_SEMAPHORE_CREATE(x,v)    {if (sema_init(&x, v, USYNC_THREAD, 0)) throw SemaphoreProblem(0);}
    #define TM_SEMAPHORE_DESTROY(x)     sem_destroy(&x)
    #define TM_SEMAPHORE_WAIT(x)        sem_wait(&x)
    #define TM_SEMAPHORE_SIGNAL(x)      sem_post(&x)

#elif defined(PTHREAD)        
    #define TM_SEMAPHORE_DECLARE(x)     sem_t x
    #define TM_SEMAPHORE_CREATE(x,v)    {if (sem_init(&x, 0, v)) throw SemaphoreProblem(0);}
    #define TM_SEMAPHORE_DESTROY(x)     sem_destroy(&x)
    #define TM_SEMAPHORE_WAIT(x)        sem_wait(&x)
    #define TM_SEMAPHORE_SIGNAL(x)      sem_post(&x)
#endif                    

#endif // end of _TM_SEMAPHORE_


#endif /* _TC_PROTOTYPE_H */
