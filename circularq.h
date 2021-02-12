#ifndef CIRCULARQ_H

#define CIRCULARQ_H

#define BUFFSIZE 10

#define ITEMSIZE (sizeof(fpga_data))

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
    bool EnQEmpty(struct fpga_data** emptyPointer);
	
	/*
		returns the value in the front of the queue.
	*/
    bool GetFront(struct fpga_data** emptyPointer);
	
	/*
		deletes value from the front of the queue
	*/
	bool DeQueue();
	

    bool GetRear(fpga_data **emptyPointer);

	private:
	bool empty;
    fpga_data * increment(fpga_data *pointer);
    fpga_data * decrement(fpga_data *pointer);
	fpga_data * base;
	fpga_data * end;
	fpga_data * front;
	fpga_data * rear;
};




#endif
