//--------------------------------------------------------------------------
//    ThreadMentor: Thread Visualization System
//  (c)2000-2001 Michigan Technological University
// 
// File: ThreadTree.h
//
// ADT General Tree for library wide thread management
// Note:
//   1. Each level in the tree is a circular and doulbly
//      linked list. The eldest Left_child of a level is the 
//      head of the linked list, each Left_child in a level
//      points to its parent, and the parent of those 
//      Left_children points to the eldest Left_child
//   2. Any node can have any number of children.
//   3. Any thread created by the current thread is the
//      children of the current thread.
//   4. The primary thread of a process is the root of 
//      the subtree for all the threads created in this
//      process's address space.
//-------------------------------------------------------------------------------
#ifndef _THREADTREE_H
#define _THREADTREE_H

#include "TreeNode.h"

#ifdef WIN32_THREAD	
   #define LOCK_MUTEX() WaitForSingleObject( hMutex, INFINITE)
#endif					
#if defined(SOLARIS_THREAD)			
   #define LOCK_MUTEX()  mutex_lock(&mutex)			
#endif					
#if defined(PTHREAD)				
   #define LOCK_MUTEX()  pthread_mutex_lock(&mutex);		
#endif					
#ifdef mtuTHREAD 				
   #define LOCK_MUTEX()  MUTEX_LOCK(mutex);			
#endif					
   
       
#ifdef WIN32_THREAD				
   #define UNLOCK_MUTEX()  ReleaseMutex(hMutex)			
#endif					
#if defined(SOLARIS_THREAD) 			
   #define UNLOCK_MUTEX()  mutex_unlock(&mutex)			
#endif					
#if defined(PTHREAD)				
   #define UNLOCK_MUTEX()  pthread_mutex_unlock(&mutex)		
#endif					
#ifdef mtuTHREAD 				
   #define UNLOCK_MUTEX()  MUTEX_UNLOCK(mutex)			
#endif					
  
// -------------------------------------------------------------------------------
// definition for the ThreadTree class
// -------------------------------------------------------------------------------
class ThreadTree
{
public:
    enum  { NO_ERR = 0, ERR = -1};		    // return codes for tree functions
    // constructor and destructor
    ThreadTree() ;
    ThreadTree(treeNodePtr node) :  root_(node)
    	{ number_of_nodes_ = 1; number_of_levels_ = 1;}
    ~ThreadTree();
     
    // "predicate" member function
    bool IsEmpty(){ return root_ == NULL ? true : false; }
    
    // access member function
    int Number_of_nodes() ;   		 // How many nodes are in the tree
    int Number_of_levels();  // How many levels (generations) are there in the tree? 
    
    // nchildren: Returns the number of children that are direct offspring of the specified 
    // node (or current node if none is specified).  
    int nchildren(treeNodePtr node) ;
    
    // nsiblings: Returns the number of nodes at the same level as the specified or current
    // node. This number includes the specified or current node.
    int nsiblings(treeNodePtr node) ;
    
    // tree operation member function
    bool Remove(int thread_id);
    int Insert(treeNodePtr newnode, treeNodePtr oldnode);
    treeNodePtr Search(int thread_id, treeNodePtr) const;
    void deleteTree(void);

    // traversal member function
    // typesafes on iteration methods.  These call the built-in iterator then 
    // return the contents of the now-current node.  They do not affect the state
    // of the tree.
    treeNodePtr root(void)                
        {return root_;     }
    treeNodePtr prev(treeNodePtr node)    
        {return(node ? (node=node->prev) : (treeNodePtr)0);}
    treeNodePtr next(treeNodePtr node)    
        {return(node ? (node=node->next) : (treeNodePtr)0);}
    treeNodePtr parent(treeNodePtr node)  
        {return(node ? (node=node->parent) : (treeNodePtr)0);}
    treeNodePtr child(treeNodePtr node)   
        {return(node ? (node=node->child) : (treeNodePtr)0);}
    treeNodePtr eldest(treeNodePtr node);
    treeNodePtr youngest(treeNodePtr node);

    // Debugging aid functions
    void dump(void) const;
    
private:
    
    treeNodePtr  root_;	   	// the root node of the tree
    int number_of_nodes_;	// number of nodes,
    int number_of_levels_;	// number of levels in tree
    /* mutex for exclusive thread tree operation */
#ifdef WIN32_THREAD
    HANDLE hMutex;
#endif
#if defined(SOLARIS_THREAD)
    mutex_t mutex;
#endif
#if defined(PTHREAD) 
    pthread_mutex_t mutex;
#endif
#if defined(mtuTHREAD)
    MUTEX_t mutex;
#endif

    
};

#endif  // _THREADTREE_H

