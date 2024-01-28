# Mega CD Program Architecture Concepts

## The Hardware
> "If you wish you wish to make an apple pie from scratch, you must first invent the universe."

The Mega Drive by itself has no operating system. The data on the cartridge is the sum of all the code that will run on the hardware. There is no standard library to reference. There is not even a simple `print` statement to have text show up on the screen, unless you write one yourself.

Writing a game for retro hardware is, in a way, writing an "operating system" from scratch, which is both a thrill and drudgery. Before you can really dig into the fun stuff, you have to plan how to manage your hardware resources (especially VRAM) and how to architect your program. The complexity of this planning/design phase escalates once we bring the Mega CD into the picture. You now have a second CPU, non-persistent memory regions and buffer ownership to manage as well.

This is why, before anything else, you should familiarize yourself with the hardware. First, read the "Mega CD Outline" official documentation. This provides a decent introduction to the hardware and its capabilities. Next, browse the "Mega CD BIOS Manual," "Mega CD Hardware Manual," and "Mega CD Software Development Manual." While don't have to read these closely (and frankly, some of the translations throughout make things even more confusing at times), but at least give them a glance to get an idea of some of the terminology used.

Before starting, you should have a basic understanding of these Mega CD specific pieces and how they work together at a high level:

  - Which components are managed by which CPU (i.e. VDP & VRAM -> Main CPU, CD Drive & Internal BRAM -> Sub CPU)
  - Work RAM
  - Word RAM
  - PRG RAM
  - The Gate Array (at a high level, you don't need to focus too much on registers...)
  - The Gate Array Comm Flags/Command/Status registers (...except for these)
  - The IP/SP

## The Kernel
Unlike a cartridge based Mega Drive game, where the entire program and all its data is present as one fixed, contiguous block of memory space, the parts of a Mega CD game come and go in the form of files read from disc. Because the data is transient, we need a "master program" that does the low level work of deciding which program file to load, executing it, and then choosing the next file to run.

This is basically the concept of an operating system kernel: the core code that remains loaded into memory for the entire duration of the system's uptime. It manages the loading and execution of user programs as well as providing services and hardware interfaces to those programs. While we certainly won't need something as complex as the kind of kernel you would find in a PC operating system, having an ever present supervisor program is the best way to design your Mega CD project.

Our kernel will have the largest scope, maintaining the global state of the entire program, and that state determines the flow of the program. For example: the system has booted, and we're at the kernel entry. We set the global state ID to 1 for 'intro screen' and enter our main loop. For state ID 1, we load `intro.mmd` into memory and jump to it. It fades in a couple nice logos, sets the global state to 2, for the title screen, then exits, and we're back in the kernel. The kernel loops again and sees that we're in state ID 2 now, so it loads and executes the title screen. From there, the user selects 1P Start or 2P Versus or Options, setting either state 3, 4 or 5 respectively. After making the selection, it exits, and we're back in the kernel, which loads the appropriate file for the state. And so on. At the most basic level, the kernel will keep track of "where we are" in the program as a whole and load/run the appropriate file for that state.

Since the kernel is loaded in memory at all times, it can also provide funcionality to the transitory modules making up most of the program. For example, maybe our game uses lots of text, so why not put the font and the print routines inside the kernel. That way we don't need to compile that code/data into each module, reducing data redundancy, code size and (most importantly) load times. Another good use case for keeping data resident is a fancy loading screen graphic. Re-loading the loading screen from disc each time we load a file is a bit silly and we're just adding additional wait time due to another disc access. Instead we can keep our fancy loading graphics in memory and the kernel can display them when making a load call. 

Given the dual-microprocessor design of the Mega CD system, the kernel will be split into two parts, running independently on either CPU. The relationship between the two, as their names imply, is that the Main CPU kernel is the "leader," maintaining the state of the program and making requests for support data to the Sub CPU kernel. In more modern terms, we can think of this as a Server/Client architecture, where the Sub CPU kernel is the server, accepting requests from and sending responses to the client, the Main CPU. The two kernels tell each other about the status of these requests and keep in sync via the Gate Array communication registers.

## The Library
Earlier we said that the Mega Drive has no operating system, that it is a devoid of any code except for what is in the cartridge. The addition of the Mega CD hardware with its internal ROM brings a CD player, memory manager and, importantly for us, a library of system functions.

On the Sub CPU side, we have the BIOS calls which are used for data I/O (namely, CD-ROM and Backup RAM access) and for controlling some miscaellaneous Mega CD specific hardware components. These calls are documented in the official Sega docs and Megadev provides definitions, structures and wrappers to help you use them in your code. You will definitely be using these in your Sub CPU kernel.

On the Main CPU side, we have what we refer to as the Boot ROM library (abbreviated to bootlib throughout Megadev). This is a set of functions that provide a wide range of general functionality useful to Mega Drive games, such as DMA operations, controller input reads, palette and sprite list caches, and much more. The catch is that some of these functions place certain expectations on your program structure and use precious Work RAM space. Moreover, these calls are not documented and our understanding of their use is based on reverse engineering.

While the Sub CPU BIOS calls are required and a basic part of Mega CD development, the Main CPU Boot ROM library is entirely optional to use. While helpful, some of the components of the library will affect your program architecture more fundamentall than others, and you should carefully examine how the pieces you wish to use will be integrated. Refer to bootrom.md for an in-depth look at this library of functions.

## Memory Planning
As we have discussed, there is no hardware delineation of read-only (ROM) and storage (RAM) for your Mega CD program. As stated in the main readme:

> The memory layout of a standard Mega Drive cartridge game is relatively simple, with a large contiguous block of address space (32 Megabits!) available for the program. Things are not so simple with the Mega CD, however. We have three seperate memory blocks (Word RAM, PRG RAM and Work RAM), all of which are extremely small in comparison (2, 4, and 0.5 Megabits, respectively).

As part of your program design, you must consider how those memory banks will be utilized and partitioned. Though each one is a block of writable memory, it's helpful to use the analogy of "ROM" and "RAM" for program code/data and for runtime memory, respectively, when speaking in terms of your program. So for the memory regions being used to run code (as opposed to being a simply buffer), we need to manually define the boundaries of "ROM" and "RAM" for the program.

While this adds a layer of complexity in your planning, it also offers some flexibility: the layouts are not required to be static and can be changed as needed. For example, maybe one module has lots of graphics data for something like a cutscene, but doesn't have much logic. It will need lots of ROM space but not much RAM. And maybe the next module will have actual gameplay, which will need a fair bit more RAM to keep track the game state. You can tailor the memory layout to each module as necessary.

### Work RAM layout
The memory block to which you will need to pay particular attention is the Main CPU Work RAM. It is the smallest of the banks but arguably the most important. This is from where the "outermost" pieces of code will execute, which is to say the kernel. The size may be further restricted depending on how much of the Boot ROM library you choose to use. For that reason, we'll take a closer look at Work RAM.

Here is its default layout, with no user programs or Boot ROM library usage:

    FF0000  +----------------+
            | IP/AP Use      |
            |                |
            |                |
            =                =
            |                |
            |                |
            |                |
            | Top of Stack   |
    FFFD00  +----------------+
            | System Use     |
            |                |
    FFFFFF  +----------------+

The System Use area, beginning at 0xFFFD00, contains the jump table for the exceptions/interrupts. The jump table is a list of JMP commands: two bytes for the opcode and a four byte pointer. The M68000 vector table is defined by the Boot ROM when it is loaded into Main CPU memory space by the hardware at startup. The vectors point to locations within this System Use partition within Work RAM and cannot be changed. Therefore, this area is considered to be off limits except for changing the pointers.

In reality, the jump table only takes up 180 bytes, leaving the space from 0xFFFDB4 and onward free for use (588 bytes). However, this free space is used by many of the Boot ROM library functions, so keep that in mind if you plan to use the built-in library. See the bootrom.md file for more info.

The stack is allocated to 0xFFFD00 by default. This can, of course, be re-pointed to wherever you'd like, though with the default layout it's already in an optimal position. Recall that the stack fills upwards, so you'll want to allocate enough space for it when planning the space for your ROM/RAM partitions above it. Too little space and it will overflow up into your RAM, possible overwriting runtime memory or runtime memory corrupting the stack. Too much space and it becomes wasted memory. This is an area of your program that you will likely want to optimize, especially if you are primarily coding in C. Lots of nested function calls with registers pushes and allocated local variables will quickly fill up the stack. Prefer using global values whenever possible to prevent too much stack usage.

Let's map out a possible memory layout, apropos of nothing. As we said earlier, our goal is to define the boundaries of "ROM" (program code and resources) and "RAM" (runtime memory) for our program within the confines of usable space.

In our program architecture, we'll load modules to Word RAM and run them directly from there. This means Work RAM is exclusive to our kernel. That simplifies things. So first, let's say 512 bytes allocated to the stack. This should actually be more than enough. (The Boot ROM allocates only 256 bytes; see bootrom.md for much more on that.) That leaves us a little less than 63KB for our ROM and RAM partitions.

The kernel won't have lots of logic by itself, let's say 1KB for global game state amd some basic services (global mode, game save information, input mirrors, random number generator, etc). Let's also keep our graphics system in the kernel as well: a VDP register cache, a sprite cache, some space for decompressing graphics, a DMA queue. Maybe anothet 4KB. We want to also keep our task processor within the kernel. Let's say a task frame is 64 bytes, and want to support a maximum of 32 tasks, meaning we need to allocate 2KB for the task system.

That's about 7KB of RAM, leaving us with just under 56KB of ROM. Our memory map looks like this:

    FF0000  +----------------+
            | "ROM"          |
            |                |
            |                |
            |                |
	    |                |
    FFDF00  +----------------+
            | "RAM"          |
            |                |
    FFFB00  +----------------+
            | Stack          |
    FFFD00  +----------------+
            | System Use     |
            |                |
    FFFFFF  +----------------+

The start address and length of the ROM and RAM sections would then be specified as global symbols within the module code. More on that in modules.md.

This example is not based on any real-world implementations, though you could use it as a generic layout to get things up and running. However, as we have said a couple times, once you bring the Boot ROM library into the picture, the space available for usage may change drastically. We'll continue with Work RAM mapping while using the library in bootrom.md.

### Memory Layout in Other Regions
We have focused heavily on Work RAM because of its importance as the location of the kernel and its trickiness due to its small size. The other memory banks are, comparitively, easier to work with.

PRG RAM has comparitively huge storage. Of the 4Mbit (512KB) of space available, only 24KB is reserved for the BIOS. Here is where the SP, the Sub CPU side kernel, will execute. You are free to allocate as much or as little of the available PRG RAM space for it as you'd like.

Word RAM is, officially, a buffer shared between the two CPUs. You can execute code from this region, and many games do just that, but it should never be used for low level code (i.e. the kernel) on either CPU. Similarly, be careful when running your vertical/horizontal interrupts from Word RAM and be sure to repoint them to default handlers in the kernel (in Work RAM) when loading another module. This is because Word RAM is only available to one CPU at a time, and if an integral piece of code suddenly becomes unavailable due to the buffer ownership changing, the system will crash.

Word RAM also has two modes. In one mode, all 2Mbits is "owned" by one of the CPUs and is inaccessible to the other. One of the main aspects of syncing the Main and Sub CPU is reconciling ownership of Word RAM in this mode. In the other mode, Word RAM is split into 1Mbit banks, of which each is owned by one CPU. The ownership can be readily switched, making this mode useful for streaming data (such as video) which is loaded from disc via the Sub CPU into one bank while the Main CPU copies the data from the other bank to VRAM, and then switching banks to continue the process.

As Word RAM is highly transitory, there is less need to do any serious memory mapping. A module running from this space will still need "ROM" and "RAM" areas partitioned, but this is limited in scope to only that module and will disappear when it is unloaded anyway.
