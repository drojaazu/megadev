# CD Mastering
The format of the binary data on a Compact Disc is defined by a series of standards developed over the years. For the Mega CD system, CD Audio (the Red Book standard) and CD-ROM (the Yellow Book standard) formats are used. CD Audio is the original and most prevalent format, containing 16-bit stereo PCM audio divided into tracks. CD-ROM was developed later and is used to store data in the ISO9660 file system.

These formats can be mixed (what is called a "mixed mode" disc) and this is how you can use CD audio playback in your game. However, if you are unfamiliar with creating such discs, please read through the notes below.

## The ISO file
There are often misconceptions about CD images. You may be familiar with ISO files and consider them an "image" of the disc. This is not entirely true. An ISO file is a dump of the data track on the CD-ROM, an image of the ISO9660 file system only. It is likely, in the case of a standard CD-ROM, that the ISO is the entirety of the readable data on the disc, but it is not a true image inasmuch as it has not copied e.g. the lead in/out or the subcode channels. It also does not include the CD audio.

When building your game in Megadev, an ISO file will be generated. You can run this in an emulator burn it to a CD as-is. The emulator or burning software will generate the necessary metadata to read/burn a simple, non-mixed mode CD-ROM disc.

## Including Files on the Disc
By default, there will be a `disc` directory created in your project. Any files you add to this directory will be added to the final ISO image. All modules and other final binaries generated from the project build will automatically be added to this directory before the ISO is created.

## CD Audio
CA audio (CD-DA) is 16 bit stereo PCM data sampled at 44.1khz. Your burning software may allow you to use a variety of audio formats, but ideally your source audio should be a 16 bit stereo 44.1khz WAV file. Lossy formats such as MP3 should be avoided if possible.

You cannot simply add a .WAV or .MP3 file to the `disc` directory and expect to have CD audio. This is because CD-DA is a different format than CD-ROM. The burning software will deal with the conversion of the audio file to the correct CD-DA data, but we must specify the track layout on the disc.

## The CUE sheet
At the start of all CDs is a "lead-in" area which includes the definition of the layout of the disc. This layout information is called the Table of Contents (TOC), and it specifies the start and stop locations of each track. In order to include CD-DA, we need to specify such a layout for our audio tracks as well as the CD-ROM data track.

A CUE file is a text file that represents the TOC on the disc. It is a simple format where the source files on your computer are specifed by the track number they should appear on the disc. For mixed-mode CDs, the data track will always be track 1 while audio tracks come after it. As an example:

    FILE "game.iso" BINARY
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
