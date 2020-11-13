#include "fpga.h"
#include <QDebug>

#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#include <sys/mman.h>
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

// QSyS dependent address
#define FPGA_LED_PIO_BASE 0x3000
#define FPGA_SW_PIO_BASE 0x4000
#define FPGA_DCC_DATA_0_BASE 0x5000
#define FPGA_DCC_DATA_1_BASE 0x6000
#define FPGA_DCC_DATA_2_BASE 0x01e0 
#define FPGA_DCC_DATA_3_BASE 0x01d0
#define FPGA_DCC_DATA_4_BASE 0x01c0
#define FPGA_DCC_DATA_5_BASE 0x01b0
#define FPGA_DCC_DATA_6_BASE 0x01a0
#define FPGA_DCC_DATA_7_BASE 0x0190
#define FPGA_DCC_DATA_8_BASE 0x0180
#define FPGA_DCC_DATA_9_BASE 0x0170
#define FPGA_DCC_DATA_10_BASE 0x0160
#define FPGA_DCC_DATA_11_BASE 0x0150
#define FPGA_DCC_DATA_12_BASE 0x0140
#define FPGA_DCC_DATA_13_BASE 0x0130
#define FPGA_DCC_DATA_14_BASE 0x0120
#define FPGA_DCC_DATA_15_BASE 0x0110
#define FPGA_DCC_DATA_16_BASE 0x0100
#define FPGA_DCC_DATA_17_BASE 0x00f0
#define FPGA_DCC_DATA_18_BASE 0x00e0
#define FPGA_DCC_DATA_19_BASE 0x00d0
#define FPGA_DCC_DATA_20_BASE 0x00c0
#define FPGA_DCC_DATA_21_BASE 0x00b0
#define FPGA_DCC_DATA_22_BASE 0x00a0
#define FPGA_DCC_DATA_23_BASE 0x0090
#define FPGA_DCC_DATA_24_BASE 0x0080
#define FPGA_DCC_DATA_25_BASE 0x0070
#define FPGA_DCC_DATA_26_BASE 0x0060
#define FPGA_DCC_DATA_27_BASE 0x0050
#define FPGA_DCC_DATA_28_BASE 0x0040
#define FPGA_DCC_DATA_29_BASE 0x0030
#define FPGA_DCC_DATA_30_BASE 0x0020
#define FPGA_DCC_DATA_31_BASE 0x0010
#define FPGA_PPS_COUNT_OUT_BASE 0x01f0
#define FPGA_DCC_TIME_OUT_BASE 0x7000
#define FPGA_READ_PIO_BASE 0x8000
#define FPGA_PPS_TIME_OUT_BASE 0x9000

// ///////////////////////////////////////
// memory map

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0x04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

FPGA::FPGA() : m_bIsVideoEnabled(false)
{
    m_file_mem = open("/dev/mem", (O_RDWR | O_SYNC));
    virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, m_file_mem, HW_REGS_BASE);
    m_bInitSuccess = Init();
    if (!m_bInitSuccess)
        qDebug() << "FPGA init failed!!!\r\n";
}

FPGA::~FPGA()
{
    close(m_file_mem);
}

bool FPGA::Init()
{
    bool bSuccess = false;

    if (m_file_mem != -1)
    {
        virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, m_file_mem, HW_REGS_BASE);
        if (virtual_base == MAP_FAILED)
        {
        }
        else
        {
            m_led_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + FPGA_LED_PIO_BASE) & (unsigned long)(HW_REGS_MASK));
            m_sw_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + FPGA_SW_PIO_BASE) & (unsigned long)(HW_REGS_MASK));

            m_read_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + FPGA_READ_PIO_BASE) & (unsigned long)(HW_REGS_MASK));

            m_dcc_data_0_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_0_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_1_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_1_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_2_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_2_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_3_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_3_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_4_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_4_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_5_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_5_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_6_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_6_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_7_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_7_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_8_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_8_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_9_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_9_BASE) &  (unsigned long)(HW_REGS_MASK));
            m_dcc_data_10_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_10_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_11_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_11_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_12_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_12_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_13_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_13_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_14_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_14_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_15_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_15_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_16_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_16_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_17_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_17_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_18_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_18_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_19_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_19_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_20_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_20_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_21_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_21_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_22_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_22_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_23_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_23_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_24_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_24_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_25_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_25_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_26_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_26_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_27_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_27_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_28_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_28_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_29_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_29_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_30_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_30_BASE) & (unsigned long)(HW_REGS_MASK));
            m_dcc_data_31_base = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_DATA_31_BASE) & (unsigned long)(HW_REGS_MASK));

            m_dcc_time_base    = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_DCC_TIME_OUT_BASE) & (unsigned long)(HW_REGS_MASK));
            m_pps_count_base   = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_PPS_COUNT_OUT_BASE) & (unsigned long)(HW_REGS_MASK));
            m_pps_time_out_base  = (uint8_t *)virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST +  FPGA_PPS_TIME_OUT_BASE) & (unsigned long)(HW_REGS_MASK));
            bSuccess = true;
        }
    }

    return bSuccess;
}
/*
bool FPGA::DataRead(buff_t *buffer)
{
    buffer->pps_count = *(uint32_t *)m_pps_count_base;
    buffer->time = *(uint32_t *)m_dcc_time_base;


    buffer->data_0 = *(uint32_t *)m_dcc_data_0_base;
    buffer->data_1 = *(uint32_t *)m_dcc_data_1_base;
    buffer->data_2 = *(uint32_t *)m_dcc_data_2_base;
    buffer->data_3 = *(uint32_t *)m_dcc_data_3_base;
    buffer->data_4 = *(uint32_t *)m_dcc_data_4_base;
    buffer->data_5 = *(uint32_t *)m_dcc_data_5_base;
    buffer->data_6 = *(uint32_t *)m_dcc_data_6_base;
    buffer->data_7 = *(uint32_t *)m_dcc_data_7_base;
    buffer->data_8 = *(uint32_t *)m_dcc_data_8_base;
    buffer->data_9 = *(uint32_t *)m_dcc_data_9_base;
    buffer->data_10 = *(uint32_t *)m_dcc_data_10_base;
    buffer->data_11 = *(uint32_t *)m_dcc_data_11_base;
    buffer->data_12 = *(uint32_t *)m_dcc_data_12_base;
    buffer->data_13 = *(uint32_t *)m_dcc_data_13_base;
    buffer->data_14 = *(uint32_t *)m_dcc_data_14_base;
    buffer->data_15 = *(uint32_t *)m_dcc_data_15_base;
    buffer->data_16 = *(uint32_t *)m_dcc_data_16_base;
    buffer->data_17 = *(uint32_t *)m_dcc_data_17_base;
    buffer->data_18 = *(uint32_t *)m_dcc_data_18_base;
    buffer->data_19 = *(uint32_t *)m_dcc_data_19_base;
    buffer->data_20 = *(uint32_t *)m_dcc_data_20_base;
    buffer->data_21 = *(uint32_t *)m_dcc_data_21_base;
    buffer->data_22 = *(uint32_t *)m_dcc_data_22_base;
    buffer->data_23 = *(uint32_t *)m_dcc_data_23_base;
    buffer->data_24 = *(uint32_t *)m_dcc_data_24_base;
    buffer->data_25 = *(uint32_t *)m_dcc_data_25_base;
    buffer->data_26 = *(uint32_t *)m_dcc_data_26_base;
    buffer->data_27 = *(uint32_t *)m_dcc_data_27_base;
    buffer->data_28 = *(uint32_t *)m_dcc_data_28_base;
    buffer->data_29 = *(uint32_t *)m_dcc_data_29_base;
    buffer->data_30 = *(uint32_t *)m_dcc_data_30_base;
    buffer->data_31 = *(uint32_t *)m_dcc_data_31_base;



    return true;
}
*/

bool FPGA::DataRead(fpga_data * buffer)
{
	buffer->pps_count = *(uint32_t *)m_pps_count_base;
    buffer->time = *(uint32_t *)m_dcc_time_base;
    buffer->pps_time = *(uint32_t *)m_pps_time_out_base;


    buffer->data[0] = *(uint32_t *)m_dcc_data_0_base;
    buffer->data[1] = *(uint32_t *)m_dcc_data_1_base;
    buffer->data[2] = *(uint32_t *)m_dcc_data_2_base;
    buffer->data[3] = *(uint32_t *)m_dcc_data_3_base;
    buffer->data[4] = *(uint32_t *)m_dcc_data_4_base;
    buffer->data[5] = *(uint32_t *)m_dcc_data_5_base;
    buffer->data[6] = *(uint32_t *)m_dcc_data_6_base;
    buffer->data[7] = *(uint32_t *)m_dcc_data_7_base;
    buffer->data[8] = *(uint32_t *)m_dcc_data_8_base;
    buffer->data[9] = *(uint32_t *)m_dcc_data_9_base;
    buffer->data[10] = *(uint32_t *)m_dcc_data_10_base;
    buffer->data[11] = *(uint32_t *)m_dcc_data_11_base;
    buffer->data[12] = *(uint32_t *)m_dcc_data_12_base;
    buffer->data[13] = *(uint32_t *)m_dcc_data_13_base;
    buffer->data[14] = *(uint32_t *)m_dcc_data_14_base;
    buffer->data[15] = *(uint32_t *)m_dcc_data_15_base;
    buffer->data[16] = *(uint32_t *)m_dcc_data_16_base;
    buffer->data[17] = *(uint32_t *)m_dcc_data_17_base;
    buffer->data[18] = *(uint32_t *)m_dcc_data_18_base;
    buffer->data[19] = *(uint32_t *)m_dcc_data_19_base;
    buffer->data[20] = *(uint32_t *)m_dcc_data_20_base;
    buffer->data[21] = *(uint32_t *)m_dcc_data_21_base;
    buffer->data[22] = *(uint32_t *)m_dcc_data_22_base;
    buffer->data[23] = *(uint32_t *)m_dcc_data_23_base;
    buffer->data[24] = *(uint32_t *)m_dcc_data_24_base;
    buffer->data[25] = *(uint32_t *)m_dcc_data_25_base;
    buffer->data[26] = *(uint32_t *)m_dcc_data_26_base;
    buffer->data[27] = *(uint32_t *)m_dcc_data_27_base;
    buffer->data[28] = *(uint32_t *)m_dcc_data_28_base;
    buffer->data[29] = *(uint32_t *)m_dcc_data_29_base;
    buffer->data[30] = *(uint32_t *)m_dcc_data_30_base;
    buffer->data[31] = *(uint32_t *)m_dcc_data_31_base;



    return true;
}

//bool FPGA::LedSet(int mask){
//    if (!m_bInitSuccess)
//        return false;

//    //qDebug() << "FPGA:LedSet\r\n";
//    //*(uint32_t *)m_led_base = mask;
//    return true;
//}

//bool FPGA::SwitchRead(uint16_t *mask)
//{
//    if (!m_bInitSuccess)
//        return false;
//
//    *mask = *(uint16_t *)m_sw_base;
//    return true;
//}

bool FPGA::ReadSet(int mask)
{
    //    if (!m_bInitSuccess)
    //        return false;

    *(uint32_t *)m_read_base = mask;
    return true;
}

//bool FPGA::DccPeakRead(uint32_t *mask)
//{
//    //    if (!m_bInitSuccess)
//    //        return false;
//
//    *mask = *(uint32_t *)m_dcc_peak_base;
//    return true;
//}

//bool FPGA::DccTailRead(uint32_t *mask)
//{
//    //    if (!m_bInitSuccess)
//    //        return false;
//
//    *mask = *(uint32_t *)m_dcc_tail_base;
//    return true;
//}

//bool FPGA::DccTimeRead(uint32_t *mask)
//{
//    //    if (!m_bInitSuccess)
//    //        return false;
//
//    *mask = *(uint32_t *)m_dcc_time_base;
//    return true;
//}
