//--------------------------------------------------------------------------------
//  ThreadMentor: Thread Visualization System
//  (c)2003-2004 Michigan Technological University
//
//  File: 
//     mtuStrstream.h
//--------------------------------------------------------------------------------
#ifndef _MTUSTRSTREAM_H
#define _MTUSTRSTREAM_H

#include <iostream>
#include <strstream>
#include <string>

using namespace std;

// Wrap-up class used to fix VC.NET strstream problem

class mtuStrstream
{
public:
    mtuStrstream(void);
    ~mtuStrstream(void);

    mtuStrstream &operator << (mtuStrstream &);
    mtuStrstream &operator << (strstream &);
    mtuStrstream &operator << (const char *);
    mtuStrstream &operator << (char);
    mtuStrstream &operator << (unsigned char);
    mtuStrstream &operator << (short);
    mtuStrstream &operator << (unsigned short);
    mtuStrstream &operator << (int);
    mtuStrstream &operator << (unsigned int);
    mtuStrstream &operator << (long);
    mtuStrstream &operator << (unsigned long);
    mtuStrstream &operator << (float);
    mtuStrstream &operator << (double);

    char *str();
    void seekp(int, ios_base::seekdir); // Only reset to zero for now

private:
    string Str;
};

// Replace strstream if uses VC.NET
#ifdef VC_NET
typedef mtuStrstream mtu_strstream;
typedef mtuStrstream mtu_ostrstream;
#else
typedef strstream mtu_strstream;
typedef ostrstream mtu_ostrstream;
#endif

#endif // end _MTUSTRSTREAM_H

