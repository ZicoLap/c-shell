#ifndef SPL_VMM_H
#define SPL_VMM_H

#include <stdio.h>

#define TOTAL_FRAMES 64
#define FRAME_BYTES 256
#define VMM_PAGE_BYTES 256
#define PAGE_TABLE_ENTRIES 256
#define TLB_ENTRIES 16

 typedef struct
{
    unsigned int pageNumber;
    unsigned int frameNumber;
} TLBEntry;  

typedef struct
{
    int tlb_hits;              // Counts the number of tlb hits
    int pagetable_hits;        // Counts the number of pagetable hits (A tlb hit counts as pagetable hit)
    int total_memory_accesses; // Counts the total number of addresses simulated
} Statistics;

/**
 * Simulates a series of virtual memory accesses using a TLB and a page table.
 *
 * @param fd_addresses The file supplying the stream of virtual accesses.
 *                     Contains one decimal address per line.
 * @param fd_backing The file containing the "true" state of the virtual adress space.
 *                   This file is used to read contents of pages not currently in memory.
 * @return Statistics Some statistics about the simulation
 */
Statistics simulate_virtual_memory_accesses(FILE *fd_addresses, FILE *fd_backing);

#endif
