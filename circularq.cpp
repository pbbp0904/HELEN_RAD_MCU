
#include <iostream>
#include "circularq.h"
#include <stdlib.h>

//const unsigned long ITEMSIZE = (sizeof(struct fpga_data));

CircularQ::CircularQ()
{
	base =(fpga_data*) malloc(ITEMSIZE*BUFFSIZE);
	end=base+(BUFFSIZE-1);
	front=base;
	rear=base;
	empty=1;
	
	//printf("the base address is %lX\n",base);
	//printf("The end address is %lX\n",end);
	//printf("The itemsize is %lX\n",ITEMSIZE);
}
CircularQ::~CircularQ()
{
	free(base);
}

//check if the buffer is full
bool CircularQ::IsFull()
{
	bool full=(increment(front)==rear);
	return full;
}

//check if the buffer is full
bool CircularQ::IsEmpty()
{
	return empty;
}

bool CircularQ::OneOrLess()
{
	bool one=front==rear;
	return one;
}

fpga_data *  CircularQ::increment(fpga_data *pointer)
{
	if(pointer==end)
	{
		return base;
	}
	else
	{
		return (pointer+1);
	}
}
fpga_data * CircularQ::decrement(fpga_data *pointer)
{
	if(pointer==base)
	{
		return end;
	}
	else
	{
		return (pointer-1);
	}
}

	/*
		takes an empty pointer to a buff pointer then sets its value to the rear value on the queue
		you may write to the return pointer to write to the the queue. returns 0 if full.
	*/
	bool CircularQ::EnQEmpty(fpga_data **emptyPointer)
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
	
	//deletes item off front of queue returns zero if empty
	bool CircularQ::DeQueue()
	{
		if (empty)
		{
			return 0;
		}
		if(front==rear)
		{
			empty=1;
			return 1;
		}
		
		front=decrement(front);
		return 1;
	}
	
	/*
		returns the value in the front of the queue.
	*/
	bool CircularQ::GetFront(fpga_data **emptyPointer)
	{
		if(empty)
			return 0;
		*emptyPointer=front;
		return 1;
	}
	
   /*
    * gets value at the rear of the queue
    */
    bool CircularQ::GetRear(fpga_data **emptyPointer)
    {
        if(empty)
            return 0;
        *emptyPointer=rear;
        return 1;
    }

















