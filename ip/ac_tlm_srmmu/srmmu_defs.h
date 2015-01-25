#ifndef SRMMU_DEFS_H
#define SRMMU_DEFS_H

#define LEON3

#ifdef LEON3
/*Leon3 ASI Map*/
#define ASI_FCMISS              0x01
#define ASI_SYSR                0x02   
#define ASI_UINST               0x08   
#define ASI_SINST               0x09   
#define ASI_UDATA               0x0A   
#define ASI_SDATA               0x0B   
#define ASI_ITAG                0x0C   
#define ASI_IDATA               0x0D   
#define ASI_DTAG                0x0E   
#define ASI_DDATA               0x0F   
#define ASI_IFLUSH              0x10   
#define ASI_DFLUSH              0x11   
#define ASI_FLUSH_PAGE          0x10   
#define ASI_FLUSH_CTX           0x13   
#define ASI_DCTX                0x14   
#define ASI_ICTX                0x15   
#define ASI_MMUFLUSHPROBE       0x18   
#define ASI_MMUREGS             0x19   
#define ASI_MMU_BP              0x1C  
#define ASI_MMU_DIAG            0x1D   
#define ASI_MMU_DSU             0x1F   

#endif /*LEON3 */

/* Fault Status Register fields */
/* access type */
#define SRMMU_AT_SHIFT     5
#define SRMMU_AT_MASK      0x07
#define SRMMU_AT_LUD       0      /* load user data */
#define SRMMU_AT_LSD       1      /* load supervisor data */
#define SRMMU_AT_LUI       2      /* load/exec user instruction */
#define SRMMU_AT_LSI       3      /* load/exec supervisor instruction */
#define SRMMU_AT_SUD       4      /* store user data */
#define SRMMU_AT_SSD       5      /* store supervisor data */
#define SRMMU_AT_SUI       6      /* store user instruction */
#define SRMMU_AT_SSI       7      /* store supervisor instruction */
/* fault type */
#define SRMMU_FT_SHIFT     2
#define SRMMU_FT_MASK      0x07

/*Fault Status Priority Check*/
#define    FSR_FT_NONE     0
#define    FSR_FT_IAR      1
#define    FSR_FT_PE       2
#define    FSR_FT_PVR      3
#define    FSR_FT_TE       4
#define    FSR_FT_ABE      5
#define    FSR_FT_IE       6
#define    FSR_FT_R        7

/*Page Access Types*/
#define PAGE_READ  0x0
#define PAGE_WRITE 0x1

/*SRMMU Registers*/
#define MMU_CTRL_REG           0x0
#define MMU_CTXTBL_PTR         0x100
#define MMU_CTX_REG            0x200
#define MMU_FAULT_STATUS       0x300
#define MMU_FAULT_ADDR         0x400
#define MMU_NUM_REGS           5

#define ET_INVALID  0
#define ET_PTD      1
#define ET_PTE      2
#define ET_RESERVED 3

/*Page Map Size*/
#define L1_MAP_SIZE_MASK 0x00FFFFFF /*16MB*/
#define L2_MAP_SIZE_MASK 0x0003FFFF /*256KB*/
#define L3_MAP_SIZE_MASK 0x00000FFF /*4KB*/

/*PTD Masks*/
#define PTD_L0_MASK 0xFF000000
#define PTD_L1_MASK 0x00FC0000
#define PTD_L2_MASK 0x0003F000

#define CTP_MASK 0xFFFFFFFC

#define FSR_FT_MAX_PRIO(n,o)    ((fsr_ft_prio[n] > fsr_ft_prio[o]) ? (n) : (o))

#define PAGE_ENTRY_SIZE sizeof(table_entry) /*size of each entry on table*/

#define PAGE_READ  0x0
#define PAGE_WRITE 0x1

#define     VFP_PAGE        0   /* Level-3 PTE  */
#define     VFP_SEGMENT     1   /* Level-2 & 3 PTE/PTDs */
#define     VFP_REGION      2   /* Level-1,2 & 3 PTE/PTDs */
#define     VFP_CONTEXT     3   /* Level-0,1,2 & 3 PTE/PTDs */
#define     VFP_ENTIRE      4   /* All PTEs/PTDs */


/* Cache Control register fields */

#define CACHE_CTRL_IF (1 <<  4)  /* Instruction Cache Freeze on Interrupt */
#define CACHE_CTRL_DF (1 <<  5)  /* Data Cache Freeze on Interrupt */
#define CACHE_CTRL_DP (1 << 14)  /* Data cache flush pending */
#define CACHE_CTRL_IP (1 << 15)  /* Instruction cache flush pending */
#define CACHE_CTRL_IB (1 << 16)  /* Instruction burst fetch */
#define CACHE_CTRL_FI (1 << 21)  /* Flush Instruction cache (Write only) */
#define CACHE_CTRL_FD (1 << 22)  /* Flush Data cache (Write only) */
#define CACHE_CTRL_DS (1 << 23)  /* Data cache snoop enable */

#define CPU_FEATURE_CACHE_CTRL   (1 << 16)
/*Control Register*/
typedef struct {
    uint32_t   IMPL:4;      /*   [28:31]  */
    uint32_t   VER:4;       /*   [24:27]  */
    uint32_t   SC:16;       /*   [8:23]   */
    uint32_t   PSO:1;       /*   [7]      */
    uint32_t   reserved:5;  /*   [2:6]    */
    uint32_t   NF:1;        /*   [1]      */
    uint32_t   E:1;         /*   [0]      */
} control_reg;

/*Context Table Pointer Register*/
typedef struct {
    uint32_t   CTP:30;     /*  [2:31] */
    uint32_t   reserved:2; /*  [0:1]  */
} ctx_table_pointer_reg;

typedef struct {
    uint32_t CTXN;           /* [0:31] */
} ctxn_reg;

// Fault Status Register
typedef struct {
    uint32_t reserved:14;   /*   [18:31]  */
    uint32_t EBE:2;         /*   [10:17]  */
    uint32_t L:2;           /*   [8:9]   */
    uint32_t AT:2;          /*   [5:7]    */
    uint32_t FT:2;          /*   [2:4]    */
    uint32_t FAV:1;         /*   [1]      */
    uint32_t OW:1;          /*   [0]      */
} fault_status_reg;

// Fault Address Register
typedef struct {
    uint32_t address;    /* [0:3] */
} fault_addr_reg;


typedef struct {
    uint32_t   L1:8;     /* [24:31] */
    uint32_t   L2:6;     /* [18:23] */
    uint32_t   L3:6;     /* [12:17] */
    uint32_t   offset:12;  /* [0:11]  */
} virtual_addr;

typedef struct {
    uint32_t ET:2;     /*  [0:1]   */
    uint32_t ACC:3;    /*  [2:4]   */
    uint32_t R:1;      /*  [5]     */
    uint32_t M:1;      /*  [6]     */
    uint32_t C:1;      /*  [7]     */
    uint32_t PPN:24;   /*  [8:31]  */
} pte_entry;


typedef struct {
    uint32_t ET:2;      /* [0:1]  */
    uint32_t PTP:30;    /* [2:31] */ 
} ptd_entry;


typedef union {
    ptd_entry  PTD;
    pte_entry  PTE;
} table_entry;

typedef struct {
    uint32_t reserved:8;    /*   [0:7]   */
    uint32_t type:4;        /*   [8:11]   */
    uint32_t VFPA:20;       /*   [12:31]   */
} flush_probe_entry;

#endif /*SRMMU_DEFS_H*/
