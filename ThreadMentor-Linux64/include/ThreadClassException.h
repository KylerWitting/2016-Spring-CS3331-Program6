//---------------------------------------------------------------------------------------------
//    ThreadMentor: Thread Visualization System
//  (c)2000-2001 Michigan Technological University
// 
// FILE: ThreadClassException.h
// 
//  -- Define all the exception class in the ThreadClass Library
//     to support exception handling.
//  -- Define all the error messages for thread operation and
//     syn. primitive operation
//  -- The file replace the previous error message definition file
//     mtuThreadErrmsg.h 
//---------------------------------------------------------------------------------------------
#ifndef _TC_EXCEPTION_H
#define _TC_EXCEPTION_H

#define TM_ERROR  -1
#define TM_SUCCESS 0

#define ESBUF 1
#define SNOTOWNER 2
#define SNOID 3
#define NEGSSIZE 4
#define ERBUF 5
#define RNOTOWNER 6
#define RNOID 7
#define NEGRSIZE 8
#define RSSYNM2ONE 9
#define MUTEXOTL 10
#define MUTEXNOUNL 11
#define MTSID 12
#define UIDNO 13
#define NTNO 14
#define RDULNOTO 15
#define WRULNOTO 16

extern unsigned int err_no;

//*************************************************************************
// ThreadMentor Exception Handling rules
//
// (1) Whenever you use a function type of integer, you should
//     check if it returns TM_ERROR (TM_ERROR is return when exception
//     occurs). If yes, GetLastErrMsg(const char *prefix) should be
//     called to get the error message.
//
//     for example:
//      if(channel.Send(sendbuf, sendbuf_size) == TM_ERROR)
//      {
//         GetLastErrorMsg("Send");
//         exit(0);
//      }
//      is a suggested way of coding for using Send function in Channel
//
// (2) If a function return other type (void, boolean, etc), the system
//     will do error checking for you. If deadly error occurs (for example,
//     out of memory), an appropriate error message will be given and program
//     will be terminated by the System. On the other hand, if only minor error
//     occurs, the System may just give a warning message, and the program
//     will keep running.
//
// (3) Pay attention to the error messages:
//     -- if a error occur, the execution results are meanless
//     -- if a warning message, pay attention to your results, it may not be
//        what you want.
//
//   --- cfang   July 2003     
//*************************************************************************     


void GetLastErrMsg(const char *prefix);



//********************************************************
// the following classes are designed for System Use ONly


class ThreadException
{
public:
    void mGetErrorMessage(){};
};


class ThreadProblem : public ThreadException
{
    public:
        ThreadProblem(int arg_code);
};


class MutexProblem : public ThreadException
{
    public:
        MutexProblem(int arg_code);
};


class SemaphoreProblem : public ThreadException
{
    public:
        SemaphoreProblem(int arg_code);
};


class BarrierProblem : public ThreadException
{
    public:
        BarrierProblem(int arg_code);
};


class RWLockProblem : public ThreadException
{
    public:
        RWLockProblem(int arg_code);
};


class MonitorProblem : public ThreadException
{
    public:
        MonitorProblem(int arg_code);
};



class ChannelProblem : public ThreadException
{
    public:
         ChannelProblem(int arg_code);
         
};


#endif //_TC_EXCEPTION_H

