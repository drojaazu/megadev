# pcm_playback
PCM audio playback

This is an example of playing audio through the Mega CD PCM chip. It streams one PCM file from the disc and plays it through channel 1.

The chip plays all 64 KB of its wave RAM in a loop. While it plays one half, the Sub CPU fills the other half straight from the CDC by DMA, one 4 KB bank (two sectors) at a time, using the access loop's partial loads (`load_file_part`). Nothing is staged in Program RAM, so the file can be any length; the example's is 256 KB. The Sub CPU reads each bank back and keeps a byte sum, left in COMSTAT3 when playback ends, so an emulator's debugger can compare it with the file.

The PCM data is made up of signed 8-bit values, though the format is slight different from standard signed PCM data:

 - 0x00 - 0x7F corresponds to [+0, +127]
 - 0x80 - 0xFE corresponds to [-0, -126]
 - 0xFF is reserved as a loop marker

The easiest way to convert your audio is to export as a standard 8 bit signed WAV file and use the `scdwav2pcm` tool with SCDTools:

https://github.com/classiccoding/scdtools

This tool will also helpfully give you the frequency value to set on the channel to ensure proper playback speed.

