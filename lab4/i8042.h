#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for i8042 KEYBOARD CONTROLLER
 */

#define BIT(n) (0x01<<(n))

//KEYBOARD VARIABLES
#define KBD_IRQ	        	1    /**< @brief Keyboard 0 IRQ line */
#define STAT_REG 			0x64
#define OUT_BUF 			0x60
#define OBF 				BIT(0)
#define PAR_ERR				BIT(7)
#define TO_ERR 				BIT(5)
#define DELAY_US    		20000
#define BREAKCODE 			0x81
#define CODETYPE 			BIT(7)
#define CHANGELEDS 			0xED

//MOUSE VARIABLES
#define MOUSE_IRQ 			12
#define IBF 				BIT(0)
#define IN_BUF	 			0x64
#define BYTETOMOUSE 		0xD4
#define ENABLE_PACKETS 		0xF4
#define ISFIRSTPACKET		BIT(3)
#define LB					BIT(0)
#define RB					BIT(1)
#define MB					BIT(2)
#define XOV					BIT(6)
#define YOV					BIT(7)

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

/**@}*/

#endif /* _LCOM_I8042_H */
