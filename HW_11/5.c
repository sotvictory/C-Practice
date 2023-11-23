#include <stdio.h>
#include <stdlib.h>

typedef struct Seg {
    unsigned size;
    unsigned addr;
} segment;

#define SEG_ERR 25

/* Виртуальный адрес: номер сегмента (SegNum) и смещение в сегменте (offset)
 * Организация памяти 8-сегментная (2^3) => под номер сегмента отводится 3 первых бита в VirtAddr
 * Тогда под смещение отводится 32 - 3 = 29 оставшихся бит в VirtAddr
 * Номер строки таблицы сегментов = номер сегмента
 * Строка таблицы сегментов: размер сегмента (size) и физический адрес начала сегмента (addr)
 * Физический адрес = сумма адреса начала сегмента (addr) + смещение (offset) */

unsigned int VirtIntoPhys(segment *SegTable, unsigned VirtAddr)
{
    unsigned SegNum = VirtAddr >> 29;
    unsigned offset = VirtAddr & 0x1fffffff;

    if (SegTable[SegNum].size > offset) {
        return SegTable[SegNum].addr + offset;
    } else {
        exit(SEG_ERR);
    }
}