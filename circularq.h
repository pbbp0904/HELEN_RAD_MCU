#ifndef CIRCULARQ_H

#define CIRCULARQ_H

#define BUFFSIZE 10

#define ITEMSIZE (sizeof(buff_t))

#include "fpga.h"

class CircularQ{
	public:
	CircularQ();
	~CircularQ();
	bool IsFull();//check if the buffer is full
	
	bool OneOrLess();//checks if the buffer has one or less value
	
	bool IsEmpty();//check if the buffer is Empty
	
	/*
		takes an empty pointer to a buff pointer then sets its value to the rear value on the queue
		you may write to the return pointer to write to the the queue. returns 0 if full.
	*/
    bool EnQEmpty(struct buff_t** emptyPointer);
	
	/*
		returns the value in the front of the queue.
	*/
    bool GetFront(struct buff_t** emptyPointer);
	
	/*
		deletes value from the front of the queue
	*/
	bool DeQueue();
	

    bool GetRear(buff_t **emptyPointer);

	private:
	bool empty;
    buff_t * increment(buff_t *pointer);
    buff_t * decrement(buff_t *pointer);
	buff_t * base;
	buff_t * end;
	buff_t * front;
	buff_t * rear;
};




#endif
