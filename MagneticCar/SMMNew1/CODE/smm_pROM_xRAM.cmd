## ###################################################################
##
##     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
##
##     Filename  : smm_pROM_xRAM.cmd
##
##     Project   : SMMNew1
##
##     Processor : 56F8013VFAE
##
##     Compiler  : Metrowerks DSP C Compiler
##
##     Date/Time : 2012-2-6, 8:06
##
##     Abstract  :
##
##     This file is used by the linker. It describes files to be linked,
##     memory ranges, stack size, etc. For detailed description about linker
##     command files see CodeWarrior documentation. This file is generated by default.
##     You can switch off generation by setting the property "Generate linker file = no"
##     in the "Build options" tab of the CPU bean and then modify this file as needed.
##
##     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
##     
##     http      : www.freescale.com
##     mail      : support@freescale.com
##
## ###################################################################

MEMORY {
        # I/O registers area for on-chip peripherals
        .x_Peripherals (RW)   : ORIGIN = 0xF000, LENGTH = 0

        # List of all sections specified in the "Build options" tab
        #Internal vector boot area.
        .p_Interruptsboot  (RWX) : ORIGIN = 0x00000000, LENGTH = 0x0004
        .p_Interrupts  (RWX) : ORIGIN = 0x00000000, LENGTH = 0x0000005C
        .p_Code  (RWX) : ORIGIN = 0x0000005C, LENGTH = 0x00001FA4
        .x_Data  (RW) : ORIGIN = 0x00000001, LENGTH = 0x000007FF


        # p_flash_ROM_data mirrors x_Data, mapping to origin and length
        # the "X" flag in "RX" tells the debugger flash p-memory.
        # the p-memory flash is directed to the address determined by AT
        # in the data_in_p_flash_ROM section definition

        .p_flash_ROM_data  (RX) : ORIGIN = 0x00000001, LENGTH = 0x000007FF

        #Other memory segments
        .p_internal_RAM  (RWX) : ORIGIN = 0x00008000, LENGTH = 0x0800

}

KEEP_SECTION { interrupt_vectorsboot.text }
KEEP_SECTION { interrupt_vectors.text }
KEEP_SECTION { .RAW_PROG_FLASH  }
KEEP_SECTION { .RAW_RAM  }

SECTIONS {

        .interrupt_vectorsboot :
        {
          F_vector_addr = .;
          # interrupt vectors boot area
          * (interrupt_vectorsboot.text)
        } > .p_Interruptsboot

        .interrupt_vectors :
        {
          # interrupt vectors
          * (interrupt_vectors.text)
        } > .p_Interrupts

        .ApplicationCode :
        {

              F_Pcode_start_addr = .;

              # .text sections
              OBJECT (F_EntryPoint, Cpu.c) # The function _EntryPoint have to be placed at the beginning of the code
                                       # section for proper functionality of the serial bootloader.
              * (.text)
              * (rtlib.text)
              * (startup.text)
              * (fp_engine.text)
              * (ll_engine.text)
              * (user.text)
              * (.data.pmem)

              # save address where the WIR routine starts in pROM
              . = ALIGN(2);
              __pROM_WIR_start = .;
              * (.RAW_PROG_FLASH)
              # save address where the WIR routine ends in pROM
              __pROM_WIR_end = .;
              # calculate size of WIR routine
              __WIR_size = __pROM_WIR_end - __pROM_WIR_start;
              F_Pcode_end_addr = .;

              # save address where for the data start in pROM
              . = ALIGN(2);
              __pROM_data_start = .;

        } > .p_Code

        # AT sets the download address
        # the download stashes the data just after the program code
        # as determined by our saved pROM data start variable

        .data_in_p_flash_ROM : AT(__pROM_data_start)
        {
              # the data sections flashed to pROM
              # save data start so we can copy data later to xRAM

              __xRAM_data_start = .;

              # .data sections
              * (.const.data.char)  # used if "Emit Separate Char Data Section" enabled
              * (.const.data)

              * (fp_state.data)
              * (rtlib.data)
              * (.data.char)        # used if "Emit Separate Char Data Section" enabled
              * (.data)

              # save data end and calculate data block size
              . = ALIGN(2);
              __xRAM_data_end = .;
              __data_size = __xRAM_data_end - __xRAM_data_start;

        } > .p_flash_ROM_data    # this section is designated as p-memory
                                 # with X flag in the memory map
                                 # the start address and length map to
                                 # actual internal xRAM

        .ApplicationData :
        {
              # save space for the pROM data copy
              . = __xRAM_data_start + __data_size;

              # .bss sections
              * (rtlib.bss.lo)
              * (rtlib.bss)
              . = ALIGN(4);
              F_Xbss_start_addr = .;
              _START_BSS = .;
              * (.bss.char)         # used if "Emit Separate Char Data Section" enabled
              * (.bss)
              . = ALIGN(2);         # used to ensure proper functionality of the zeroBSS hardware loop utility
              _END_BSS   = .;
              F_Xbss_length = _END_BSS - _START_BSS;

              /* Setup the HEAP address */
              . = ALIGN(4);
              _HEAP_ADDR = .;
              _HEAP_SIZE = 0x00000100;
              _HEAP_END = _HEAP_ADDR + _HEAP_SIZE;
              . = _HEAP_END;

              /* SETUP the STACK address */
              _min_stack_size = 0x00000200;
              _stack_addr = _HEAP_END;
              _stack_end  = _stack_addr + _min_stack_size;
              . = _stack_end;

              /* EXPORT HEAP and STACK runtime to libraries */
              F_heap_addr   = _HEAP_ADDR;
              F_heap_end    = _HEAP_END;
              F_Lstack_addr = _HEAP_END;
              F_StackAddr = _HEAP_END;
              F_StackEndAddr = _stack_end - 1;

              # runtime code __init_sections uses these globals:

              F_Ldata_size     = __data_size;
              F_Ldata_RAM_addr = __xRAM_data_start;
              F_Ldata_ROM_addr = __pROM_data_start;

              F_xROM_to_xRAM   = 0x0000;
              F_pROM_to_xRAM   = 0x0001;

              F_start_bss   = _START_BSS;
              F_end_bss     = _END_BSS;

              __DATA_END=.;

        } > .x_Data

        .ApplicationCodeInRAM :
        {
              __PRG_RAM_START=.;
              .=__PRG_RAM_START + __DATA_END;
              __pRAM_WIR_start = .;

              # insert RAM routine (this is for proper move of memory pointer and it is equal to the row above)
              * (.RAW_RAM)

              # You can add your sections here.

        } > .p_internal_RAM

        .WIRData :
        {
              # ****** start WIR ******
              # definition of symbols for copy of WIR routine from pROM to pRAM
              F_LWIR_size     = __WIR_size;
              F_LWIR_ROM_addr = __pROM_WIR_start;
              F_LWIR_ROM_addr_low = __pROM_WIR_start & 0xFFFF;
              __pROM_WIR_start_high =  (__pROM_WIR_start / 0x10000) & 0xFFFF;
              F_LWIR_ROM_addr_high = __pROM_WIR_start_high;
              F_LWIR_RAM_addr = __pRAM_WIR_start;
              F_LWIR_RAM_addr_low = __pRAM_WIR_start & 0xFFFF;
              __pRAM_WIR_start_high =  (__pRAM_WIR_start / 0x10000) & 0xFFFF;
              F_LWIR_RAM_addr_high = __pRAM_WIR_start_high;
              # ****** end WIR ******

        } >> .x_Data

}