/*
    Seeed_AMG8833_driver.cpp
    Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR

    Copyright (c) 2018 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Author     : downey
    Create Time: May 2018
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include "Seeed_AMG8833_driver.h"



/** @brief structure function,set IIC addr
    @param IIC addr
 * */
AMG8833::AMG8833(u8 addr) {
    _isConnect = false;
    set_iic_addr(addr);
}


/** @brief set frame rate,default is 10FPS
    @param rate
    @return 0 if success
 * */
s32 AMG8833::set_frame_rate(u8 rate) {
    return IIC_write_byte(FRAME_RATE_ADDR, rate);
}


/** @brief set sensor mode.
    @param sensor mode
    @return 0 if success
 * */
s32 AMG8833::set_sensor_mode(u8 mode) {
    return IIC_write_byte(POWER_CONTROL_REG_ADDR, mode);
}


/** @brief set interrupt mode.
    @param interrupt mode
    @return 0 if success
 * */
s32 AMG8833::set_interrupt_mode(u8 mode) {
    return IIC_write_byte(INTERRUPT_CONTROL_REG_ADDR, mode);
}

/** @brief get interrupt status.
    @param interrupt status.
    @return 0 if no interrupt.1 indicate interrupt was generated.
 * */
s32 AMG8833::get_interrupt_status() {
    u8 value;
    IIC_read_byte(STATUS_REG_ADDR, &value);
    if (value & 0x02) {
        return 1;
    }
    return 0;
}


/** @brief clear status.overflow status or interrupt status.
    @param value
    @return 0 if success
 * */
s32 AMG8833::clear_status(u8 value) {
    return IIC_write_byte(STATUS_CLEAR_REG_ADDR, value);
}

/** @brief read interrupt status for 64 channel pixels.
    @param u8 [8],dst value.
    @return 0 if success
 * */
s32 AMG8833::read_pixels_interrupt_status(u8* status) {
    for (u32 i = 0; i < 8; i++) {
        IIC_read_byte(INTERRUPT_TABLE_1_8_REG_ADDR + i, &status[i]);
    }
    return 0;
}

/** @brief set upper limit.
    @param u8[2] value.
    @return 0 if success
 * */
s32 AMG8833::set_upper_limit(u8 value[]) {
    s32 ret;
    ret = IIC_write_byte(INT_LEVEL_REG_ADDR_HL, value[0]);
    ret = IIC_write_byte(INT_LEVEL_REG_ADDR_HH, value[1]);
    return ret;
}



/** @brief set lower limit.
    @param u8[2] value.
    @return 0 if success
 * */
s32 AMG8833::set_lower_limit(u8 value[]) {
    s32 ret;
    ret = IIC_write_byte(INT_LEVEL_REG_ADDR_LL, value[0]);
    ret = IIC_write_byte(INT_LEVEL_REG_ADDR_LH, value[1]);
    return ret;
}


/** @brief set hysteresis for interrupt.
    @param u8[2] value.
    @return 0 if success
 * */
s32 AMG8833::set_hysteresis(u8 value[]) {
    s32 ret;
    ret = IIC_write_byte(INT_LEVEL_REG_ADDR_YSL, value[0]);
    ret = IIC_write_byte(INT_LEVEL_REG_ADDR_YSH, value[1]);
    return ret;
}


/** @brief reset sensor flag,0x30-reset interrupt flag,0x3f-reinit the sensor.
    @param reset option value.
    @return 0 if success
 * */
s32 AMG8833::reset_flags(u8 value) {
    return IIC_write_byte(RESET_REG_ADDR, value);
}

/** @brief read pixels for 64 channel register data,u16-form
    @param u16[64],dst value
    @return 0 if success
 * */
s32 AMG8833::read_pixel_temperature_reg_value(u16* value) {
    u8 val_l = 0, val_h = 0;
    for (int i = 0; i < PIXEL_NUM; i++) {
        IIC_read_byte(TEMPERATURE_REG_ADDR_L + 2 * i, &val_l);
        IIC_read_byte(TEMPERATURE_REG_ADDR_H + 2 * i, &val_h);
        value[i] = (u16)val_h << 8 | val_l;
    }
    return 0;
}

/** @brief read pixels for 64 channel register data,convert to float data
    @param float[64],dst value
    @return 0 if success
 * */
s32 AMG8833::read_pixel_temperature(float* pixel_data) {
    u8 val_l = 0, val_h = 0;
    u16 value = 0;
    for (int i = 0; i < PIXEL_NUM; i++) {
        IIC_read_byte(TEMPERATURE_REG_ADDR_L + 2 * i, &val_l);
        IIC_read_byte(TEMPERATURE_REG_ADDR_H + 2 * i, &val_h);
        value = (u16)val_h << 8 | val_l;
        pixel_data[i] = (float)(value >> 2) + (float)(value & 0x03) * 0.25;
    }
    return 0;
}



/** @brief config sensor.
    @return 0 if success


 * */
s32 AMG8833::init() {
    s32 ret = 0;
    IIC_begin();

    /*32°C，calculate method：u16 value,last bit0~bit1*0.25,bit2~bit10 /4,bit11 is sign bit,1 is negative.*/
    u8 cfg_data_high_limit[] = {0x80, 0x00};
    /*0.25°C*/
    u8 cfg_data_low_limit[] = {0x01, 0x00};
    /*2°C,When temperature lower than (high_limit-hysteresis)(30-2=28℃),INT pin back to origin status. */
    u8 cfg_data_hysteresis[] = {0x08, 0x00};

    /*Normal mode!!,if return none-zero,IIC communication error,return error code.*/
    ret = set_sensor_mode(NORMAL_MODE);
    if (ret != 0) {
        _isConnect = false;
        return ret;
    }
    delay(30);
    set_upper_limit(cfg_data_high_limit);
    set_lower_limit(cfg_data_low_limit);
    set_hysteresis(cfg_data_hysteresis);
    /*Enable interrupt function.*/
    set_interrupt_mode(INT_ACTIVE | INT_ABS_VALUE_INT_MODE);
    //set_interrupt_mode(INT_ACTIVE);
    delay(10);
    /*Clear all status register*/
    clear_status(CLEAR_ALL_STATUS);
    /*Reset all flag*/
    reset_flags(INIT_RESET_VALUE);
    delay(300);
    _isConnect = true;
    return 0;
}

AMG8833::operator bool() { return _isConnect; }

/**********************************************************************************************************/
/************************************************IIC PART************************************************/
/**********************************************************************************************************/

s32 AMG_IIC_OPRTS::IIC_write_byte(u8 reg, u8 byte) {
    Wire.beginTransmission(_IIC_ADDR);
    Wire.write(reg);
    Wire.write(byte);
    return Wire.endTransmission();
}


s32 AMG_IIC_OPRTS::IIC_write_16bit(u8 reg, u16 value) {
    Wire.beginTransmission(_IIC_ADDR);
    Wire.write(reg);

    Wire.write((u8)(value >> 8));
    Wire.write((u8)value);
    return Wire.endTransmission();
}




void AMG_IIC_OPRTS::IIC_read_byte(u8 reg, u8* byte) {
    Wire.beginTransmission(_IIC_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_IIC_ADDR, (u8)1);
    while (1 != Wire.available());
    *byte = Wire.read();

}

void AMG_IIC_OPRTS::IIC_read_16bit(u8 start_reg, u16* value) {
    u8 val = 0;
    *value = 0;
    Wire.beginTransmission(_IIC_ADDR);
    Wire.write(start_reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_IIC_ADDR, sizeof(u16));
    while (sizeof(u16) != Wire.available());
    val = Wire.read();
    *value |= (u16)val << 8;
    val = Wire.read();
    *value |= val;
}



void AMG_IIC_OPRTS::set_iic_addr(u8 IIC_ADDR) {
    _IIC_ADDR = IIC_ADDR;
}
