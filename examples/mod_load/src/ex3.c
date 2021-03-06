
#include "io_def.h"
#include "main/boot.h"
#include "main/main.h"
#include "types.h"
#include "vdp.h"

extern u16 global_mode;

void main() {
  boot_print("Example file Number Three!\xff",
             (to_vdpptr(NMT_POS_PLANE(10, 10, BOOT_PLANEA_ADDR)) | VRAM_W));

  do {
    boot_vint_wait_default();
  } while (!(*JOY1_PRESS & PAD_START_MSK));

  global_mode = 0;
  return;
}
