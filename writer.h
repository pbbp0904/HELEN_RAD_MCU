#ifndef WRITER_H
#define WRITER_H

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

    void MakeNewFile(int fileNum);

private:
    HPS *hps;
    FPGA *fpga;
    FILE* datafile;
    uint16_t Status;
    buff_t buff[2];
    time_t now;
    uint32_t data_0_old;
    uint32_t data_1_old;
    uint32_t data_2_old;
    uint32_t data_3_old;
    uint32_t data_4_old;
    uint32_t data_5_old;
    int32_t writeCount;
    int32_t maxWriteCount = 700000; //700000 writes is about 100MB per file
    int32_t fileCount;

};


#endif // WRITER_H
