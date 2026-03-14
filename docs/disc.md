# CD Mastering

The format of the binary data on a Compact Disc is defined by a series of standards developed over the years. For the Mega CD system, CD Audio (the Red Book standard) and CD-ROM (the Yellow Book standard) formats are primarily used. CD Audio is the original and most prevalent format, containing 16-bit stereo PCM audio divided into tracks. CD-ROM was developed later and is used to store data in the ISO9660 file system.

These formats can be used together (what is called a "mixed mode" disc) and this is how you can use CD audio playback in your game. If you are unfamiliar with creating such discs, please read through the notes below.

## ISO Files

There are often misconceptions about CD images. You may be familiar with ISO files and consider them an "image" of the disc. This is not entirely true. An ISO file is a dump of the ISO9660 file system within the CD-ROM data track only. While it is often true that the ISO file makes up the entirety of the readable data on the disc, it is not a true image inasmuch as there is data on the disc that is present outside of the file system that has not been copied, such as the lead in/out or the subcode channels. And if the disc was mixed-mode with CD audio, the ISO file will not include that audio.

When building your game in Megadev, an ISO file will be generated. This is your CD-ROM data track. You can run this in an emulator burn it to a CD as-is. The emulator or burning software will generate the necessary metadata to read/burn a simple CD-ROM disc.

## CD Audio

CD audio (CD-DA) is 16 bit stereo PCM data sampled at 44.1khz. Your burning software may allow you to use a variety of audio formats, but ideally your source audio should match this format: lossless 16 bit stereo 44.1khz WAV files. Lossy formats such as MP3 should be avoided if possible.

The source audio is not included on the data track, but is processed separately to create audio tracks. That is, do not put your .WAV files in your `DISC_PATH`. They should be stored elsewhere, such as in `RES_PATH`.

The order of the tracks is then specified with a CUE sheet.

## CUE Sheets

At the start of all CDs is a "lead-in" area which includes the definition of the layout of the disc. This layout information is called the Table of Contents (TOC), and it specifies the start and stop locations of each track. In order to include CD audio, we need to specify such a layout for our audio tracks as well as the CD-ROM data track.

A CUE file is a text file that represents the TOC on the disc. It is a simple format where the source files on your computer are specifed by the track number they should appear on the disc. For mixed-mode CDs, the data (CD-ROM) track will always be track 1 while audio tracks come after it. As an example:

    FILE "disc1.iso" BINARY
      TRACK 01 MODE1/2048
        INDEX 01 00:00:00
    FILE "track02.wav" WAVE
      TRACK 02 AUDIO
        INDEX 00 00:00:00
        INDEX 01 00:02:00
    FILE "track03.wav" WAVE
      TRACK 03 AUDIO
        INDEX 00 00:00:00
        INDEX 01 00:02:00

The basic concept is to specify a source file, a track type and a time index, which is relative to the end of the previous entry. For track 1, we specify mode 1, which is the standard CD-ROM format allowing for 2048 bytes per sector. For audio tracks, we indicate it is as such and specify two time indices: the first is the pre-gap, which is played as one track naturally transitions to the next, and the second is the start point when seeking to the start of the track. Per the official Mega CD documentation, due to the seek time of the drive, there should be a 2 second pre-gap on each audio track. This acts as a buffer so that the audio from another track does not accidentally partially play.

Note that this pregap period must exist in your source audio. That is, you must have 2 seconds of silence at the start of the actual audio file to account for this gap. If not, your audio will be cut off by two seconds at the start. You can easily add 2 seconds to your tracks with ffmpeg like so:

    ffmpeg -i track02.wav -af "aformat=s16:44100, adelay=2000|2000" track02_pregap.wav

Where the numeric value specified is milliseconds of blank audio.

You will find `example.cue` within the `etc` directory of the megadev directory. You can copy this file and modify the audio track entries as necessary.
