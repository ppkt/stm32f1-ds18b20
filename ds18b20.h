#ifndef __DS18B20_H__
#define __DS18B20_H__
#include "one_wire.h"
#include "math.h"
#include "utils.h"
#include "usart.h"
#include <stdlib.h>

// Structure in which temperature is stored
typedef struct {
    s8 integer;
    u16 fractional;
} simple_float;

// Structure for returning list of devices on one wire
typedef struct {
    u8 size;
    one_wire_device *devices;
} ds18b20_devices;

void ds18b20_init(GPIO_TypeDef *gpio, u16 port, TIM_TypeDef *timer);
void ds18b20_set_precission(u8 precission);
ds18b20_devices ds18b20_get_devices();

void ds18b20_convert_temperature_simple(void);
simple_float ds18b20_read_temperature_simple(void);

void ds18b20_convert_temperature_all(void);
simple_float* ds18b20_read_temperature_all(void);

void ds18b20_wait_for_conversion(void);
simple_float ds18b20_decode_temperature(void);

#endif // __DS18B20_H__
