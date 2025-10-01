/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file sub/xform.h
 * @brief Sub CPU side graphics rendering utilities
 * @todo this needs some renaming...
 */

#ifndef MEGADEV__SUB_XFORM_H
#define MEGADEV__SUB_XFORM_H

typedef short xform_pos;
typedef short xform_delta;

#define XFORM_POS_INT_BITS	13
#define XFORM_POS_FRAC_BITS (16 - XFORM_POS_INT_BITS)
#define XFORM_POS_FRAC_MASK (1 << XFORM_POS_FRAC_BITS)

#define to_xform_pos(value) ((xform_pos) ((value) * (XFORM_POS_FRAC_MASK)))

#define XFORM_DELTA_INT_BITS	5
#define XFORM_DELTA_FRAC_BITS (16 - XFORM_DELTA_INT_BITS)
#define XFORM_DELTA_FRAC_MASK (1 << XFORM_DELTA_FRAC_BITS)

#define to_xform_delta(value)                         \
	((xform_delta) ((value) * (XFORM_DELTA_FRAC_MASK)))

#endif