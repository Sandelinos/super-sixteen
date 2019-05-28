#include "Font.h"

uint8_t font_table[] = {
	0b1111110,   // [0] => "0"
	0b0110000,   // [1] => "1"
	0b1101101,   // [2] => "2"
	0b1111001,   // [3] => "3"
	0b0110011,   // [4] => "4"
	0b1011011,   // [5] => "5"
	0b1011111,   // [6] => "6"
	0b1110000,   // [7] => "7"
	0b1111111,   // [8] => "8"
	0b1111011,   // [9] => "9"
	0b1110111,   // [10] => "A"
	0b0011111,   // [11] => "b"
	0b1001110,   // [12] => "C"
	0b0111101,   // [13] => "d"
	0b1001111,   // [14] => "E"
	0b1000111,   // [15] => "F"
	0b0000001,   // [16] => "dash"
	0b0000000,   // [17] => "blank"
};

uint8_t font[18];

void initializeFont() {
	for (int pin = 0; pin < 17; pin++) { //reversing bits, wired A-G on 7-0 for some reason
		int num = font_table[pin];
		font[pin] = ((num & 0x01) << 7)
			| ((num & 0x02) << 5)
			| ((num & 0x04) << 3)
			| ((num & 0x08) << 1)
			| ((num & 0x10) >> 1)
			| ((num & 0x20) >> 3)
			| ((num & 0x40) >> 5)
			| ((num & 0x80) >> 7);
	}
}