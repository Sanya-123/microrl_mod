/*
 * comands.c
 *
 *  Created on: 14 июл. 2020 г.
 *      Author: user
 */

#include "comands.h"
#include "console_mod/microrl.h"
#include "console_mod/misc.h"
#include <string.h>
#include "xgpio.h"

extern XGpio gpioOut;//extern gpio


//my cmds
const char _CMD_TIME[] =            "time";
int time_cmd(void * vpThis, int argc, const char * const * argv)//задача запускающая задачи и смотри сколько на них тратиться времени
{
    if(argc == 1)
    {
        __print(vpThis, "Programm 'time' incluse programm in second argument and use timer\n\r");
        __print(vpThis, "you second argument is empty!\n\r");
        return -1;
    }
    //TODO add timer
//    TickType_t LastTick = xTaskGetTickCount();
    execute(vpThis, argc - 1, &argv[1]);//вызываю 2 аргумент
//    TickType_t timeUse = (xTaskGetTickCount() - LastTick) / portTICK_PERIOD_MS;//in ms
    char buf[32];
    sprintf(buf, "%dms\n\r", /*timeUse*/0);
    __print(vpThis, "For task(");
    __print(vpThis, argv[0]);
    __print(vpThis, ") use time: ");
    __print(vpThis, buf);
    return 0;
}

const char GPIO_CMD_LED[] =            "gpio";
const char GPIO_HELP_LED[] =           "on or off led on board";
int gpio_cmd(void * vpThis, int argc, const char * const * argv)
{
	auto void printHelp()//синтаксис для функции внутри функции
	{
		__print(vpThis, "Error input argument\n\r");
		__print(vpThis, "Syntaxys:gpio [number 0-31] [on/off]Example: \n\r");
		__print(vpThis, "\tgpio  1 on\n\r");
	}

	if(argc != 3)
	{
		printHelp();
		return -1;
	}

	if((strlen(argv[1]) > 2) || ((strncmp(argv[2], "on", 2) != 0) && (strncmp(argv[2], "off", 3) != 0)))//если первый аргумент больше 1 или 2 аргкмент ни равен ни off ни on
	{
		printHelp();
		return -1;
	}

	if(atoi(argv[1]) > 31)
	{
		printHelp();
		return -1;
	}





	if(strncmp(argv[2], "off", 3) == 0)
	{
		XGpio_DiscreteClear(&gpioOut, 1, 1 << (atoi(argv[1])));
		xil_printf("Off gpio %d \n\r", atoi(argv[1]));
	}
	else if(strncmp(argv[2], "on", 2) == 0)
	{
		XGpio_DiscreteSet(&gpioOut, 1, 1 << (atoi(argv[1])));
		xil_printf("On gpio %d \n\r", atoi(argv[1]));
	}

	return 0;
}

const char DATA_CMD_LED[] =            "data";
const char DATA_HELP_LED[] =           "number of harmonic";
int data_cmd(void * vpThis, int argc, const char * const * argv)
{
	auto void printHelp()//синтаксис для функции внутри функции
	{
		__print(vpThis, "Error input argument\n\r");
		__print(vpThis, "Syntaxys:data [number 0-255]Example: \n\r");
		__print(vpThis, "\tdata  127\n\r");
	}

	if(argc != 3)
	{
		printHelp();
		return -1;
	}

	uint8_t data = atoi(argv[1]);
	uint8_t data1 = atoi(argv[2]);

	XGpio_DiscreteClear(&gpioOut, 1, 0x0000FFFF << 16);
	XGpio_DiscreteSet(&gpioOut, 1, data << 24);
	XGpio_DiscreteSet(&gpioOut, 1, data1 << 16);
	xil_printf("Set %d hatrmonic\n\r", data);
	xil_printf("or band %d...%d\n\r", data, data1);

	return 0;
}

void initMyCMD()
{
	addCmd(GPIO_CMD_LED, GPIO_HELP_LED, gpio_cmd);
	addCmd(DATA_CMD_LED, DATA_HELP_LED, data_cmd);
}
