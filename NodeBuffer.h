/*-------------------------------------------------------------------------*
 *---									---*
 *---		NodeBuffer.h						---*
 *---									---*
 *---	    This file defines a class that implements a thread-safe 	---*
 *---	buffer of pointers to math expressions.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2019 May 6		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/


class   NodeBuffer
{
  enum { SIZE   = 16 };

  Node*   array_[SIZE];
  int   inIndex_;
  int   outIndex_;
  int   numItems_;
  pthread_mutex_t       lockIt_;
  pthread_cond_t        notEmpty_;
  pthread_cond_t        notFull_;


public :

  NodeBuffer        ()
  {
    for  (int i = 0;  i < SIZE;  i++)
    {
      array_[i]	= NULL;
    }

    inIndex_ = outIndex_ = numItems_ = 0;
    pthread_mutex_init(&lockIt_,NULL);
    pthread_cond_init(&notEmpty_,NULL);
    pthread_cond_init(&notFull_,NULL);

  }

  ~NodeBuffer       ()
  {
    pthread_mutex_destroy(&lockIt_);
    pthread_cond_destroy(&notEmpty_);
    pthread_cond_destroy(&notFull_);
  }

  int   getNumItems  () const
  { return(numItems_); }

  void  putIn (Node* nodePtr)
  {
    pthread_mutex_lock(&lockIt_);
    while  (getNumItems() >= SIZE)
    {
      pthread_cond_wait(&notFull_,&lockIt_);
    }

    array_[inIndex_] = nodePtr;


    inIndex_++;
    numItems_++;
    if  (inIndex_ >= SIZE)
      inIndex_ = 0;
    pthread_cond_signal(&notEmpty_);
    pthread_mutex_unlock(&lockIt_);
  }

  Node*   pullOut ()
  {
   pthread_mutex_lock(&lockIt_);
    while  (getNumItems() <= 0)
    {
      pthread_cond_wait(&notEmpty_,&lockIt_);

    }

    Node* toReturn        = array_[outIndex_];

    array_[outIndex_]	  = NULL;

    outIndex_++;
    numItems_--;
    if  (outIndex_ >= SIZE)
      outIndex_ = 0; 
	  
    pthread_cond_signal(&notFull_);
    pthread_mutex_unlock(&lockIt_);

    return(toReturn);
  }
};
