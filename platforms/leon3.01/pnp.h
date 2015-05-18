/**
 * @file      pnp.h
 * @author    Rogerio Alves
 *            Bruno Cardoso Lopes
 * 
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      
 * 
 * @brief     LEON3 Linux Plug & Play configuration loader
 *            write roms before booting
 * 
 * @attention Copyright (C) 2002-2013 --- The ArchC Team
 * 
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 * 
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 * 
 * 
 */

#ifndef  LEON_PNP_H
#define  LEON_PNP_H

#include "leon_pnp_defs.h"
#include "memory.h"

#define RAM_ADDR    0x40000000
#define ROM_ADDR    0x00000000
#define ONCHIP_ADDR 0x80000000
#define APBROM_BASEADDR  0x800FF000
#define AHBROM_BASEADDR  0xFFFFF000

//#define DEBUG_ROMS


using generic::memory;

/**
   Direct write with memory
   @param 


**/
void direct_write(memory &mem, uint32_t p, uint32_t addr, uint32_t id) {
     mem.direct_write(addr, p);
#ifdef DEBUG_ROMS
     mem.direct_read(addr, p);/*Test if can read from ROM*/
     printf("0x%.8x --> 0x%.8x\n",(addr+id),p); /*print values*/
#endif
}


#define     WRITE_AMBA_AHB_CONF(name, idx, addr) \
    direct_write(ahb_rom, name[idx].id_reg,addr-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].user[0],(addr+4)-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].user[1],(addr+8)-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].user[2],(addr+12)-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].bar0,(addr+16)-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].bar1,(addr+20)-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].bar2,(addr+24)-AHBROM_BASEADDR,AHBROM_BASEADDR); \
    direct_write(ahb_rom, name[idx].bar3,(addr+28)-AHBROM_BASEADDR,AHBROM_BASEADDR); \

#define     WRITE_AMBA_APB_CONF(name, idx, addr) \
    direct_write(apb_rom, name[idx].id_reg,addr-APBROM_BASEADDR,APBROM_BASEADDR); \
    direct_write(apb_rom, name[idx].bar,(addr+4)-APBROM_BASEADDR,APBROM_BASEADDR); \


/*
 *  AMBA AHB Conf 
 */
typedef struct {
    uint32_t id_reg;
    uint32_t user[3];
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
} amba_ahb_t;

/*
 *  AMBA APB Conf 
 */
typedef struct {
    uint32_t id_reg;
    uint32_t bar;
} amba_apb_t;

void set_pnp(memory &ahb_rom,
             memory &apb_rom)
{
    amba_ahb_t ahb_conf[NUM_DEV];
    amba_apb_t apb_conf[NUM_DEV];

    int amba_ahb_mst_c = 0;
    int amba_ahb_slv_c = 0;
    int amba_apb_c     = 0;
    /*Mapped devices counter*/
    int ahb_dev_count  = 0;
    int apb_dev_count  = 0;
    
    uint32_t rom_start_addr =  0x20000000;//ROM_ADDR;
    uint32_t ram_start_addr = RAM_ADDR;
    uint32_t onchipreg_addr = ONCHIP_ADDR;


    memset(&ahb_conf, 0, NUM_DEV*sizeof(amba_ahb_t));
    memset(&apb_conf, 0, NUM_DEV*sizeof(amba_apb_t));

    printf("Mapping devices\n");

    /*  
     *  LEON PLATAFORM MAP
     *  ------------------
     *
     *  ++ AHB MASTER
     *      VENDOR_GAISLER GAISLER_LEON3   - Leon3 SPARC V8 Processor
     *  ++ AHB SLAVE
     *      VENDOR_GAISLER GAISLER_SRCTRL  - Grlib Sram/Prom Mem Controller 
     *      VENDOR_GAISLER GAISLER_APBMST  - AHB/APB Bridge
     *  ++ APB SLAVE
     *      VENDOR_GAISLER GAISLER_APBUART - APB UART
     *      VENDOR_GAISLER GAISLER_GPTIMER - General purpose timer unit 
     */

    /**
     *  AMBA AHB Masters
     *  ----------------
     */


    // Leon3 SPARC V8 Processor
    ahb_conf[ahb_dev_count].id_reg  = (VENDOR_GAISLER << 24) | (GAISLER_LEON3 << 12);
    ahb_conf[ahb_dev_count].user[0] = ahb_conf[ahb_dev_count].id_reg;
    ahb_conf[ahb_dev_count].user[1] = ahb_conf[ahb_dev_count].id_reg;
    ahb_conf[ahb_dev_count].user[2] = ahb_conf[ahb_dev_count].id_reg;
    printf("AHB Master %d: (0x%.8x) Leon3 SPARC V8 Processor\n",ahb_dev_count,ahb_conf[ahb_dev_count].id_reg );
    WRITE_AMBA_AHB_CONF(ahb_conf, 0, AHB_MST_CFG_ADDR + amba_ahb_mst_c);
    amba_ahb_mst_c += LEON3_AHB_CONF_WORDS*4;
    ahb_dev_count++;
    /**
     *  AMBA AHB Slaves
     *  ---------------
     */

    // Grlib Sram/Prom Mem Controller
    ahb_conf[ahb_dev_count].id_reg  = (VENDOR_GAISLER << 24) | (GAISLER_SDCTRL << 12);
    ahb_conf[ahb_dev_count].user[0] = ahb_conf[ahb_dev_count].id_reg;
    ahb_conf[ahb_dev_count].user[1] = ahb_conf[ahb_dev_count].id_reg;
    ahb_conf[ahb_dev_count].user[2] = ahb_conf[ahb_dev_count].id_reg;

    ahb_conf[ahb_dev_count].bar0   = (0x0) |
                         (0x3e00 << 4) | // C : 1, MASK:0xf
                         AHB_MS;
    ahb_conf[ahb_dev_count].bar1   = (rom_start_addr) |
                         (0xe00 << 4) | // C : 1, MASK:0xf
                         AHB_MS;
    ahb_conf[ahb_dev_count].bar2   = (ram_start_addr) | 
                         (0x3c00 << 4) | // C : 1, MASK:0xf 
                         AHB_MS;
    printf("AHB Slave  %d: (0x%.8x) SRAM/PROM Memory Controller\n",ahb_dev_count,ahb_conf[ahb_dev_count].id_reg );
    WRITE_AMBA_AHB_CONF(ahb_conf, 1, AHB_SLV_CFG_ADDR + amba_ahb_slv_c);
    amba_ahb_slv_c += LEON3_AHB_CONF_WORDS*4;
    ahb_dev_count++;


    // AHB/APB Bridge
    ahb_conf[ahb_dev_count].id_reg = (VENDOR_GAISLER << 24) | (GAISLER_APBMST << 12);
    ahb_conf[ahb_dev_count].user[0] = ahb_conf[ahb_dev_count].id_reg;
    ahb_conf[ahb_dev_count].user[1] = ahb_conf[ahb_dev_count].id_reg;
    ahb_conf[ahb_dev_count].user[2] = ahb_conf[ahb_dev_count].id_reg;

    ahb_conf[ahb_dev_count].bar0   = (onchipreg_addr) | 
                         (0xfff << 4) | // C : 1, MASK:0xff
                         AHB_MS; 
    printf("AHB Slave  %d: (0x%.8x) AHB/APB Bridge\n",ahb_dev_count,ahb_conf[ahb_dev_count].id_reg ); 
    WRITE_AMBA_AHB_CONF(ahb_conf, 2, AHB_SLV_CFG_ADDR + amba_ahb_slv_c);
    amba_ahb_slv_c += LEON3_AHB_CONF_WORDS*4;
    ahb_dev_count++;

    /**
     *  AMBA APB Slaves 
     *  ---------------
     */


    // APB UART
    apb_conf[apb_dev_count].id_reg = (VENDOR_GAISLER << 24) | 
                         (GAISLER_APBUART << 12) |
                         LEON_INTERRUPT_UART_1_RX_TX;
    apb_conf[apb_dev_count].bar    = ((APB_UART_ADDR & 0xfff) << 20) | 
                         (0x0fff << 4) | 
                         APB_IO;
    printf("APB Slave  %d: (0x%.8x) APBUART\n",apb_dev_count,apb_conf[apb_dev_count].id_reg ); 
    WRITE_AMBA_APB_CONF(apb_conf, 0, APB_CFG_ADDR + amba_apb_c);
    amba_apb_c += LEON3_APB_CONF_WORDS*4;
    apb_dev_count++;


    // IRQMP - Interrupt Controller
    apb_conf[apb_dev_count].id_reg = (VENDOR_GAISLER << 24) | 
                         (GAISLER_IRQMP << 12) |
                         0;
    apb_conf[apb_dev_count].bar    = ((APB_IRQMP_ADDR & 0xfff) << 20) | 
                         (0x0fff << 4) | 
                         APB_IO;

    printf("APB Slave  %d: (0x%.8x) IRQMP\n",apb_dev_count,apb_conf[apb_dev_count].id_reg ); 
    WRITE_AMBA_APB_CONF(apb_conf, 1, APB_CFG_ADDR + amba_apb_c);
    amba_apb_c += LEON3_APB_CONF_WORDS*4;
    apb_dev_count++;


    // General purpose timer unit
    apb_conf[apb_dev_count].id_reg = (VENDOR_GAISLER << 24) | 
                         (GAISLER_GPTIMER << 12) |
                         LEON_INTERRUPT_TIMER1;
    apb_conf[apb_dev_count].bar    = ((APB_GPTIMER_ADDR & 0xfff) << 20) | 
                         (0x0fff << 4) | 
                         APB_IO;
    printf("APB Slave  %d: (0x%.8x) GPTIMER\n",apb_dev_count,apb_conf[apb_dev_count].id_reg ); 
    WRITE_AMBA_APB_CONF(apb_conf, 2, APB_CFG_ADDR + amba_apb_c);
    amba_apb_c += LEON3_APB_CONF_WORDS*4;
    apb_dev_count++;
}

#endif  /* LEON_PNP_H */
