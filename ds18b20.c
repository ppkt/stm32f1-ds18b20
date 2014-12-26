#include "ds18b20.h"

u8 precission = 3;

void ds18b20_init(GPIO_TypeDef *gpio, u16 port, TIM_TypeDef *timer) {
	one_wire_init(gpio, port, timer);
}

void ds18b20_set_precission(u8 p) {
	precission = p;
	one_wire_reset_pulse();

	one_wire_write_byte(0xCC); // Skip ROM
	one_wire_write_byte(0x4E); // Write scratchpad

	one_wire_write_byte(0x4B);
	one_wire_write_byte(0x46);
	// set precission
	one_wire_write_byte(0x1F | (precission << 5));
}

void ds18b20_convert_temperature() {
	one_wire_reset_pulse();

	one_wire_write_byte(0xCC); // Skip ROM
	one_wire_write_byte(0x44); // Convert temperature
}

void ds18b20_read_scratchpad() {
	one_wire_reset_pulse();
	one_wire_write_byte(0xCC); // Skip ROM
	one_wire_write_byte(0xBE); // Read scratchpad

	int i;
	u8 data[9];

//	printf("\r\n");
	for (i = 0; i < 9; ++i) {
		data[i] = one_wire_read_byte();
//		printf("%X ", data[i]);
	}

//	printf("\r\n\0");

	u8 temp_msb = data[1]; // Sign byte + lsbit
	u8 temp_lsb = data[0]; // Temp data plus lsb
    float temp = (temp_msb << 8 | temp_lsb) / powf(2, 4);
    int rest = (temp - (int)temp) * 1000.0;
    printf("%d.%d\r\n", (int)temp, rest);
}

void ds18b20_wait_for_conversion() {
	if (precission == 0) {
		delay(95);
	} else if (precission == 1) {
		delay(190);
	} else if (precission == 2) {
		delay(380);
	} else if (precission == 3) {
		delay(750);
	}
}
