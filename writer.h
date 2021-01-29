#ifndef WRITER_H
#define WRITER_H

#include "fpga.h"
#include "hps.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>

using namespace std;

#define BUFFER_SIZE                 16  // write buffer 5   times per file LYSO save rate
//#define BUFFER_SIZE               4096    // write buffer 160 times per file CLYC save rate
#define MAX_WRITE_COUNT_PER_FILE  655360  // 655360 is about 100MB per file


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
    buff_t buff[BUFFER_SIZE];
    time_t now;
    uint32_t data_0_old;
    uint32_t data_1_old;
    uint32_t data_2_old;
    uint32_t data_3_old;
    uint32_t data_4_old;
    uint32_t data_5_old;
    uint32_t pulse_num_old;
    int32_t writeCount;
    int32_t maxWriteCount = MAX_WRITE_COUNT_PER_FILE;
    int32_t fileCount;

};


#endif // WRITER_H
