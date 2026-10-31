/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : linker_script.ld
* Description : Linker Script File
*******************************************************************************/

MEMORY {
    ROM               (rx)  : ORIGIN = 0x20010000, LENGTH = 0x00FF0000
    QSPI_FLASH_RES    (r)   : ORIGIN = 0x21000000, LENGTH = 0x03000000
    MMU_PAGE_TABLE    (rw)  : ORIGIN = 0x80000000, LENGTH = 0x00004000
    CACHED_RAM        (rwx) : ORIGIN = 0x80004000, LENGTH = 0x0036C800
    UNCACHED_RAM      (rwx) : ORIGIN = 0x82370800, LENGTH = 0x00081800
    STACK             (rw)  : ORIGIN = 0x803F2000, LENGTH = 0x0000C800
    HARDWARE_INIT_RAM (rwx) : ORIGIN = 0x803FE800, LENGTH = 0x00001800
}

/*
NOTE: each file for different sections are referenced with a filename starting with an '*'.
This is used that the linker finds the files even when relative paths are used inside the static library (e.g. libBSP.a)

When using ar (GCC archiver) with the option '-P' relative path names are added into the library.
*/
SECTIONS
{
    LOAD_MODULE1 : {
        KEEP(* (VECTOR_TABLE))      /*asm*/
    } > ROM

    __vector_table_base = ADDR(LOAD_MODULE1);


    .data.memclk_setup : ALIGN( 0x20 ) {
        "*r_spibsc_setup.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_memclk_setup.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_*_memclk_setup.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_spibsc_setup.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_memclk_setup.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_*_memclk_setup.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        . = ALIGN( 0x4 );
    } > HARDWARE_INIT_RAM AT > ROM

    __data_memclk_setup_load_start  = LOADADDR(.data.memclk_setup);
    __data_memclk_setup_exec_start  = ADDR(.data.memclk_setup);
    __data_memclk_setup_load_end    = LOADADDR(.data.memclk_setup) + SIZEOF(.data.memclk_setup);

    .bss.memclk_setup (NOLOAD) : ALIGN( 0x20 ) {
        "*r_spibsc_setup.o" (.bss .bss.* COMMON)
        "*r_memclk_setup.o" (.bss .bss.* COMMON)
        "*r_*_memclk_setup.o" (.bss .bss.* COMMON)
        "*r_spibsc_setup.c.obj" (.bss .bss.* COMMON)
        "*r_memclk_setup.c.obj" (.bss .bss.* COMMON)
        "*r_*_memclk_setup.c.obj" (.bss .bss.* COMMON)
        . = ALIGN( 0x4 );
    } > HARDWARE_INIT_RAM

    __bss_memclk_setup_start  = ADDR(.bss.memclk_setup);
    __bss_memclk_setup_end    = ADDR(.bss.memclk_setup) + SIZEOF(.bss.memclk_setup);

    LOAD_MODULE2 : ALIGN( 0x200 ) {
        /* This section used by "hardware setup" and must to be assigned to the Internal RAM. */
        "*r_cpg_*.o"      (.text .text.* .rodata .rodata.* .data .data.*)
        "*rza_io_regrw.o" (.text .text.* .rodata .rodata.*)
        "*rza_io_regrw.o" (.data .data.*)
        "*hwsetup*.o"     (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_cpg_*.c.obj"      (.text .text.* .rodata .rodata.* .data .data.*)
        "*rza_io_regrw.c.obj" (.text .text.* .rodata .rodata.*)
        "*rza_io_regrw.c.obj" (.data .data.*)
        "*hwsetup*.c.obj"     (.text .text.* .rodata .rodata.* .data .data.*)

        . = ALIGN( 0x4 );
    } > HARDWARE_INIT_RAM AT > ROM

    __module2_load_start  = LOADADDR(LOAD_MODULE2);
    __module2_exec_start  = ADDR(LOAD_MODULE2);
    __module2_load_end    = LOADADDR(LOAD_MODULE2) + SIZEOF(LOAD_MODULE2);


    LOAD_MODULE3 : ALIGN( 0x20 ) {
        /* This section used by "hardware setup" and must to be assigned to the Internal RAM. */
        __module3_exec_start = .;
        "*r_cpg*.o"       (.bss* COMMON*)
        "*rza_io_regrw.o" (.bss .bss.* COMMON)
        "*r_cpg*.c.obj"       (.bss* COMMON*)
        "*rza_io_regrw.c.obj" (.bss .bss.* COMMON)
        . = ALIGN( 0x4 );
        __module3_exec_end = .;
    } > HARDWARE_INIT_RAM AT > ROM


    LOAD_MODULE4 : ALIGN( 0x4 ) {
        KEEP("*reset_handler.o" (RESET_HANDLER))     /*asm*/
        KEEP("*initsect.o" (INIT_SECTION))            /*asm*/
        KEEP("*sections.o"       (.rodata .rodata.*))
        KEEP("*resetprg.o"       (INIT_TBL))
        KEEP("*reset_handler.asm.obj" (RESET_HANDLER))     /*asm*/
        KEEP("*initsect.asm.obj" (INIT_SECTION))            /*asm*/
        KEEP("*sections.c.obj"       (.rodata .rodata.*))
        KEEP("*resetprg.c.obj"       (INIT_TBL))

        "*cpu.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*initsect.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*sections.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*irq.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*irqfiq_handler.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*mmu_operation.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*reset_handler.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*vector_table.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*vfp_init.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*fini.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*init.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*resetprg.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_compiler_abstraction.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_gpio_drv_api.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_gpio_lld_rza2m.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_mmu_lld_rza2m.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_stb_lld_rza2m.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_cbuffer.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_ostm_drv_api.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_ostm_lld_rza2m.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_scifa_drv_api.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_scifa_hld_prv.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_rza2_scifa_lld.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_rza2_scifa_lld_baud.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*hwsetup.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_devlink_wrapper.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_os_abstraction_api.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_os_abstraction_stdio.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*heap_renesas.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*hwsetup_userdef.o" (.text .text.* .rodata .rodata.* .data .data.*)

        "*cpu.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*initsect.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*sections.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*irq.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*irqfiq_handler.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*mmu_operation.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*reset_handler.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*vector_table.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*vfp_init.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*fini.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*init.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*resetprg.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_compiler_abstraction.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_gpio_drv_api.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_gpio_lld_rza2m.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_mmu_lld_rza2m.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_stb_lld_rza2m.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_cbuffer.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_ostm_drv_api.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_ostm_lld_rza2m.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_scifa_drv_api.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_scifa_hld_prv.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_rza2_scifa_lld.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_rza2_scifa_lld_baud.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*hwsetup.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_devlink_wrapper.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_os_abstraction_api.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_os_abstraction_stdio.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*heap_renesas.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*hwsetup_userdef.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)
    } > ROM

    /*
        The vector mirror table and the irq handler must include in the .data section.
        Because the addresses on ROM are not calculated if the two put in each sections.
    */
    .data : ALIGN( 0x20 ) {
        __vect_exec_start = .;
        __vect_load_start = LOADADDR(.data) + ( . - ADDR(.data) );

        __vector_mirror_table_base = .;
        KEEP(* (VECTOR_MIRROR_TABLE))   /*asm*/

        . = ALIGN( 0x4 );
        __vect_load_end =  LOADADDR(.data) + ( . - ADDR(.data) );

        /********************************************************************/

        . = ALIGN(0x00000100);
        __fastcode_exec_start = .;
        __fastcode_load_start = LOADADDR(.data) + ( . - ADDR(.data) );

        "*/r_intc_*.o"  (.text .text.* .rodata .rodata.*)
        "*/r_intc_*.c.obj"  (.text .text.* .rodata .rodata.*)

        KEEP(* (IRQ_FIQ_HANDLER))       /*asm*/


        . = ALIGN( 0x4 );
        __fastcode_load_end = LOADADDR(.data) + ( . - ADDR(.data) );

        /********************************************************************/

        __data_exec_start = .;
        __data_load_start = LOADADDR(.data) + ( . - ADDR(.data) );


        . = ALIGN( 0x4 );
        __data_load_end = LOADADDR(.data) + ( . - ADDR(.data) );

    } > CACHED_RAM AT > ROM

    .bss (NOLOAD) : ALIGN( 0x20 ) {
        __bss_start = .;

        "*.o" (.bss .bss.* COMMON)
        "*.asm.obj" (.bss .bss.* COMMON)
        "*.c.obj" (.bss .bss.* COMMON)

        . = ALIGN( 0x4 );
        __bss_end = .;

    } > CACHED_RAM

    VRAM (NOLOAD) : ALIGN( 0x40 ) {
        * (.VRAM_SECTION0)
    } > CACHED_RAM

    .uncached_RAM (NOLOAD) : ALIGN( 0x20 ) {
        __uncached_RAM_start = .;

        "*r_cache_*.o" (.bss .bss.* COMMON)
        "*r_cache_*.asm.obj" (.bss .bss.* COMMON)
        "*r_cache_*.c.obj" (.bss .bss.* COMMON)

        * (UNCACHED_BSS)

        . = ALIGN( 0x4 );
        __uncached_RAM_end = .;
    } > UNCACHED_RAM

    .uncached_RAM2 : ALIGN( 0x20 ) {
        __uncached_RAM2_exec_start = .;
        __uncached_RAM2_load_start = LOADADDR(.uncached_RAM2) + ( . - ADDR(.uncached_RAM2) );

        "*r_cache_*.o" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_cache_*.asm.obj" (.text .text.* .rodata .rodata.* .data .data.*)
        "*r_cache_*.c.obj" (.text .text.* .rodata .rodata.* .data .data.*)

        . = ALIGN( 0x4 );
        __uncached_RAM2_load_end = LOADADDR(.uncached_RAM2) + ( . - ADDR(.uncached_RAM2) );
    } > UNCACHED_RAM AT > ROM

        .uncached_RAM_USB_SMPL_0 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_USB_SMPL_0_start = .;

                * (USB_SMPL_0)
                "*r_usb_vendor_hapl_p0.o" (.bss .bss.* USB_SMPL_0)
                "*r_usb_vendor_hapl_p0.c.obj" (.bss .bss.* USB_SMPL_0)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_SMPL_0_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_USB_SMPL_1 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_USB_SMPL_1_start = .;

                * (USB_SMPL_1)
                "*r_usb_vendor_hapl_p1.o" (.bss .bss.* USB_SMPL_1)
                "*r_usb_vendor_hapl_p1.c.obj" (.bss .bss.* USB_SMPL_1)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_SMPL_1_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_USB_CLASS_0 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_USB_CLASS_0_start = .;

                * (USB_CLASS_0)
                "*r_usbh0_hmsc_driver.o" (.bss .bss.* USB_CLASS_0)
                "*r_usbh0_hmsc_driver.c.obj" (.bss .bss.* USB_CLASS_0)
                "*r_usbh0_hstorage_driver.o" (.bss .bss.* USB_CLASS_0)
                "*r_usbh0_hstorage_driver.c.obj" (.bss .bss.* USB_CLASS_0)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_CLASS_0_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_USB_CLASS_1 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_USB_CLASS_1_start = .;

                * (USB_CLASS_1)
                "*r_usbh0_hmsc_driver.o" (.bss .bss.* USB_CLASS_1)
                "*r_usbh0_hmsc_driver.c.obj" (.bss .bss.* USB_CLASS_1)
                "*r_usbh0_hstorage_driver.o" (.bss .bss.* USB_CLASS_1)
                "*r_usbh0_hstorage_driver.c.obj" (.bss .bss.* USB_CLASS_1)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_CLASS_1_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_OHCI_ED2_0 (NOLOAD) : ALIGN( 0x10 ) {
                __uncached_RAM_OHCI_ED2_0_start = .;

                * (OHCI_ED2_0)
                "*r_usbh0_hohci_memory.o" (.bss .bss.* OHCI_ED2_0)
                "*r_usbh0_hohci_memory.c.obj" (.bss .bss.* OHCI_ED2_0)

                . = ALIGN( 0x10 );
                __uncached_RAM_OHCI_ED2_0_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_OHCI_ED2_1 (NOLOAD) : ALIGN( 0x10 ) {
                __uncached_RAM_OHCI_ED2_1_start = .;

                * (OHCI_ED2_1)
                "*r_usbh1_hohci_memory.o" (.bss .bss.* OHCI_ED2_1)
                "*r_usbh1_hohci_memory.c.obj" (.bss .bss.* OHCI_ED2_1)

                . = ALIGN( 0x10 );
                __uncached_RAM_OHCI_ED2_1_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_USB_HOST_0 : ALIGN( 0x4 ) {
                __uncached_RAM_USB_HOST_0_start = .;

                * (USB_HOST_0)
                "*r_usbh0_hmanager.o" (.data .data.* USB_HOST_0)
                "*r_usbh0_hmanager.c.obj" (.data .data.* USB_HOST_0)
                "*r_usbh0_hhubsys.o" (.data .data.* USB_HOST_0)
                "*r_usbh0_hhubsys.c.obj" (.data .data.* USB_HOST_0)
                "*r_usbh0_hhci.o" (.data .data.* USB_HOST_0)
                "*r_usbh0_hhci.c.obj" (.data .data.* USB_HOST_0)
                "*r_usbh0_hohci_main.o" (.data .data.* USB_HOST_0)
                "*r_usbh0_hohci_main.c.obj" (.data .data.* USB_HOST_0)
                "*r_usbh0_hehci_memory.o" (.data .data.* USB_HOST_0)
                "*r_usbh0_hehci_memory.c.obj" (.data .data.* USB_HOST_0)
                "*r_usbh0_hohci_memory.o" (.data .data.* USB_HOST_0)
                "*r_usbh0_hohci_memory.c.obj" (.data .data.* USB_HOST_0)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_HOST_0_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_USB_HOST_INIT_0 : ALIGN( 0x4 ) {
                __uncached_RAM_USB_HOST_INIT_0_exec_start  = .;
                __uncached_RAM_USB_HOST_INIT_0_load_start = LOADADDR(.uncached_RAM_USB_HOST_INIT_0) + ( . - ADDR(.uncached_RAM_USB_HOST_INIT_0) );

                * (USB_HOST_INIT_1)
                "*usbh0_hmanager.o" (.data .data.*USB_HOST_INIT_0)
                "*usbh0_hmanager.c.obj" (.data .data.*USB_HOST_INIT_0)
                "*usbh0_hohci_memory*.o" (.data .data.*USB_HOST_INIT_0)
                "*usbh0_hohci_memory*.c.obj" (.data .data.*USB_HOST_INIT_0)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_HOST_INIT_0_load_end = LOADADDR(.uncached_RAM_USB_HOST_INIT_0) + ( . - ADDR(.uncached_RAM_USB_HOST_INIT_0) );
        } > UNCACHED_RAM AT > ROM

        .uncached_RAM_USB_HOST_1 : ALIGN( 0x4 ) {
                __uncached_RAM_USB_HOST_1_start = .;

                * (USB_HOST_1)
                "*r_usbh1_hmanager.o" (.data .data.* USB_HOST_1)
                "*r_usbh1_hmanager.c.obj" (.data .data.* USB_HOST_1)
                "*r_usbh1_hhubsys.o" (.data .data.* USB_HOST_1)
                "*r_usbh1_hhubsys.c.obj" (.data .data.* USB_HOST_1)
                "*r_usbh1_hhci.o" (.data .data.* USB_HOST_1)
                "*r_usbh1_hhci.c.obj" (.data .data.* USB_HOST_1)
                "*r_usbh1_hohci_main.o" (.data .data.* USB_HOST_1)
                "*r_usbh1_hohci_main.c.obj" (.data .data.* USB_HOST_1)
                "*r_usbh1_hehci_memory.o" (.data .data.* USB_HOST_1)
                "*r_usbh1_hehci_memory.c.obj" (.data .data.* USB_HOST_1)
                "*r_usbh1_hohci_memory.o" (.data .data.* USB_HOST_1)
                "*r_usbh1_hohci_memory.c.obj" (.data .data.* USB_HOST_1)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_HOST_1_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_USB_HOST_INIT_1 : ALIGN( 0x4 ) {
                __uncached_RAM_USB_HOST_INIT_1_exec_start  = .;
                __uncached_RAM_USB_HOST_INIT_1_load_start = LOADADDR(.uncached_RAM_USB_HOST_INIT_1) + ( . - ADDR(.uncached_RAM_USB_HOST_INIT_1) );

                * (USB_HOST_INIT_1)
                "*usbh1_hmanager.o" (.data .data.*USB_HOST_INIT_1)
                "*usbh1_hmanager.c.obj" (.data .data.*USB_HOST_INIT_1)
                "*usbh1_hohci_memory*.o" (.data .data.*USB_HOST_INIT_1)
                "*usbh1_hohci_memory*.c.obj" (.data .data.*USB_HOST_INIT_1)

                . = ALIGN( 0x4 );
                __uncached_RAM_USB_HOST_INIT_1_load_end = LOADADDR(.uncached_RAM_USB_HOST_INIT_1) + ( . - ADDR(.uncached_RAM_USB_HOST_INIT_1) );
        } > UNCACHED_RAM AT > ROM

        /* 
        PERIODICLISTBASE Register must be aligned on 4 Kbyte boundaries.
        Refer to RZ/A2M User's Manual 32.2.4.3(6)
        */
        .uncached_RAM_EHCI_PFL_0 (NOLOAD) : ALIGN( 0x1000 ) {
                __uncached_RAM_EHCI_PFL_0_start = .;

                * (EHCI_PFL_0)
                "*r_usbh0_hehci_memory.o" (.bss .bss.* EHCI_PFL_0)
                "*r_usbh0_hehci_memory.c.obj" (.bss .bss.* EHCI_PFL_0)

                . = ALIGN( 0x1000 );
                __uncached_RAM_EHCI_PFL_0_end = .;
        } > UNCACHED_RAM

        /* 
        PERIODICLISTBASE Register must be aligned on 4 Kbyte boundaries.
        Refer to RZ/A2M User's Manual 32.2.4.3(6)
        */
        .uncached_RAM_EHCI_PFL_1 (NOLOAD) : ALIGN( 0x1000 ) {
                __uncached_RAM_EHCI_PFL_1_start = .;

                * (EHCI_PFL_1)
                "*r_usbh1_hehci_memory.o" (.bss .bss.* EHCI_PFL_1)
                "*r_usbh1_hehci_memory.c.obj" (.bss .bss.* EHCI_PFL_1)

                . = ALIGN( 0x1000 );
                __uncached_RAM_EHCI_PFL_1_end = .;
        } > UNCACHED_RAM

        /* 
        ASYNCLISTADDR Register must be aligned on 32 byte boundaries.
        Refer to RZ/A2M User's Manual 32.2.4.3(7)
        */
        .uncached_RAM_EHCI_QH_0 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_QH_0_start = .;

                * (EHCI_QH_0)
                "*r_usbh0_hehci_memory.o" (.bss .bss.* EHCI_QH_0)
                "*r_usbh0_hehci_memory.c.obj" (.bss .bss.* EHCI_QH_0)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_QH_0_end = .;
        } > UNCACHED_RAM

        /* 
        ASYNCLISTADDR Register must be aligned on 32 byte boundaries. 
        Refer to RZ/A2M User's Manual 32.2.4.3(7)
        */
        .uncached_RAM_EHCI_QH_1 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_QH_1_start = .;

                * (EHCI_QH_1)
                "*r_usbh1_hehci_memory.o" (.bss .bss.* EHCI_QH_1)
                "*r_usbh1_hehci_memory.c.obj" (.bss .bss.* EHCI_QH_1)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_QH_1_end = .;
        } > UNCACHED_RAM

        /* 
        qTD must be aligned on 32 byte boundaries.
        Refer to EHCI Spec.3.5.1      
        */
        .uncached_RAM_EHCI_QTD_0 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_QTD_0_start = .;

                * (EHCI_QTD_0)
                "*r_usbh0_hehci_memory.o" (.bss .bss.* EHCI_QTD_0)
                "*r_usbh0_hehci_memory.c.obj" (.bss .bss.* EHCI_QTD_0)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_QTD_0_end = .;
        } > UNCACHED_RAM

        /* 
        qTD must be aligned on 32 byte boundaries.
        Refer to EHCI Spec.3.5.1      
        */
        .uncached_RAM_EHCI_QTD_1 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_QTD_1_start = .;

                * (EHCI_QTD_1)
                "*r_usbh1_hehci_memory.o" (.bss .bss.* EHCI_QTD_1)
                "*r_usbh1_hehci_memory.c.obj" (.bss .bss.* EHCI_QTD_1)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_QTD_1_end = .;
        } > UNCACHED_RAM

        /* 
        Isochronous TDs must be aligned on 32 byte boundaries. 
        Refer to EHCI Spec.3.3.1
        */
        .uncached_RAM_EHCI_ITD_0 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_ITD_0_start = .;

                * (EHCI_ITD_0)
                "*r_usbh0_hehci_memory.o" (.bss .bss.* EHCI_ITD_0)
                "*r_usbh0_hehci_memory.c.obj" (.bss .bss.* EHCI_ITD_0)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_ITD_0_end = .;
        } > UNCACHED_RAM

        /* 
        Isochronous TDs must be aligned on 32 byte boundaries. 
        Refer to EHCI Spec.3.3.1
        */
        .uncached_RAM_EHCI_ITD_1 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_ITD_1_start = .;

                * (EHCI_ITD_1)
                "*r_usbh1_hehci_memory.o" (.bss .bss.* EHCI_ITD_1)
                "*r_usbh1_hehci_memory.c.obj" (.bss .bss.* EHCI_ITD_1)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_ITD_1_end = .;
        } > UNCACHED_RAM

        /* 
        Split Transaction Isochronous Transfer Descriptor must be aligned on 32 byte boundaries. 
        Refer to EHCI Spec.3.4.1
        */
        .uncached_RAM_EHCI_SITD_0 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_SITD_0_start = .;

                * (EHCI_SITD_0)
                "*r_usbh0_hehci_memory.o" (.bss .bss.* EHCI_SITD_0)
                "*r_usbh0_hehci_memory.c.obj" (.bss .bss.* EHCI_SITD_0)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_SITD_0_end = .;
        } > UNCACHED_RAM

        /* 
        Split Transaction Isochronous Transfer Descriptor must be aligned on 32 byte boundaries. 
        Refer to EHCI Spec.3.4.1
        */
        .uncached_RAM_EHCI_SITD_1 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_EHCI_SITD_1_start = .;

                * (EHCI_SITD_1)
                "*r_usbh1_hehci_memory.o" (.bss .bss.* EHCI_SITD_1)
                "*r_usbh1_hehci_memory.c.obj" (.bss .bss.* EHCI_SITD_1)

                . = ALIGN( 0x20 );
                __uncached_RAM_EHCI_SITD_1_end = .;
        } > UNCACHED_RAM

        /* 
        HcHCCA Register must be aligned on 256 byte boundaries.
        Refer to RZ/A2M User's Manual 32.2.4(7)
        */
        .uncached_RAM_OHCI_HCCA_0 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_OHCI_HCCA_0_start = .;

                * (OHCI_HCCA_0)
                "*r_usbh0_hohci_memory.o" (.bss .bss.* OHCI_HCCA_0)
                "*r_usbh0_hohci_memory.c.obj" (.bss .bss.* OHCI_HCCA_0)

                . = ALIGN( 0x20 );
                __uncached_RAM_OHCI_HCCA_0_end = .;
        } > UNCACHED_RAM

        /* 
        HcHCCA Register must be aligned on 256 byte boundaries.
        Refer to RZ/A2M User's Manual 32.2.4(7)
        */
        .uncached_RAM_OHCI_HCCA_1 (NOLOAD) : ALIGN( 0x20 ) {
                __uncached_RAM_OHCI_HCCA_1_start = .;

                * (OHCI_HCCA_1)
                "*r_usbh1_hohci_memory.o" (.bss .bss.* OHCI_HCCA_1)
                "*r_usbh1_hohci_memory.c.obj" (.bss .bss.* OHCI_HCCA_1)

                . = ALIGN( 0x20 );
                __uncached_RAM_OHCI_HCCA_1_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_OHCI_TD_0 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_OHCI_TD_0_start = .;

                * (OHCI_TD_0)
                "*r_usbh0_hohci_memory.o" (.bss .bss.* OHCI_TD_0)
                "*r_usbh0_hohci_memory.c.obj" (.bss .bss.* OHCI_TD_0)

                . = ALIGN( 0x4 );
                __uncached_RAM_OHCI_TD_0_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_OHCI_TD_1 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_OHCI_TD_1_start = .;

                * (OHCI_TD_1)
                "*r_usbh1_hohci_memory.o" (.bss .bss.* OHCI_TD_1)
                "*r_usbh1_hohci_memory.c.obj" (.bss .bss.* OHCI_TD_1)

                . = ALIGN( 0x4 );
                __uncached_RAM_OHCI_TD_1_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_OHCI_ED_0 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_OHCI_ED_0_start = .;

                * (OHCI_ED_0)
                "*r_usbh0_hohci_memory.o" (.bss .bss.* OHCI_ED_0)
                "*r_usbh0_hohci_memory.c.obj" (.bss .bss.* OHCI_ED_0)

                . = ALIGN( 0x4 );
                __uncached_RAM_OHCI_ED_0_end = .;
        } > UNCACHED_RAM

        .uncached_RAM_OHCI_ED_1 (NOLOAD) : ALIGN( 0x4 ) {
                __uncached_RAM_OHCI_ED_1_start = .;

                * (OHCI_ED_1)
                "*r_usbh1_hohci_memory.o" (.bss .bss.* OHCI_ED_1)
                "*r_usbh1_hohci_memory.c.obj" (.bss .bss.* OHCI_ED_1)

                . = ALIGN( 0x4 );
                __uncached_RAM_OHCI_ED_1_end = .;
        } > UNCACHED_RAM

    .mmu_page_table (NOLOAD) : ALIGN( 0x4000 ) {
        __mmu_page_table_base = .;
        . += 0x00004000;
        __mmu_page_table_end  = .;
    } > MMU_PAGE_TABLE


    .stack (NOLOAD) : ALIGN( 0x10 ) {
        __sys_stack_start = .;
        . += 0x00008000;
        . = ALIGN( 0x10 );
        __sys_stack_end = .;

        __irq_stack_start = .;
        . += 0x00002000;
        . = ALIGN( 0x10 );
        __irq_stack_end = .;

        __fiq_stack_start = .;
        . += 0x00000400;
        . = ALIGN( 0x10 );
        __fiq_stack_end = .;

        __svc_stack_start = .;
        . += 0x00002000;
        . = ALIGN( 0x10 );
        __svc_stack_end = .;

    } > STACK

    .text2 : ALIGN( 0x4 ) {
        * (.text .text.*)
        * (.rodata .rodata.*)

        . = ALIGN( 0x4 );

        __preinit_array_start = .;
        KEEP(* (.preinit_array*))
        __preinit_array_end = .;
        __init_array_start = .;
        KEEP(* (.init_array*))
        __init_array_end = .;
        __fini_array_start = .;
        KEEP(* (.fini_array*))
        __fini_array_end = .;
    } > ROM

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > ROM

    .data2 : ALIGN( 0x20 ) {
        __data2_exec_start = .;
        __data2_load_start = LOADADDR(.data2) + ( . - ADDR(.data2) );

        * (.data .data.*)

        . = ALIGN( 0x4 );
        __data2_load_end = LOADADDR(.data2) + ( . - ADDR(.data2) );
    } > CACHED_RAM AT > ROM

    .bss2 (NOLOAD) : ALIGN( 0x20 ) {
        __bss2_start = .;

        * (.bss .bss.*)
        * (COMMON)

        . = ALIGN( 0x4 );
        __bss2_end = .;
    } > CACHED_RAM

    .heap (NOLOAD) : ALIGN( 0x10 ) {
        end = .;
        __heap_start = .;
        . = . + 0x000D4000;
        __heap_end = .;
        /* heap end */

    } > CACHED_RAM

    .freertos_heap (NOLOAD) : ALIGN( 0x10 ) {
        PROVIDE(__freertos_heap_start = .);
        *(.freertos_heap)
        PROVIDE(__freertos_heap_end = .);
        /* heap end */

    } > CACHED_RAM

    .RES_FLASH (NOLOAD) :
    {
        __guilianiResources = .;
    } > QSPI_FLASH_RES
}
/*EOF*/
