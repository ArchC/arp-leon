#ifndef _TIMER_DEFS_H
#define _TIMER_DEFS_H

#define NUM_TIMERS 4 /*LEON3 has 4 timers*/

/*Timer Configuration Register Initial values(See.: Leon Manual p.77)*/
#define GPTIMER_CONFIG_REG_PATTERN  0x342
/*Control Register Initial Values*/
#define GPTIMER_CONTROL_REG_PATTERN 0x3 

/*Base IRQ*/
#define BASE_IRQ 0x8

#define IRQMP

#ifdef IRQMP
 #define IRQ_SEND_ADDR 0x80000204
#endif

/*Control Register bit masks*/
#define GPTIMER_ENABLE_MASK      (1 << 0)
#define GPTIMER_RESTART_MASK     (1 << 1)
#define GPTIMER_LOAD_MASK        (1 << 2)
#define GPTIMER_INT_ENABLE_MASK  (1 << 3)
#define GPTIMER_INT_PENDING_MASK (1 << 4)
#define GPTIMER_CHAIN_MASK       (1 << 5) /* Not supported */
#define GPTIMER_DEBUG_HALT_MASK  (1 << 6) /* Not supported */

#define GPTIMER_BASEADDR 0x80000300
#define TIMER_BASE 0x10

/*Decrement Unit*/
typedef struct{
   int prescalar_value;
   unsigned int prescalar_reload;
}LEON3_prescalar_regs;


/*Timer Unit*/
typedef struct{
   int counter; //timer_value;
   unsigned int reload; //timer_reload;
   unsigned int control;
}LEON3_timers;




#endif /* _TIMER_DEFS_H */
