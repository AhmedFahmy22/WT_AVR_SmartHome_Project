/*****************************************************************/
/* Author  : Ahmed                                               */
/* Layer   : MCAL                                                */
/* SWC     : I2C                                                 */
/* Version : 1.0                                                 */
/* Date    : 27 Dec 2023                                         */
/*****************************************************************/
#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_


#define I2C_DIVISION_FACTOR 2
/**Options
* 0 to 255
*/


#define I2C_STATE I2C_STATE_ENABLE
/**Options
* I2C_STATE_DISABLE
* I2C_STATE_ENABLE
*/


#define I2C_INTERRUPT I2C_INTERRUPT_DISABLE
/**Options
* I2C_INTERRUPT_DISABLE
* I2C_INTERRUPT_ENABLE
*/


#define I2C_BITRATE_PRESCALER I2C_BITRATE_PRESCALER_1
/**Options
* I2C_BITRATE_PRESCALER_1
* I2C_BITRATE_PRESCALER_4
* I2C_BITRATE_PRESCALER_16
* I2C_BITRATE_PRESCALER_64
*/


#define I2C_GENERAL_CALL I2C_GENERAL_CALL_DISABLE
/**Options
* I2C_GENERAL_CALL_DISABLE
* I2C_GENERAL_CALL_ENABLE
*/

#define I2C_MASTER_ADDRESS 10

#define I2C_SLAVE_ADDRESS  20

#endif
