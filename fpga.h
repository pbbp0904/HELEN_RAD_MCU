#ifndef FPGA_H
#define FPGA_H

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>


struct buff_t
{
    uint32_t pps_count;
    uint32_t pps_time;
    uint32_t time;
    uint32_t pulse_num;
    uint32_t buff_diff;
    uint32_t data_0;
    uint32_t data_1;
    uint32_t data_2;
    uint32_t data_3;
    uint32_t data_4;
    uint32_t data_5;
    uint32_t data_6;
    uint32_t data_7;
    uint32_t data_8;
    uint32_t data_9;
    uint32_t data_10;
    uint32_t data_11;
    uint32_t data_12;
    uint32_t data_13;
    uint32_t data_14;
    uint32_t data_15;
    uint32_t data_16;
    uint32_t data_17;
    uint32_t data_18;
    uint32_t data_19;
    uint32_t data_20;
    uint32_t data_21;
    uint32_t data_22;
    uint32_t data_23;
    uint32_t data_24;
    uint32_t data_25;
    uint32_t data_26;
    uint32_t data_27;
    uint32_t data_28;
    uint32_t data_29;
    uint32_t data_30;
    uint32_t data_31;
    uint32_t delim;


//    bool operator==(const buff_t& b) {
//        bool result = true;
//        result &= this->pps_count == b.pps_count;
//        result &= this->time == b.time;
//        for(int i = 0; i < 32; i++) {
//            result &= this->data[i] == b.data[i];
//        }
//        return result;
//    }
    
//    bool operator=(const buff_t& b) {
//        this->pps_count = b.pps_count;
//        this->time = b.time;
//        for(int i = 0; i < 32; i++) {
//            this->data[i] = b.data[i];
//        }
//        return true;
//    }
};



class FPGA
{
public:
    FPGA();
    ~FPGA();

    bool DataRead(buff_t *buffer);
    bool LedSet(int mask);
    bool SwitchRead(uint16_t *mask);
    bool DccPeakRead(uint32_t *mask);
    bool DccTailRead(uint32_t *mask);
    bool DccTimeRead(uint32_t *mask);
    bool ReadSet(int mask);


protected:
    bool m_bInitSuccess;
    int m_file_mem;
    bool m_bIsVideoEnabled;

    void *virtual_base;

    uint8_t *m_led_base;
    uint8_t *m_sw_base;

    uint8_t *m_read_base;

    uint8_t *m_pps_count_base;
    uint8_t *m_pps_time_out_base;
    uint8_t *m_dcc_time_base;
    uint8_t *m_pulse_num_out_base;
    uint8_t *m_buff_diff_out_base;

    uint8_t *m_dcc_data_0_base;
    uint8_t *m_dcc_data_1_base;
    uint8_t *m_dcc_data_2_base;
    uint8_t *m_dcc_data_3_base;
    uint8_t *m_dcc_data_4_base;
    uint8_t *m_dcc_data_5_base;
    uint8_t *m_dcc_data_6_base;
    uint8_t *m_dcc_data_7_base;
    uint8_t *m_dcc_data_8_base;
    uint8_t *m_dcc_data_9_base;
    uint8_t *m_dcc_data_10_base;
    uint8_t *m_dcc_data_11_base;
    uint8_t *m_dcc_data_12_base;
    uint8_t *m_dcc_data_13_base;
    uint8_t *m_dcc_data_14_base;
    uint8_t *m_dcc_data_15_base;
    uint8_t *m_dcc_data_16_base;
    uint8_t *m_dcc_data_17_base;
    uint8_t *m_dcc_data_18_base;
    uint8_t *m_dcc_data_19_base;
    uint8_t *m_dcc_data_20_base;
    uint8_t *m_dcc_data_21_base;
    uint8_t *m_dcc_data_22_base;
    uint8_t *m_dcc_data_23_base;
    uint8_t *m_dcc_data_24_base;
    uint8_t *m_dcc_data_25_base;
    uint8_t *m_dcc_data_26_base;
    uint8_t *m_dcc_data_27_base;
    uint8_t *m_dcc_data_28_base;
    uint8_t *m_dcc_data_29_base;
    uint8_t *m_dcc_data_30_base;
    uint8_t *m_dcc_data_31_base;



    bool Init();

};

#endif // FPGA_H
