/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file bramcart.def.h
 * @brief Function codes for the BRAM_CART vector
 */

#ifndef MEGADEV__MAIN_BRAMCART_DEF_H
#define MEGADEV__MAIN_BRAMCART_DEF_H

#define BRAM_CART 0xFFFDAE

/*
https://gendev.spritesmind.net/forum/viewtopic.php?t=826
400000-4FFFFF : Cart ID; reads return 0x04 on D7-D0 only.
680000-6BFFFF : 1st 128Kx8 RAM chip; data on D7-D0 only.
6C0000-6FFFFF : 2nd 128Kx8 RAM chip; data on D7-D0 only. (not used)
700000-7FFFFF : Writes by /LWR latch D0; 0=RAM write enabled, 1=disabled
*/
#define BRAM_CART_ID 0x400001

#define BRAM_CART_BANK0 0x680000

#define BRAM_CART_BANK1 0x6C0000

#define BRAM_CART_WRITE_ENABLE 0x7FFFFF

#endif
