/*
 * app.cpp
 *
 *  Created on: 30 марта 2016
 *      Author: shurik
 */
#include <app.h>
#include <dragonmaster.h>


void AppClass::init()
{
	ApplicationClass::init();
	Serial.printf("AppClass init done!\n");
}

void AppClass::start()
{
	ApplicationClass::start();
	Serial.printf("AppClass start done!\n");
}

void AppClass::_loop()
{
	ApplicationClass::_loop();
//	Serial.printf("AppClass loop\n");

}
