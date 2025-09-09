
#include "bridge.h"
#include <sub/cdrom.h>
#include <sub/gate_arr.h>
#include <sub/memmap.h>
#include <sub/pcm.h>

// void PCM_PLAYBACK_C(u8 * pcm_data, u32 pcm_data_size);

char const * const filenames[] = {"IPX.MMD;1"};

const PcmChannelSettings pcmSettings = {0xFF, 0xFF, 0x6B, 0x5, 0, 0, 0};

static inline void PCM_PLAYBACK_C(u8 * pcm_data, u32 pcm_data_size)
{
	register u32 a0_pcm_data asm("a0") = (u32) pcm_data;
	register u32 d0_data_size asm("d0") = pcm_data_size;

	asm volatile("jsr PCM_PLAYBACK"
							 : "+d"(d0_data_size), "+a"(a0_pcm_data)
							 : "d"(d0_data_size), "a"(a0_pcm_data)
							 : "cc", "d6", "d7", "a1", "a2");
}

extern void sp_fatal();

__attribute__((section(".init"))) void main()
{
	register u16 command, param1;

	do
	{
		do
		{
			command = *gareg_comcmd0;
		} while (command == 0);

		if (command != *gareg_comcmd0)
			continue;

		param1 = *gareg_comcmd1;

		switch (command)
		{
			case CMD_LOAD_WORDRAM:
				load_file(CDROM_LOAD_CDC, filenames[param1], (u8 *) WORD_RAM_2M);
				grant_2m();
				if (access_op_result != CDROM_RESULT_OK)
				{
					sp_fatal();
				}
				break;

			case CMD_LOAD_PRGRAM:
				load_file(CDROM_LOAD_CDC, "AUDIO.PCM;1", (u8 *) PRG_RAM_BANK3);
				if (access_op_result != CDROM_RESULT_OK)
				{
					sp_fatal();
				}
				break;

			case CMD_PLAY_PCM:
				pcm_clear_ram_c();

				pcm_config_channel_c(CHANNEL(1), &pcmSettings);

				// we use the highest bit of the comflags to inform main that the
				// audio is playing
				// (be careful about setting low bits on COMFLAGS when using the
				// default VINT handler in the Main Boot ROM Library; see the
				// Predefined Comm Flag Semantics section of bootrom.md for more
				// info)
				*GA_COMFLAGS_SUB |= 0x80;
				// pcm_playback((u8 *)PRG_RAM_BANK3, 0x39BC1);
				// pcm_playback((u8 *)PRG_RAM_BANK3, 0x40000);
				PCM_PLAYBACK_C((u8 *) PRG_RAM_BANK3, 0x40000);
				*((volatile u8 *) _PCM_CDISABLE) = 0xFF;

				*GA_COMFLAGS_SUB &= ~0x80;
				break;
		}

		*GA_COMSTAT0 = *gareg_comcmd0;
		do
		{
			asm("nop");
			command = *gareg_comcmd0;
		} while (command != 0);

		do
		{
			asm("nop");
			command = *gareg_comcmd0;
		} while (command != 0);

		*GA_COMSTAT0 = 0;

	} while (1);
}

/**
 * This is a very simple playback driver.
 * All PCM data must be already loaded into the buffer
 * Audio is played on channel 1
 *
 * The data is divided into 32kb blocks, with any remainder
 * not played
 * For this reason, the file should be padded out to the next
 * 32kb boundary to have the full clip played
 *
 * (I said it was simple. It can certainly be expanded to support
 * oddly sized files and streaming from disc with some further
 * modification.)
 */
void pcm_playback(u8 * pcm_data, u32 pcm_data_size)
{

	bool pcm_put_upper = false;
	// 32kb blocks
	u8 pcmram_blocks = pcm_data_size / 0x8000;

	// wave bank select
	// put initial block of data into lower bank
	u8 wb_select = WAVEBANK(0);
	for (u8 copy_iter = 0; copy_iter < 8; ++copy_iter)
	{
		*PCM_CTRL = wb_select;
		u8 * pcm_ram = (u8 *) (_PCM_RAM + 1);
		for (u16 addr = 0; addr < 0x1000; ++addr)
		{
			*pcm_ram = *pcm_data++;
			pcm_ram += 2;
		}
		++wb_select;
	}
	pcm_put_upper = true;

	// begin playback on lower bank
	*PCM_CDISABLE = 0b11111110;

	// begin looping over the rest of the data, loading from the upper
	// wave ram block
	for (u16 block_iter = 0; block_iter < (pcmram_blocks - 1); ++block_iter)
	{
		if (! pcm_put_upper)
			wb_select = 0x80;

		for (u8 copy_iter = 0; copy_iter < 8; ++copy_iter)
		{
			*PCM_CTRL = wb_select;
			u8 * pcm_ram = (u8 *) (_PCM_RAM + 1);
			for (u16 addr = 0; addr < 0x1000; ++addr)
			{
				*pcm_ram = *pcm_data++;
				pcm_ram += 2;
			}
			++wb_select;
		}

		if (pcm_put_upper)
		{
			while (*((volatile u8 *) _PCM_PLAY_CH1_H) <= 0x7F)
			{
				asm("nop");
				asm("nop");
			}
		}
		else
		{
			while (*((volatile u8 *) _PCM_PLAY_CH1_H) > 0x7F)
			{
				asm("nop");
				asm("nop");
			}
		}
		pcm_put_upper = ! pcm_put_upper;
	}

	// wait to finish playback before returning
	if (pcm_put_upper)
	{
		while (*((volatile u8 *) _PCM_PLAY_CH1_H) <= 0x7F)
		{
			asm("nop");
			asm("nop");
		}
	}
	else
	{
		while (*((volatile u8 *) _PCM_PLAY_CH1_H) > 0x7F)
		{
			asm("nop");
			asm("nop");
		}
	}
}
