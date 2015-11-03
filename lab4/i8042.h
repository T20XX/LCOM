#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for i8042 KEYBOARD CONTROLLER
 */

#define BIT(n) (0x01<<(n))

#define KBD_IRQ	        1    /**< @brief Keyboard 0 IRQ line */
#define STAT_REG 0x64
#define OUT_BUF 0x60
#define OBF BIT(0)
#define PAR_ERR BIT(7)
#define TO_ERR BIT(5)
#define DELAY_US    20000
#define BREAKCODE 0x81
#define CODETYPE BIT(7)
#define CHANGELEDS 0xED


/**@}*/

#endif /* _LCOM_I8042_H */
