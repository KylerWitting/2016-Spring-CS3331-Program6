//--------------------------------------------------------------------------
//    ThreadMentor: Thread Visualization System
//  (c)2000-2001 Michigan Technological University
//
// Filename: defmsg.h
//      Define all the message types, and other definition about 
// 		message.
//--------------------------------------------------------------------------
#ifndef _DEFMSG_H
#define _DEFMSG_H

// the two types of RWLock:
#define WRITER 1        // wirter has higher priority
#define READER 2        // reader has higher priority

// the two types of Monitor
#define HOARE	0
#define MESA	1
#define MR_NOSIGNAL     4
#define MR_SIGNAL       5

const int MSG_SIZE= 100;

// Message for Thread
const long Msg_CreateThread     =  1;
const long Msg_TerminateThread  =  2;
const long Msg_JoinThread	=  3;

// Message for Semaphore
const long Msg_SemaphoreCreate  =  4;
const long Msg_SemaphoreDestroy =  5;
const long Msg_SemaphoreSignal  =  6;
const long Msg_SemaphoreWait    =  7;
const long Msg_SemaphoreGot     =  8;

const long Msg_MutexCreate  =9;
const long Msg_MutexDestroy =10;
const long Msg_MutexLock    =11;
const long Msg_MutexUnlock  =12;
const long Msg_MutexOwnlock =13;

// Message for RWLock
const long Msg_RWLockCreate    =14;
const long Msg_RWLockDestroy   =15;
const long Msg_RWLockReaderLock=16;
const long Msg_RWLockWriterLock=17;
const long Msg_RWLockReaderGot =18;
const long Msg_RWLockWriterGot =19;
const long Msg_RWLockReaderUnlock=20;
const long Msg_RWLockWriterUnlock=21;

// Message for Barrier
const long Msg_BarrierCreate =22;
const long Msg_BarrierDestroy=23;
const long Msg_BarrierWait   =24;
const long Msg_BarrierLeaving=25;

// Message for Monitor
const long Msg_MonitorCreate =26;
const long Msg_MonitorDestroy=27;
const long Msg_MonitorBegin  =28;
const long Msg_MonitorEnd    =29;
const long Msg_MonitorWait   =30;
const long Msg_MonitorSignal =31;
const long Msg_MonitorBroadcast=32;

// Message for Monitor::Condition
const long Msg_ConditionCreate =33;
const long Msg_ConditionDestroy=34;
const long Msg_ConditionWait   =35;
const long Msg_ConditionGot    =36;
const long Msg_ConditionSignal =37;
const long Msg_SignalReceive   =38;

const long Msg_ConditionBroadcast       = 39;
const long Msg_ConditionWaitNothing     = 40;
const long Msg_ConditionFinishWaitNothing=41;

// Message for MessageQueue
const long Msg_MessageQCreate   =42;
const long Msg_MessageQSendBegin=43;
const long Msg_MessageQSendEnd  =44;
const long Msg_MessageQReceiveBegin=45;
const long Msg_MessageQReceiveEnd  =46;
const long Msg_MessageQDestroy     =47;

// the sturct of a message
typedef struct {
	long messagetype;		// type
	char text[MSG_SIZE];		
} TMsg;

// new message types added to support thread suspend & resume
const long Msg_SuspendThread    =  48;   
const long Msg_ResumeThread     =  49;
const long Msg_ResumeSignal     =  50;

// message type used to support user defined message
const long Msg_UsrMessage       =  51;

// message type used to support Asynchronous one to one channel
const long Msg_AsynOneToOneChannelCreate =  52;
const long Msg_AsynOneToOneChannelSend   =  53;
const long Msg_AsynOneToOneChannelRecv   =  54;
const long Msg_AsynOneToOneChannelDestroy = 55;

//message type used to support Synchronous one to one channel
const long Msg_SynOneToOneChannelCreate  =  56;
const long Msg_SynOneToOneChannelSend    =  57;
const long Msg_SynOneToOneChannelRecv    =  58;
const long Msg_SynOneToOneChannelAck     =  59;
const long Msg_SynOneToOneChannelDestroy =  60;

const long Msg_ChannelRecvWait =  62;
const long Msg_SynManyToOneChannelCreate  =  63;
const long Msg_AsynManyToOneChannelCreate  = 64;


// message type used to support synchronous many to many channel
const long Msg_SynManyToManyChannelCreate  =  61;
const long Msg_SynManyToManyChannelSend    =  62;
const long Msg_SynManyToManyChannelRecv    =  63;
const long Msg_SynManyToManyChannelDestroy =  64;

// message type used to support synchronous many to many channel
const long Msg_AsynManyToManyChannelCreate  =  65;
const long Msg_AsynManyToManyChannelSend    =  66;
const long Msg_AsynManyToManyChannelRecv    =  67;
const long Msg_AsynManyToManyChannelDestroy =  68;

const int tmColorRed     = 0;   // simple gradient red bar
const int tmColorOrange  = 1;   // simple gradient orange bar
const int tmColorYellow  = 2;   // simple gradient yellow bar
const int tmColorGreen   = 3;   // simple gradient green bar
const int tmColorCyan    = 4;   // simple gradient cyan bar
const int tmColorBlue    = 5;   // simple gradient blue bar
const int tmColorPurple  = 6;   // simple purple (the default user color)
const int tmColorWhite   = 7;   // simple gradient white bar
const int tmColorStripe  = 8;   // black bar with a white stripe
const int tmColorRainbow = 9;   // rainbow bar (crazy but noticable)
const int tmColorNone    = 10;  // no bar (used mostly by system)

#endif
