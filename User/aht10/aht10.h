#ifndef __AHT10_H_
#define __AHT10_H_

#include "stdint.h"
#include "./usart/bsp_debug_usart.h"
#include "stm32f4xx.h"
#include "./systick/bsp_SysTick.h"
#include "./led/bsp_led.h"

#define AHT_I2C_SCL_GPIO_PORT GPIOB
#define AHT_I2C_SCL_PIN GPIO_Pin_3

#define AHT_I2C_SDA_GPIO_PORT GPIOB
#define AHT_I2C_SDA_PIN GPIO_Pin_4

//软件IIC使用的宏
#define AHT_I2C_SCL_1()  GPIO_SetBits(AHT_I2C_SCL_GPIO_PORT, AHT_I2C_SCL_PIN)		/* SCL = 1 */
#define AHT_I2C_SCL_0()  GPIO_ResetBits(AHT_I2C_SCL_GPIO_PORT, AHT_I2C_SCL_PIN)		/* SCL = 0 */

#define AHT_I2C_SDA_1()  GPIO_SetBits(AHT_I2C_SDA_GPIO_PORT, AHT_I2C_SDA_PIN)		/* SDA = 1 */
#define AHT_I2C_SDA_0()  GPIO_ResetBits(AHT_I2C_SDA_GPIO_PORT, AHT_I2C_SDA_PIN)		/* SDA = 0 */

#define I2C_SDA_READ()  GPIO_ReadInputDataBit(AHT_I2C_SDA_GPIO_PORT, AHT_I2C_SDA_PIN)	/* 读SDA口线状态 */

#endif /*__AHT10_H_*/

