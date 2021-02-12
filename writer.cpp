#include "writer.h"
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <QDebug>
#include "circularq.h"

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

    //Creating initial file
    MakeNewFile(0);

    //Begin data collection
    DCCPoll();

}












void Writer::DCCPoll(){
	
	circularq buffer;
	buff_t * cdata;
	buffer.EnQEmpty(&cdata);
	
    fpga->ReadSet(1);
    writeCount = 0;
    fileCount = 0;
	bool isnew;
    while(1){
        // Reading data from FPGA
        fpga->ReadSet(0);
        fpga->DataRead(cdata);
        fpga->ReadSet(1);
		
		isnew = cdata->pulse_num != pulse_num_old; 
		
        if(isnew){
			
            //Setting old pulse_num value
            pulse_num_old = cdata->pulse_num;
			buffer.EnQEmpty(&cdata);
        }
		if(!isnew || buffer.IsFull())
		{
			GetFront(&cdata);
			fwrite(cdata, 4,38, datafile); //writes the data
			
			//Checking if a new file needs to be created
            if(writeCount >= maxWriteCount){
                fileCount = fileCount + 1;
                MakeNewFile(fileCount);
                writeCount = 0;
            }
			writeCount = writeCount + 1;
			buffer.GetRear(&cdata);
		}
		
    }
}



void Writer::MakeNewFile(int fileNum){

    char filename[15] = "data_";
    char fileNumStr[10];
    uint32_t StartUp = 0xffffffff;

    sprintf(fileNumStr,"%d",fileNum);
    strcat(filename, fileNumStr);

    datafile = fopen(filename, "a+b");
    //qDebug() << "Starting Data Collection...";
    fwrite(&StartUp, 4, 1, datafile);
    fclose(datafile);

    datafile = fopen(filename, "a+b");

}






















//void Writer::DCCPoll(){
//    fpga->ReadSet(1);
//    writeCount = 0;
//    fileCount = 0;
//    while(1){
//        // Reading data from FPGA
//        fpga->ReadSet(0);
//        fpga->DataRead(&buff[0]);
//        fpga->ReadSet(1);

//        //Checking if data is the same as the previous read. If not, write new data to file
//        //if(buff[0].data_0 != data_0_old && buff[0].data_1 != data_1_old && buff[0].data_2 != data_2_old && buff[0].data_3 != data_3_old && buff[0].data_4 != data_4_old && buff[0].data_5 != data_5_old){
//        if(buff[0].pulse_num != pulse_num_old){
//            //qDebug() << "Writing Data!!!!";

//            fwrite(&buff[0], 4, 38, datafile); // fwrite writes the values in byte wise little endian

//            //Setting old data values
//            //data_0_old = buff[0].data_0;
//            //data_1_old = buff[0].data_1;
//            //data_2_old = buff[0].data_2;
//            //data_3_old = buff[0].data_3;
//            //data_4_old = buff[0].data_4;
//            //data_5_old = buff[0].data_5;
//            pulse_num_old = buff[0].pulse_num;

//            writeCount = writeCount + 1;

//            //Checking if a new file needs to be created
//            if(writeCount >= maxWriteCount){
//                fileCount = fileCount + 1;
//                MakeNewFile(fileCount);
//                writeCount = 0;
//            }
//        }
//    }
//}

























//void Writer::DCCPoll(){
//    qDebug() << "Starting polling!";
//    fpga->ReadSet(1);

//    while(1){
//        // Reading Data
//        fpga->ReadSet(0);
//        fpga->DataRead(&buff[0]);
//        fpga->ReadSet(1);
//        if(!(buff[0].data_0==-1)){
//            //fwrite(&buff[0], 4, 34, datafile);
//            //fwrite(&buff[0].data_4, 4, 1, datafile);
//            qDebug() << "Writing Data!";
//            fprintf(datafile, "%d,", buff[0].pps_count);
//            fprintf(datafile, "%d,", buff[0].pps_time);
//            fprintf(datafile, "%d,", buff[0].time);
//            fprintf(datafile, "%d,", buff[0].data_0);
//            fprintf(datafile, "%d,", buff[0].data_1);
//            fprintf(datafile, "%d,", buff[0].data_2);
//            fprintf(datafile, "%d,", buff[0].data_3);
//            fprintf(datafile, "%d,", buff[0].data_4);
//            fprintf(datafile, "%d,", buff[0].data_5);
//            fprintf(datafile, "%d,", buff[0].data_6);
//            fprintf(datafile, "%d,", buff[0].data_7);
//            fprintf(datafile, "%d,", buff[0].data_8);
//            fprintf(datafile, "%d,", buff[0].data_9);
//            fprintf(datafile, "%d,", buff[0].data_10);
//            fprintf(datafile, "%d,", buff[0].data_11);
//            fprintf(datafile, "%d,", buff[0].data_12);
//            fprintf(datafile, "%d,", buff[0].data_13);
//            fprintf(datafile, "%d,", buff[0].data_14);
//            fprintf(datafile, "%d,", buff[0].data_15);
//            fprintf(datafile, "%d,", buff[0].data_16);
//            fprintf(datafile, "%d,", buff[0].data_17);
//            fprintf(datafile, "%d,", buff[0].data_18);
//            fprintf(datafile, "%d,", buff[0].data_19);
//            fprintf(datafile, "%d,", buff[0].data_20);
//            fprintf(datafile, "%d,", buff[0].data_21);
//            fprintf(datafile, "%d,", buff[0].data_22);
//            fprintf(datafile, "%d,", buff[0].data_23);
//            fprintf(datafile, "%d,", buff[0].data_24);
//            fprintf(datafile, "%d,", buff[0].data_25);
//            fprintf(datafile, "%d,", buff[0].data_26);
//            fprintf(datafile, "%d,", buff[0].data_27);
//            fprintf(datafile, "%d,", buff[0].data_28);
//            fprintf(datafile, "%d,", buff[0].data_29);
//            fprintf(datafile, "%d,", buff[0].data_30);
//            fprintf(datafile, "%d\n", buff[0].data_31);
//        }
//    }
//}









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
