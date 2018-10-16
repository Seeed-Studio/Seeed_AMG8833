Grove_IR_Matrix_Temperature_sensor_AMG8833
==================  

Introduction of sensor
----------------------------  
The AGM8833 is a high Precision Infrared Array Sensor based on Advanced MEMS Technology.

***
Usage  
=======
Download all the source files.  
There are three examples for user.  
>* **exambles/basic_demo.ino**,From this example,you can get the 64 channels(8X8) IR sensor raw data from serial-watch-window.  
>* **exambles/TFT_screen_demo.ino**,From this example,we use a [TFT screen](https://www.seeedstudio.com/2.8-TFT-Touch-Shield-V2.0-p-1286.html) to show the 8X8 matrix data,Different data map different   colors.So we can make a simple、low resolution thermal imaging display system.Through this visual interface, you can see the results   very intuitively
>* **exambles/thermal_cam_interpolate/thermal_cam_interpolate.ino**,Use an interpolation algorithm to extend an 8X8 resolution gragh to a 32X32 resolution gragh.**But this example need more ram memory,doen't support the small ram arduino board like UNO.ATmega2560 or Arduino-M0 are OK.**  

In addition:  
---------------------
There is a INT pad on Seeed's AMG8833 module,This pad correspond to INT pin,when sensor generate a interruption(usually the temperature beyond limit),The pin turns to low,Otherwise stays high.Without this INT pin,you need to polling for sensor result value.  


Reference:
===============
Refer to the **/doc** to get more detail.


***
This software is written by downey  for seeed studio<br>
Email:dao.huang@seeed.cc
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>
