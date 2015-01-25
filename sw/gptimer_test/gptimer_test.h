#ifndef _GPTIMER_TEST_H
#define _GPTIMER_TEST_H

#define GPTIMER_BASEADDR 0x80000300
/*Note: Leon3 GPTIMER offset 0x0 0x4 0x8 0x10*/
/*dummy just work adjust the offset it's a simple pointer arithmetics*/
struct timerreg {
    unsigned int counter; /* 0x0 */
    unsigned int reload; /* 0x4 */
    unsigned int control; /* 0x8 */
    unsigned int dummy_1; /* 0xC */
};

struct gptimer {
    unsigned int scalercnt; /* 0x0 */
    unsigned int scalerload; /* 0x4 */
    unsigned int configreg; /* 0x8 */
    unsigned int dummy_2; /* 0xC */
    struct timerreg timer[3]; /* Using 4 timers*/
};
#endif /* _GPTIMER_TEST_H */
