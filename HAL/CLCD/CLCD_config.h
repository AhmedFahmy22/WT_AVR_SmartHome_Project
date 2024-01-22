/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : HAL                                                 */
/* SWC     : CLCD                                                */
/* Version : 1.0                                                 */
/* Date    : 11 Dec 2023                                         */
/*****************************************************************/
#ifndef CLCD_CONFIG_H_
#define CLCD_CONFIG_H_

/*Macros*/
#define CLCD_CURSOR_MOVE_LEFT  0
#define CLCD_CURSOR_MOVE_RIGHT 1

#define CLCD_DISPLAY_SHIFT_OFF 0
#define CLCD_DISPLAY_SHIFT_ON  1

#define CLCD_CURSOR_DISPLAY_OFF 0
#define CLCD_CURSOR_DISPLAY_ON  1

#define CLCD_CURSOR_BLINK_OFF 0
#define CLCD_CURSOR_BLINK_ON  1

#define CLCD_SHIFT_PATTERN_CURSOR_LEFT   0
#define CLCD_SHIFT_PATTERN_CURSOR_RIGHT  1
#define CLCD_SHIFT_PATTERN_DISPLAY_LEFT  3
#define CLCD_SHIFT_PATTERN_DISPLAY_RIGHT 4

#define CLCD_DATA_LENGTH_8BIT 0
#define CLCD_DATA_LENGTH_4BIT 1

#define CLCD_LINES_NUM_1 0
#define CLCD_LINES_NUM_2 1

#define CLCD_CHAR_FONT_5X7  0
#define CLCD_CHAR_FONT_5X10 1

/*LCD Configurations*/
#define CLCD_CURSOR_MOVE CLCD_CURSOR_MOVE_RIGHT

#define CLCD_DISPLAY_SHIFT CLCD_DISPLAY_SHIFT_OFF

#define CLCD_CURSOR_DISPLAY CLCD_CURSOR_DISPLAY_OFF

#define CLCD_CURSOR_BLINK CLCD_CURSOR_BLINK_OFF

#define CLCD_SHIFT_PATTERN CLCD_SHIFT_PATTERN_CURSOR_RIGHT

#define CLCD_DATA_LENGTH CLCD_DATA_LENGTH_8BIT

#define CLCD_LINES_NUM CLCD_LINES_NUM_2

#define CLCD_CHAR_FONT CLCD_CHAR_FONT_5X7

/*LCD Pins Configuration*/

/*Control pins*/
#define CLCD_CONTROLPORT    DIO_PORTC
#define CLCD_RSPIN          DIO_PIN2
#define CLCD_RWPIN          DIO_PIN3
#define CLCD_EPIN           DIO_PIN4

/*Data pins (Only D4 to D7 for 4-bit mode)*/
#define CLCD_D0_PORT       DIO_PORTB
#define CLCD_D0_PIN        DIO_PIN3

#define CLCD_D1_PORT       DIO_PORTB
#define CLCD_D1_PIN        DIO_PIN4

#define CLCD_D2_PORT       DIO_PORTB
#define CLCD_D2_PIN        DIO_PIN5

#define CLCD_D3_PORT       DIO_PORTB
#define CLCD_D3_PIN        DIO_PIN6

#define CLCD_D4_PORT       DIO_PORTB
#define CLCD_D4_PIN        DIO_PIN7

#define CLCD_D5_PORT       DIO_PORTC
#define CLCD_D5_PIN        DIO_PIN5

#define CLCD_D6_PORT       DIO_PORTC
#define CLCD_D6_PIN        DIO_PIN6

#define CLCD_D7_PORT       DIO_PORTC
#define CLCD_D7_PIN        DIO_PIN7



#endif
