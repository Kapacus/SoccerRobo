#include <OLLO.h> 
#include <RC100.h>


/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP

/* Dynamixel ID defines */
#define ID_Left_Wheel 1
#define ID_Right_Wheel 2
#define ID_Kick 4

/* Control table defines */
#define CCW_Angle_Limit 8 //to change control mode
#define CW_Angle_Limit 6

#define Goal_Position 30
#define Moving_Speed 32

//#define CONTROL_MODE 11

RC100 Controller;

int RxData;
int l_command = 0;
int r_command = 0;
int flag=0;
int delta_v = 10;

Dynamixel Dxl(DXL_BUS_SERIAL3);

void setup() {
  // Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  Controller.begin(1);
  
  //AX MX RX Series  
 Dxl.writeByte(ID_Left_Wheel, CCW_Angle_Limit, 0);  //Dxl.jointMode(ID) can be used
 Dxl.writeByte(ID_Right_Wheel, CCW_Angle_Limit, 0);
 Dxl.jointMode(ID_Kick);
 

 }

/*The button value declared in advance can be used to receive the button values from RC-100.
Value	Name	Contents
1	RC100_BTN_U	Pressed value- U Button
2	RC100_BTN_D	Pressed value- D Button
4	RC100_BTN_L	Pressed value- L Button
8	RC100_BTN_R	Pressed value- R Button
16	RC100_BTN_1	Pressed value- Button 1
32	RC100_BTN_2	Pressed value- Button 2
64	RC100_BTN_3	Pressed value- Button 3
128	RC100_BTN_4	Pressed value- Button 4
256	RC100_BTN_5	Pressed value- Button 5
512	RC100_BTN_6	Pressed value- Button 6*/

/*
  ASCII                   Key
  0x08	                  Backspace
  0x0d, 0x0a	          Enter
  0x1b	                  ESC
  0x1b, 0x5b, 0x41	  Cursor up
  0x1b, 0x5b, 0x42	  Cursor down
  0x1b, 0x5b, 0x43	  Cursor right
  0x1b, 0x5b, 0x44	  Cursor left
  0x1b, 0x5b, 0x48	  Home
  0x1b, 0x5b, 0x4b	  End
  
  http://support.robotis.com/en/software/roboplus/roboplus_terminal/quickstart/roboplus_terminal_keyboard_input.htm*/


void loop() {

  /*if(zgb_rx_check() == 1){// Succeed to data recieved
    RxData = zgb_rx_data(); // Get data verified
  }
  else if(zgb_rx_check() == 0 ){// Failed to data recieved
  
  }*/
    

  SerialUSB.print("r_command: ");
  SerialUSB.println(r_command);
  
  //  l_command = 200;
  
  
  SerialUSB.print("l_command: ");
  SerialUSB.println(l_command);
  
  if(Controller.available()){
      RxData = Controller.readData();
      SerialUSB.print("RcvData = ");
      SerialUSB.println(RxData);
      switch (RxData){
       case 1:  
       Controller.writeData(10);
       if (flag == 0) {
         l_command = 0;
         r_command = 1024;
         flag = 1;
        }
        else if (flag ==1) {
       l_command = l_command + delta_v;
       r_command = r_command + delta_v;
       if (l_command >1023) {l_command=1023;}
       if (r_command <1024) {r_command=1024;} 
        }
       break;
       
       case 2:
       Controller.writeData(20);
       if (flag==1){
       l_command = 1024;
       r_command = 0;
       flag = 0;
       }
       if (flag==0){
       l_command = l_command + delta_v;
       r_command = r_command + delta_v;
       if (l_command <1024) {l_command=1024;}
       if (r_command >1023) {r_command=1023;}
       }
       break;
       
       case 4:
       if (flag == 0) {
         l_command = 0;
         r_command = 1024;
         flag = 1;
        }
        else if (flag ==1) {
       l_command = l_command - delta_v;
       r_command = r_command + delta_v;
       if (l_command >1023) {l_command=1023;}
       if (r_command <1024) {r_command=1024;} 
        }
       break;
       
       case 8:
       if (flag == 0) {
         l_command = 0;
         r_command = 1024;
         flag = 1;
        }
        else if (flag ==1) {
       l_command = l_command + delta_v;
       r_command = r_command - delta_v;
       if (l_command >1023) {l_command=1023;}
       if (r_command <1024) {r_command=1024;} 
        }
       break;
       
       case 16:
       Controller.writeData(00);
       r_command = 0;
       l_command = 0;
       break;
       
       case 32:
       Dxl.goalPosition(ID_Kick, 750); //Compatible with all dynamixel serise
       //Wait for 1 second (1000 milliseconds)
       delay(500);         
       //Turn dynamixel ID 10 to position 300
       Dxl.goalPosition(ID_Kick, 512);
       // Wait for 1 second (1000 milliseconds)
       delay(500);
       break;
       
       case 0:
       Controller.writeData(00);
       break;
       
       default:
       break;
      }
  }
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  // BEHAVIOUR
  //   r_command = 285;


  
  //r_command = r_command | 0x400;
  //l_command = l_command;
  
  Dxl.writeWord(ID_Left_Wheel, Moving_Speed, l_command);// go ahead with velocity 400
  Dxl.writeWord(ID_Right_Wheel, Moving_Speed, r_command);
  
 
 //Turn dynamixel ID 10 to position 0  
 //Dxl.writeWord(ID_Kick, Goal_Position, 300); //Compatible with all dynamixel serise
 //Wait for 1 second (1000 milliseconds)
 //delay(1000);         
 //Turn dynamixel ID 10 to position 300
 //Dxl.writeWord(ID_Kick, Goal_Position, 600);
  // Wait for 1 second (1000 milliseconds)
 //delay(1000); 
  }


