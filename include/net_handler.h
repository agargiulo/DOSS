/*
 * File: include/net_handler.h
 *
 * Author: Anthony Gargiulo
 *
 * Description: ISR for the network card
 */

#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

#include <common.h>

#ifndef __SP2_ASM__

/*
** Prototypes
*/

/*
 * handles any and all interrupts that come in through the
 * 8255x card via the PCI controller.
 */
void _net_handler (int vector, int code);

#endif

#endif
