#ifndef _LEON_PNP_DEFS_H
#define _LEON_PNP_DEFS_H

#define LEON3_IO_AREA 0xfff00000
#define LEON3_CONF_AREA 0xff000
#define LEON3_AHB_SLAVE_CONF_AREA (1 << 11)

#define LEON3_AHB_CONF_WORDS 8
#define LEON3_APB_CONF_WORDS 2
#define LEON3_AHB_MASTERS 8
#define LEON3_AHB_SLAVES 8
#define LEON3_APB_SLAVES 16
#define LEON3_APBUARTS 8

/* Vendor codes */
#define VENDOR_GAISLER   1
#define VENDOR_PENDER    2
#define VENDOR_ESA       4
#define VENDOR_OPENCORES 8

#define ESA_MCTRL        0xF

/* Gaisler Research device id's */
#define GAISLER_LEON3    0x003
#define GAISLER_LEON3DSU 0x004
#define GAISLER_ETHAHB   0x005
#define GAISLER_APBMST   0x006
#define GAISLER_AHBUART  0x007
#define GAISLER_SRCTRL   0x008
#define GAISLER_SDCTRL   0x009
#define GAISLER_APBUART  0x00C
#define GAISLER_IRQMP    0x00D
#define GAISLER_AHBRAM   0x00E
#define GAISLER_GPTIMER  0x011
#define GAISLER_PCITRG   0x012
#define GAISLER_PCISBRG  0x013
#define GAISLER_PCIFBRG  0x014
#define GAISLER_PCITRACE 0x015
#define GAISLER_PCIDMA   0x016
#define GAISLER_AHBTRACE 0x017
#define GAISLER_ETHDSU   0x018
#define GAISLER_PIOPORT  0x01A
#define GAISLER_AHBJTAG  0x01c
#define GAISLER_ATACTRL  0x024
#define GAISLER_VGA      0x061
#define GAISLER_KBD      0X060
#define GAISLER_ETHMAC   0x01D

#define GAISLER_L2TIME   0xffd  /* internal device: leon2 timer */
#define GAISLER_L2C      0xffe  /* internal device: leon2compat */
#define GAISLER_PLUGPLAY 0xfff  /* internal device: plug & play configarea */

#define LEON_INTERRUPT_CORRECTABLE_MEMORY_ERROR 1
#define LEON_INTERRUPT_UART_1_RX_TX     2
#define LEON_INTERRUPT_UART_0_RX_TX     3
#define LEON_INTERRUPT_EXTERNAL_0       4
#define LEON_INTERRUPT_EXTERNAL_1       5
#define LEON_INTERRUPT_EXTERNAL_2       6
#define LEON_INTERRUPT_EXTERNAL_3       7
#define LEON_INTERRUPT_TIMER1           8
#define LEON_INTERRUPT_TIMER2           9
#define LEON_INTERRUPT_EMPTY1           10
#define LEON_INTERRUPT_EMPTY2           11
#define LEON_INTERRUPT_OPEN_ETH         12
#define LEON_INTERRUPT_EMPTY4           13
#define LEON_INTERRUPT_EMPTY5           14
#define LEON_INTERRUPT_EMPTY6           15

/* memory mapped leon control registers */

#define LEON_PREGS  0x80000000
#define LEON_MCFG1  0x00
#define LEON_MCFG2  0x04
#define LEON_ECTRL  0x08
#define LEON_FADDR  0x0c
#define LEON_MSTAT  0x10
#define LEON_CCTRL  0x14
#define LEON_PWDOWN 0x18
#define LEON_WPROT1 0x1C
#define LEON_WPROT2 0x20
#define LEON_LCONF  0x24

/* TIMER */
#define LEON_TCNT0  0x40
#define LEON_TRLD0  0x44
#define LEON_TCTRL0 0x48
#define LEON_TCNT1  0x50
#define LEON_TRLD1  0x54
#define LEON_TCTRL1 0x58
#define LEON_SCNT   0x60
#define LEON_SRLD   0x64

/* UART */
#define LEON_UART0  0x70
#define LEON_UDATA0 0x70
#define LEON_USTAT0 0x74
#define LEON_UCTRL0 0x78
#define LEON_USCAL0 0x7c
#define LEON_UART1  0x80
#define LEON_UDATA1 0x80
#define LEON_USTAT1 0x84
#define LEON_UCTRL1 0x88
#define LEON_USCAL1 0x8c


#define LEON_IMASK      0x90
#define LEON_IPEND      0x94
#define LEON_IFORCE     0x98
#define LEON_ICLEAR     0x9c
#define LEON_IOREG      0xA0
#define LEON_IODIR      0xA4
#define LEON_IOICONF    0xA8
#define LEON_IPEND2     0xB0
#define LEON_IMASK2     0xB4
#define LEON_ISTAT2     0xB8
#define LEON_ICLEAR2    0xBC

#define     APB_IO      0x1     /* APB I/O */
#define     AHB_MS      0x2     /* AHB Memory space */
#define     AHB_IO      0x3     /* AHB I/O space */

#define     AHB_MST_CFG_ADDR    0xfffff000
#define     AHB_SLV_CFG_ADDR    0xfffff800
#define     APB_CFG_ADDR        0x800ff000

#define     APB_UART_ADDR       0x001
#define     APB_IRQMP_ADDR      0x002
#define     APB_GPTIMER_ADDR    0x003

#define     NUM_DEV             10

#endif /* _LEON_PNP_DEFS_H */
