/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Example sketch showing how to control physical relays.
 * This example will remember relay state after power failure.
 * http://www.mysensors.org/build/relay
 */

// Enable debug prints to serial monitor
//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

#include <MySensors.h>
#include <elapsedMillis.h>

elapsedMillis timeElapsed; 
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define MAX_DEVICES 4
#define CLK_PIN   5
#define DATA_PIN  7
#define CS_PIN    6


uint8_t scrollSpeed = 25;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_CENTER;
uint16_t scrollPause = 2000; // in milliseconds
#define  BUF_SIZE  75
char curMessage[BUF_SIZE] = { "PUSH HAND TO PLAY" };
bool newMessageAvailable = true;
#define MY_NODE_ID 2
MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

MyMessage msg1(1,V_CUSTOM);
MyMessage msg2(1,V_STATUS);

int time = 60;

void before()
{

	
}

void setup()
{
P.begin();
P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("timerPerfection", "1.0");


	present(1, S_CUSTOM, "Time");
  present(2, S_BINARY, "Count");
  
	
}

boolean count = false;
char buff[50];
char newbuff[50];
void loop()
{

if(P.displayAnimate()){
    P.displayText(curMessage, scrollAlign, scrollSpeed, 1000, scrollEffect, PA_NO_EFFECT);
    unsigned long interval = 1000;
    long rtime = (long)time-(timeElapsed/interval);
    String stringTime = String(rtime);
    stringTime.toCharArray(buff, 50);
    char* point = buff;
  if(count){
    P.displayText(point, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    if(rtime < 1){
      count = false;
      send(msg2.set(count));
  }
  }
}

}

void receive(const MyMessage &message)
{
	if (message.type==V_VAR1) {
    Serial.println(message.getInt());
		time = message.getInt();
	}else if(message.type==V_STATUS){
  Serial.println(message.getBool());
  if(message.getBool()==1){

  count = true;
  timeElapsed = 0;
  }else{
  count = false;

}
}
}


