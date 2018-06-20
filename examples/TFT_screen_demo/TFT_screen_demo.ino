/*
 * TFT_screen_demo.ino
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

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

#include "Seeed_AMG8833_driver.h"





AMG8833 sensor;

#define TFT_PIXELS_NUM   30

void parse_int_status(u8* status)
{
    u8 val=0;
    for(u32 i=0;i<8;i++)
    {
        if(status[i])
        {
            for(u32 j=0;j<8;j++)
            {
                if(status[i]&((1<<j)))
                {
                    Serial.print("pixel ");
                    Serial.print(8*i+j+1);
                    Serial.println("interrupt is generated!!!");
                }
            }
        }
    }
}

void print_status(u8* status)
{
    for(u32 i=0;i<8;i++)
    {
        Serial.print(status[i],HEX);
        Serial.print("  ");

    }
    Serial.println("  ");
}




void setup()
{
    Serial.begin(115200);
    sensor.init();
    TFT_BL_ON;
    /*2.8 TFT screen. url:https://www.seeedstudio.com/2.8-TFT-Touch-Shield-V2.0-p-1286.html*/
    Tft.TFTinit();
}



void loop()
{
    u8 val=0;
    float pixels_temp[PIXEL_NUM]={0};
    u16 color[PIXEL_NUM]={0};
    /*Read temperature*/
    sensor.read_pixel_temperature(pixels_temp);
    /*Different temperature correspond to different color.*/
    for(u32 i=0;i<PIXEL_NUM;i++)
    {
        if(pixels_temp[i]<29)
        {
            color[i]=BLUE;
        }
        else if((pixels_temp[i]>=29)&&(pixels_temp[i]<30))
        {
            color[i]=GREEN;
        }
        else if((pixels_temp[i]>=30)&&(pixels_temp[i]<31))
        {
            color[i]=YELLOW;
        }
        else if((pixels_temp[i]>=31)&&(pixels_temp[i]<33))
        {
            color[i]=0xfd00;
        }
        else
        {
            color[i]=RED;
        }
    }
    /*Use a TFT screen to display.*/
    for(u32 i=0;i<PIXEL_NUM;i++)
    {
        Tft.fillScreen(TFT_PIXELS_NUM*(i%8),TFT_PIXELS_NUM*(i%8+1),TFT_PIXELS_NUM*(8-i/8),TFT_PIXELS_NUM*(7-i/8),color[i]);
    }
}
