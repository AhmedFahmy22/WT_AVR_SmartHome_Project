#ifndef CLCD_EXTRA_H_
#define CLCD_EXTRA_H_

#define CLCD_CUSTOM_CHAR_NUM 4

uint8 au8CharGlb[CLCD_CUSTOM_CHAR_NUM][8] = {
        {0b00100,
         0b01010,
         0b10001,
         0b00000,
         0b10001,
         0b01010,
         0b00100,
		 0},
		{0b00100,
		 0b01010,
		 0b10001,
		 0b00000,
		 0b00000,
		 0b00000,
		 0b00000,
		 0},
		{0b00000,
		 0b00000,
		 0b00000,
		 0b00000,
		 0b10001,
		 0b01010,
		 0b00100,
		 0},
        {0b00000,
         0b01000,
         0b00100,
		 0b00010,
		 0b00100,
		 0b01000,
         0b00000,
         0}
};


#endif
