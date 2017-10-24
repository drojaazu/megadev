

// Volume ID - 11 bytes
#define cfg_vol_id    "HELLO      "
//                    "###########"
// Overseas title - 48 bytes
#define cfg_name_intl "HELLO WORLD                                     "
//                    "################################################"
// Domestic (Japanese) title - 48 bytes
// (You can use Shift JIS encoded Japanese here if you'd like, however, take
// that you consider which characters are double byte, so that the whole
// string fits in 48 bytes)
#define cfg_name_jp   "HELLO WORLD                                     "
//                    "################################################"
// Game code - 16 bytes
// Traditionally in the format: GM 00-0000-00
#define cfg_game_id   "GM 00-0000-00   "
//                    "################"
// Region - 16 bytes
// Since the region is tied to the BIOS/security code anyway, you may
// as well leave this as JUE
#define cfg_region    "JUE             "
//                    "################" 
