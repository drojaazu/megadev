/**
 * @file ipx_jmptbl.s
 * @brief The IPX jump table
 *
 * Generated from IPX_API_LIST so it cannot drift out of step with the call
 * macros in ipx.h. The linker script pins this section to the start of the
 * module, so every entry is at a fixed, known address.
 */

#include "ipx_api.def.h"

.section .jmptbl

#define X(n, name) jmp name;
IPX_API_LIST
#undef X
