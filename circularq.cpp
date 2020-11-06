
#include "circularq.h"
#include <stdlib.h>
//const unsigned long ITEMSIZE = (sizeof(struct buff_t));

CircularQ::CircularQ()
{
	base =(struct buff_t*) malloc(ITEMSIZE*BUFFSIZE);
	end=base+ITEMSIZE*BUFFSIZE;
	front=base;
	rear=decrement(base);
	empty=1;
}
CircularQ::~CircularQ()
{
	delete base;
}

//check if the buffer is full
bool CircularQ::IsFull()
{
	bool full=(((struct buff_t*)increment(front))==rear);
	return full;
}

//check if the buffer is full
bool CircularQ::IsEmpty()
{
	return empty;
}



struct buff_t *  CircularQ::increment(buff_t *pointer)
{
	if(pointer==end)
	{
		return base;
	}
	else
	{
		return (pointer+ITEMSIZE);
	}
}
struct buff_t * CircularQ::decrement(buff_t *pointer)
{
	if(pointer==base)
	{
		return end;
	}
	else
	{
		return (pointer-ITEMSIZE);
	}
}

	/*
		takes an empty pointer to a buff pointer then sets its value to the rear value on the queue
		you may write to the return pointer to write to the the queue. returns 0 if full.
	*/
	bool CircularQ::EnQEmpty(struct buff_t **emptyPointer)
	{
		if(empty)
		{
			*emptyPointer=rear;
			empty=0;
			return 1;
		}
		if(IsFull())
		{
			return 0;
		}
		
		rear=decrement(rear);
		*emptyPointer=rear;
		return 1;
	}
	
	//deletes item off front of queue
	void CircularQ::DeQueue()
	{
		if(front==rear)
		{
			empty=1;
			return;
		}
		decrement(front);
	}
	
	/*
		returns the value in the front of the queue.
	*/
	bool CircularQ::GetFront(struct buff_t **emptyPointer)
	{
		*emptyPointer=front;
	}
	



















