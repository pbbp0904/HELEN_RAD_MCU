#ifndef WRITER_H
#define WRITER_H
#include "circularq.h"
#include "fpga.h"
#include "hps.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>

using namespace std;


class Writer
{
public:
    Writer();
    ~Writer();

    void DCCPolling();

    void DCCPoll();

    void DCCWrite();

    bool ReadData();

    void ClockRead();

    void SDSave();
	
	void WriteSD(fpga_data * data);
	
	bool IsNewData(fpga_data * data);
	
	void GetFName(int fnumber,char * fname);
	
	

private:
    HPS *hps;
    FPGA *fpga;
    FILE* datafile;
    uint16_t Status;
    fpga_data * cdata;
	CircularQ que;
    time_t now;
    int32_t data_0_old;
    int32_t data_1_old;
    int32_t data_2_old;
    int32_t data_3_old;
    int32_t data_4_old;
    int32_t data_5_old;

};


#endif // WRITER_H
