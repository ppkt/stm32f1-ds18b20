#ifndef __DS18B20_H__
#define __DS18B20_H__
#include "one_wire.h"
#include "math.h"
#include "utils.h"

void ds18b20_init(GPIO_TypeDef *gpio, u16 port, TIM_TypeDef *timer);
void ds18b20_set_precission(u8 precission);
void ds18b20_convert_temperature_simple();
void ds18b20_read_temperature_simple();
void ds18b20_wait_for_conversion();

#endif // __DS18B20_H__
