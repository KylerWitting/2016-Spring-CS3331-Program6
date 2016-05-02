//--------------------------------------------------------------------------
//    ThreadMentor: Thread Visualization System
//  (c)2000-2001 Michigan Technological University
//
// Filename: ThreadType.h
//     Platform dependent type redefitions
//--------------------------------------------------------------------------
#ifndef _THREADTYPE_H
#define _THREADTYPE_H

//--------------------------------------------------------------------------------
//                          OS dependent macro defines
//--------------------------------------------------------------------------------

// For the solaris thread

#ifdef SOLARIS_THREAD
#define Thread_t                        thread_t
#define pthread_t                       thread_t
#define pthread_exit(s)                 thr_exit(s)
#define sched_yield()                   thr_yield()
#define thread_self()                   thr_self()

#define pthread_mutex_t                 mutex_t
#define pthread_mutex_lock(s)           mutex_lock(s)
#define pthread_mutex_unlock(s)         mutex_unlock(s)
#define pthread_mutex_destroy(s)        mutex_destroy(s)

#define sem_t                           sema_t
#define sem_wait(s)                     sema_wait(s)
#define sem_post(s)                     sema_post(s)
#define sem_destroy(s)                  sema_destroy(s)

#define pthread_cond_t                  cond_t
#define pthread_cond_wait(x, y)         cond_wait(x,y)
#define pthread_cond_signal(x)          cond_signal(x)
#define pthread_cond_broadcast(x)       cond_broadcast(x)
#define pthread_cond_destroy(x)         cond_destroy(x)
#endif

// For the PTHREAD

#ifdef PTHREAD
#define Thread_t                        pthread_t
#define thread_self()                   pthread_self()
#endif

// For the WIN32_THREAD thread

#ifdef WIN32
#define Thread_t                        unsigned int //DWORD
#define pthread_t                       unsigned int //DWORD
#define thread_self()                   GetCurrentThreadId()
#endif

#endif 








