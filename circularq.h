#ifndef CIRCULARQ_H

#define CIRCULARQ_H

#define BUFFSIZE 10

#define ITEMSIZE (sizeof(struct buff_t))

class CircularQ{
	public:
	CircularQ();
	~CircularQ();
	bool IsFull();//check if the buffer is full
	
	bool IsEmpty();//check if the buffer is Empty
	
	/*
		takes an empty pointer to a buff pointer then sets its value to the rear value on the queue
		you may write to the return pointer to write to the the queue. returns 0 if full.
	*/
	bool EnQEmpty(struct buff_t **emptyPointer);
	
	/*
		returns the value in the front of the queue.
	*/
	bool GetFront(struct buff_t **emptyPointer);
	
	/*
		deletes value from the front of the queue
	*/
	bool DeQueue();
	
	private:
	bool empty;
	struct buff_t * increment(buff_t *pointer);
	struct buff_t * decrement(buff_t *pointer);
	struct buff_t * base;
	struct buff_t * end;
	struct buff_t * front;
	struct buff_t * rear;
};




#endif