
#include "hamming.h"
#include<iostream>
//hamming(63,57) plus p0 bit which gives 7 parity bits and 57 data bits
unsigned long long SECDED57(unsigned long long data)
{
	unsigned long long code = 0;
	
	int nump=0; //number of parity bits encountered in the loop
	int numpow=1;
	for(int i=1;i<63;i++)//place data on code bits
	{
		//checks for all powers of two to use as parity bits or else copies the data. bit zero is the only nonpower of two parity bit so the loop skips it
		if(i==numpow)
		{
			nump++;
			numpow=intpow(2,nump);
		}
		else
		{
			//printf("code is %llx,data is %llx, nump is %d i is %d\n", code,data,nump,i);
			code |= ((data<<nump) & (((unsigned long long )1)<<(i-1)));//effectivly sets bit i in code to bit i-nump in data.
			//printf("after equals code is %llx\n",code);
		}
	}
	printf("after first for loop %llx\n",code);
	for(int i=6;i>=0;i--)//setting parity bit i
	{
		setparity(&code,i);
	}
	printf("after second for loop %llx\n",code);
	//set parity bit zero
	code=code<<1;
	for(int i=1;i<64;i++)
	{
		code^=(1&(code>>i));
	}
	return code;
}
void setparity(unsigned long long * data,int pbit)
{
	int numpow = intpow(2,pbit-1);
		for(int j=0;j<63;j++)
		{
			if((j>>(pbit-1)) & 1)//check if bit i is on in this number
			{
				*data ^= (1&((*data)>>(j-1)))<<(numpow-1);//updates parity bit pbit with data from j-1
			}				
		}
}


int intpow(int base,int exp)
{
	int result = 1;
	for(int i=0;i<exp;i++)
	{
		result=result*base;
	}
	return result;
}




