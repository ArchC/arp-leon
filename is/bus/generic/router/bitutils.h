#ifndef _BITUTILS_H
#define _BITUTILS_H

inline void endian_swap32(uint32_t& addr)
 {
    addr = (addr >>24) | 
           ((addr <<8) & 0x00FF0000) |
           ((addr >>8) & 0x0000FF00) |
           (addr <<24);
 }

inline void endian_swap32(int32_t& addr)
 {
    addr = (addr >>24) | 
           ((addr <<8) & 0x00FF0000) |
           ((addr >>8) & 0x0000FF00) |
           (addr <<24);
 }

 inline void endian_swap8(unsigned short& addr)
 {
    addr = (addr >>8) | 
           (addr <<8);
 }

#endif /*bitutils.h*/
