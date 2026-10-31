/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file std_init_stack.s
 * @brief std_init.s that also moves the stack to STACK_TOP, for a module that
 * never returns. See "Moving the Stack" in docs/dev_in_c.md.
 */

#define INIT_SET_STACK
#include <std_init.s>
