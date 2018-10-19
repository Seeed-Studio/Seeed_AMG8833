/*
 * Seeed_AMG8833_driver.h
 * Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR
 *  
 * Copyright (c) 2018 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : downey
 * Create Time: May 2018
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _SEEED_AMG8833_DRIER_H
#define _SEEED_AMG8833_DRIER_H


#include <Wire.h>
#include <Arduino.h>

#ifndef SEEED_DN_DEFINES
#define SEEED_DN_DEFINES

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL_DB SerialUSB
#else
  #define SERIAL_DB Serial
#endif


typedef int            s32;
typedef long unsigned int   u32;
typedef short          s16;
typedef unsigned short u16;
typedef char           s8;
typedef unsigned char  u8;

typedef enum	
{
    NO_ERROR=0,
    ERROR_PARAM=-1,
    ERROR_COMM =-2,
    ERROR_OTHERS=-128,
}err_t;


#define CHECK_RESULT(a,b)   do{if(a=b)  {    \
                            SERIAL_DB.print(__FILE__);    \
                            SERIAL_DB.print(__LINE__);   \
                            SERIAL_DB.print(" error code =");  \
                            SERIAL_DB.println(a);                   \
                            return a;   \
                            }}while(0)

#endif

#define PIXEL_NUM                           64


#define  POWER_CONTROL_REG_ADDR             0X00
#define  RESET_REG_ADDR                     0X01
#define  FRAME_RATE_ADDR                    0X02
#define  INTERRUPT_CONTROL_REG_ADDR         0X03
#define  STATUS_REG_ADDR                    0X04
#define  STATUS_CLEAR_REG_ADDR              0X05
#define  AVERAGE_REG_ADDR                   0X07
#define  INT_LEVEL_REG_ADDR_HL              0X08
#define  INT_LEVEL_REG_ADDR_HH              0X09
#define  INT_LEVEL_REG_ADDR_LL              0X0A
#define  INT_LEVEL_REG_ADDR_LH              0X0B
#define  INT_LEVEL_REG_ADDR_YSL             0X0C
#define  INT_LEVEL_REG_ADDR_YSH             0X0D
#define  THERMISTOR_REG_ADDR_L              0X0E
#define  THERMISTOR_REG_ADDR_H              0X0F
#define  INTERRUPT_TABLE_1_8_REG_ADDR       0X10
#define  INTERRUPT_TABLE_9_16_REG_ADDR      0X11
#define  INTERRUPT_TABLE_17_24_REG_ADDR     0X12
#define  INTERRUPT_TABLE_25_32_REG_ADDR     0X13
#define  INTERRUPT_TABLE_33_40_REG_ADDR     0X14
#define  INTERRUPT_TABLE_41_48_REG_ADDR     0X15
#define  INTERRUPT_TABLE_49_56_REG_ADDR     0X16
#define  INTERRUPT_TABLE_57_64_REG_ADDR     0X17

#define  TEMPERATURE_REG_ADDR_L             0X80
#define  TEMPERATURE_REG_ADDR_H             0X81


#define INT_ACTIVE                          0X01
#define INT_ABS_VALUE_INT_MODE              0X02

#define NORMAL_MODE                         0X00
#define SLEEP_MODE                          0X10
#define STAND_BY_MODE_60S_INTERMITTENCE     0X20
#define STAND_BY_MODE_10S_INTERMITTENCE     0X21


#define CLEAR_ALL_STATUS                    0X0E
#define CLEAR_INTERRUPT_STATUS              0X02

#define INIT_RESET_VALUE                    0X3F
#define FLAG_RESET_VALUE                    0X30


#define DEFAULT_IIC_ADDR  0x68

class AMG_IIC_OPRTS
{
    public:
        void IIC_begin(){Wire.begin();}
        s32 IIC_write_byte(u8 reg,u8 byte);
        void IIC_read_byte(u8 reg,u8* byte);
        void set_iic_addr(u8 IIC_ADDR);
        void IIC_read_16bit(u8 start_reg,u16 *value);
        s32 IIC_write_16bit(u8 reg,u16 value);
    private:
        u8 _IIC_ADDR;
};


class AMG8833:public AMG_IIC_OPRTS
{
    public:
        AMG8833(u8 addr = DEFAULT_IIC_ADDR);
        s32 init();
        s32 set_upper_limit(u8 value[]);
        s32 reset_flags(u8 value);
        s32 set_lower_limit(u8 value[]);
        s32 set_sensor_mode(u8 mode);
        s32 set_hysteresis(u8 value[]);
        s32 clear_status(u8 value);
        s32 set_interrupt_mode(u8 mode);
        s32 set_frame_rate(u8 rate);
        s32 read_pixel_temperature(float *pixel_data);
        s32 get_interrupt_status();
        s32 read_pixels_interrupt_status(u8 *status);
        s32 read_pixel_temperature_reg_value(u16* value);
};


#endif

