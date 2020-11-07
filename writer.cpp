#include "writer.h"
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <QDebug>
#include "circularq.h"
#include "hamming.h"

#define ASCIIZERO 0x30

#define ONES57 0x01FFFFFFFFFFFFFF

#define FILESIZE_BITS 10000000

#define NUM_WRITES (FILESIZE_BITS/1088)

#define WRITE_ARRAY_LENGTH 20

Writer::Writer()
{
    hps = new HPS;
    fpga = new FPGA;

}

Writer::~Writer()
{
    qDebug() << "Exiting.";
    delete hps;
    delete fpga;
}

void Writer::DCCPolling(){

    //Creating Files
    datafile = fopen("data", "a");

    qDebug() << "Starting Data Collection...";

    uint32_t StartUp;
    StartUp = 0xffffffff;

    qDebug() << "result of write: " << fwrite(&StartUp, 4, 1, datafile);

    fclose(datafile);
    datafile = fopen("data", "a");
    DCCPoll();


}


/*
void Writer::DCCPoll(){
    fpga->ReadSet(1);

    while(1){
        // Reading Data
        fpga->ReadSet(0);
        fpga->DataRead(&buff[0]);
        fpga->ReadSet(1);
        if(!(buff[0].data_0==data_0_old) && !(buff[0].data_1==data_1_old) && !(buff[0].data_2==data_2_old) && !(buff[0].data_3==data_3_old) && !(buff[0].data_4==data_4_old) && !(buff[0].data_5==data_5_old)){
            //fwrite(&buff[0], 4, 34, datafile);
            //fwrite(&buff[0].data_4, 4, 1, datafile);
            fprintf(datafile, "%d,", buff[0].pps_count);
            fprintf(datafile, "%d,", buff[0].time);
            fprintf(datafile, "%d,", buff[0].data_0);
            fprintf(datafile, "%d,", buff[0].data_1);
            fprintf(datafile, "%d,", buff[0].data_2);
            fprintf(datafile, "%d,", buff[0].data_3);
            fprintf(datafile, "%d,", buff[0].data_4);
            fprintf(datafile, "%d,", buff[0].data_5);
            fprintf(datafile, "%d,", buff[0].data_6);
            fprintf(datafile, "%d,", buff[0].data_7);
            fprintf(datafile, "%d,", buff[0].data_8);
            fprintf(datafile, "%d,", buff[0].data_9);
            fprintf(datafile, "%d,", buff[0].data_10);
            fprintf(datafile, "%d,", buff[0].data_11);
            fprintf(datafile, "%d,", buff[0].data_12);
            fprintf(datafile, "%d,", buff[0].data_13);
            fprintf(datafile, "%d,", buff[0].data_14);
            fprintf(datafile, "%d,", buff[0].data_15);
            fprintf(datafile, "%d,", buff[0].data_16);
            fprintf(datafile, "%d,", buff[0].data_17);
            fprintf(datafile, "%d,", buff[0].data_18);
            fprintf(datafile, "%d,", buff[0].data_19);
            fprintf(datafile, "%d,", buff[0].data_20);
            fprintf(datafile, "%d,", buff[0].data_21);
            fprintf(datafile, "%d,", buff[0].data_22);
            fprintf(datafile, "%d,", buff[0].data_23);
            fprintf(datafile, "%d,", buff[0].data_24);
            fprintf(datafile, "%d,", buff[0].data_25);
            fprintf(datafile, "%d,", buff[0].data_26);
            fprintf(datafile, "%d,", buff[0].data_27);
            fprintf(datafile, "%d,", buff[0].data_28);
            fprintf(datafile, "%d,", buff[0].data_29);
            fprintf(datafile, "%d,", buff[0].data_30);
            fprintf(datafile, "%d\n", buff[0].data_31);
        }
        data_0_old=buff[0].data_0;
        data_1_old=buff[0].data_1;
        data_2_old=buff[0].data_2;
        data_3_old=buff[0].data_3;
        data_4_old=buff[0].data_4;
        data_5_old=buff[0].data_5;
    }
}

*/

void Writer::DCCPoll(){
    fpga->ReadSet(1);
	que.EnQEmpty(&buff);//add buff to front of que
    while(1)
	{
        // Reading Data
        fpga->ReadSet(0);
        fpga->DataRead(buff);//read data into buff
        fpga->ReadSet(1);
		if((!IsNewData(buff))&&!que.OneOrLess())
		{
			que.GetFront(&buff);//gets value off the front of the que
			WriteSD(&buff);//todo make this function
			que.DeQueue();
		}
		else if(IsNewData(buff))
		{
			if(que.IsFull())
			{
			que.GetFront(&buff);//gets value off the front of the que
			WriteSD(&buff);//todo make this function
			que.DeQueue();
			}
			EnQEmpty(&buff);
		}
	}

}
bool Writer::IsNewData(fpga_data * data)
{
	if(data->data[31]==0xffffffff)
		return 0;
	return 1;
}

void Writer::WriteSD(fpga_data * data)
{
	unsigned long long wa[WRITE_ARRAY_LENGTH];
	Format57(data,wa);//formats the data into array of 57 bit data to encode
	
	static unsigned long long numwrites=0;//stores the number of writes that have happend
	numwrites++;
	int fnumber=numwrites/NUM_WRITES;
	
	char fname[]="data000.bin\0";
	
	GetFName(fnumber,fname);//writes over the last 3 charecters of the array with digits of the number
	
	std::ofstream file;
	file.open(fname,std::ios::binary|std::ios::app);
	for(int i=0;i<WRITE_ARRAY_LENGTH;i++)
	{
		SECDEC57(wa+i);//Encodes the info in a hamming code
		myfile.write((char *)(wa+i),sizeof(long long));
	}
	file.close();
}


//formats fpga_data struct into an array of 57 bit data packets
void Writer::Format57(fpga_data * data, unsigned long long * arr)
{
	int ncopy = 0;
	int bitshift=0;
	int aridx=0;
	unsigned long long copydata;
	for(int i=0;i<34;i++)
	{
		//logic to determine which data from the struct we are copying this time arround
		if(i<32)
		{
		 copydata=data->data[i];
		}
		else if(i==32)
		{
			copydata==data->pps_count;
		}
		else 
		{
			copydata = data->time;
		}
		
		ncopy = ncopy+32;//determine the number of bits that have been copied
		aridx = ncopy / 57;//deterine what index of the copy array we are on
		bitshift = ncopy % 57;//determine the starting bit of the array we are on
		arr[arridx] |= ONES57 & (copydata<<bitshift); //copies the last 57 bits to the appropriot location
		
		if(bitshift>=(57-32))//check if any bits were not copied
		{
			bitshift=bitshift-(57-32);
			arr[arridx+1]|=copydata<<bitshift;
		}
	}
	
}

//alters filename to have the last 3 digits of fnumber
void Writer::GetFName(int fnumber,char * fname)
{
	int digi0=fnumber%10;
	int digi1=(fnumber/10)%10;
	int digi2=(fnumber/100)%10;
	
	fname[4] = Digi2Char(digi2);
	fname[5] = Digi2Char(digi1);
	fname[6] = Digi2Char(digi0);
	
	
}

//converts a digit to its correstponding charicter
char Writer::Digi2Char(int digit)
{
	char dchar = (char)(ASCIIZERO+(digit%10));
	return dchar;
}
//    write_place = 0;
//    read_place = 0;

//    while (1){
//        qDebug() << "Collecting data...";
//        while(ReadData()){
//            ClockRead();
//            SDSave();
//        }

//        if (write_place != read_place){
//            SDSave();
//        }
//        ClockRead();
//    }


//bool Writer::ReadData(){
//    fpga->DccaRead(&DccaStatus[write_place]);
//    write_place += 1;
//    if (DccaStatus[write_place-1] >= 32768){
//        DccaStatus[write_place-1] -= 32768;
//        return false;
//    }else{
//        return true;
//    }
//}

//void Writer::ClockRead(){
//    fpga->ReadSet(1);
//    fpga->ReadSet(0);
//    if (write_place >= 1000000){
//        write_place = 0;
//    }
//    if (read_place >= 1000000){
//        read_place = 0;
//    }
//}

//void Writer::SDSave(){
//    //myfile << (to_string(DccaStatus[read_place]) + "\n");
//    read_place += 1;
//}


//std::clock_t start;
//double duration;
//int i;
//start = clock();







//    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//    infofile << ("Read " + to_string(i) + " data samples from FPGA in: " + to_string(duration) + "\n");



//    start = clock();

//    fwrite(&DccPeakBuffer[0], 4, j, datafile_peak);
//    fwrite(&DccTailBuffer[0], 4, j, datafile_tail);
//    fwrite(&DccTimeBuffer[0], 4, j, datafile_time);

//    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//    infofile << ("Wrote " + to_string(j) + " data samples to SD Card in: " + to_string(duration) + "\n");








//    start = clock();

//    for(i=0;i<1000000;i++){
//        fwrite((uint16_t *)fpga->m_dcc_a_base, 2, 1, datafile);
//    }



//    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//    infofile << ("Wrote data in: " + to_string(duration) + "\n");


//void Writer::DCCPoll(){

//    std::clock_t start;
//    double duration;

//    int i;

//    start = clock();

//    for(i=0;i<1000000;i++){
//        fpga->SwitchRead(&DccaStatus[i]);
//    }

//    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//    myfile << ("Read data in: " + to_string(duration) + "\n");

//    start = clock();

//    for(i=0;i<1000000;i++){
//        myfile << (to_string(DccaStatus[i]) + "\n");
//    }

//    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//    myfile << ("Wrote data in: " + to_string(duration) + "\n");
//    myfile << "\n";

//}
