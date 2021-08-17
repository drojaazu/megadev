# pcm_playback
PCM audio playback

This is an example of playing audio through the Mega CD PCM chip. It will load and play one PCM file through channel 1.

The PCM data is made up of signed 8-bit values, though the format is slight different from standard signed PCM data:

 - 0x00 - 0x7F corresponds to [+0, +127]
 - 0x80 - 0xFE corresponds to [-0, -126]
 - 0xFF is reserved as a loop marker

The easiest way to convert your audio is to export as a standard 8 bit signed WAV file and use the `scdwav2pcm` tool with SCDTools:

https://github.com/classiccoding/scdtools

This tool will also helpfully give you the frequency value to set on the channel to ensure proper playback speed.

