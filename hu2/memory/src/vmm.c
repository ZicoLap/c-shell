#include "vmm.h"
#include "output_utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned char get_byte(unsigned char *memory, unsigned int frameNumber, unsigned int byteOffset)
{
    if (frameNumber >= TOTAL_FRAMES || byteOffset >= FRAME_BYTES)
    {
        fprintf(stderr, "[ERROR] Out of bounds access!\n");
        return 0;
    }
    return memory[frameNumber * FRAME_BYTES + byteOffset];
}

Statistics simulate_virtual_memory_accesses(FILE *addressFile, FILE *backingStoreFile)
{
    Statistics stats;
    memset(&stats, 0, sizeof(Statistics));

    unsigned char *mainMemory = malloc(TOTAL_FRAMES * FRAME_BYTES);
    TLBEntry *tlbCache[TLB_ENTRIES] = {0};
    int pageTable[PAGE_TABLE_ENTRIES];
    memset(pageTable, -1, sizeof(pageTable));

    unsigned int frameIndex = 0, tlbIndex = 0;
    char *addressLine = NULL;
    size_t lineLength = 0;

    while (getline(&addressLine, &lineLength, addressFile) != -1)
    {
        int virtualAddress = atoi(addressLine);
        unsigned int offset = virtualAddress & 0xff;
        unsigned int pageNumber = (virtualAddress >> 8) & 0xff;
        bool pageHit = false, tlbHit = false;
        int physicalAddress = 0;
        unsigned char dataByte = 0;

        TLBEntry *tlbEntry = NULL;
        for (unsigned int i = 0; i < TLB_ENTRIES; ++i)
        {
            if (tlbCache[i] && tlbCache[i]->pageNumber == pageNumber)
            {
                tlbEntry = tlbCache[i];
                break;
            }
        }

        if (tlbEntry)
        {
            tlbHit = true;
            physicalAddress = (tlbEntry->frameNumber << 8) | offset;
            dataByte = get_byte(mainMemory, tlbEntry->frameNumber, offset);
        }
        else
        {
            int frameNumber = pageTable[pageNumber];
            if (frameNumber != -1)
            {
                pageHit = true;
                physicalAddress = (frameNumber << 8) | offset;
                dataByte = get_byte(mainMemory, frameNumber, offset);
            }
            else
            {
                unsigned char *frameLocation = mainMemory + frameIndex * FRAME_BYTES;

                fseek(backingStoreFile, pageNumber * FRAME_BYTES, SEEK_SET);
                fread(frameLocation, sizeof(unsigned char), FRAME_BYTES, backingStoreFile);

                physicalAddress = (frameIndex << 8) | offset;
                dataByte = get_byte(mainMemory, frameIndex, offset);

                for (unsigned int i = 0; i < PAGE_TABLE_ENTRIES; ++i)
                {
                    if ((unsigned int)pageTable[i] == frameIndex)
                    {
                        pageTable[i] = -1;
                    }
                }
                pageTable[pageNumber] = frameIndex;
                for (unsigned int i = 0; i < TLB_ENTRIES; ++i)
                {
                    if (tlbCache[i] && tlbCache[i]->frameNumber == frameIndex)
                    {
                        free(tlbCache[i]);
                        tlbCache[i] = NULL;
                    }
                }

                if (tlbCache[tlbIndex] == NULL)
                {
                    tlbCache[tlbIndex] = malloc(sizeof(TLBEntry));
                }
                tlbCache[tlbIndex]->pageNumber = pageNumber;
                tlbCache[tlbIndex]->frameNumber = frameIndex;
                tlbIndex = (tlbIndex + 1) % TLB_ENTRIES;
                frameIndex = (frameIndex + 1) % TOTAL_FRAMES;
            }

            if (!tlbHit && pageHit)
            {
                if (tlbCache[tlbIndex] == NULL)
                {
                    tlbCache[tlbIndex] = malloc(sizeof(TLBEntry));
                }
                tlbCache[tlbIndex]->pageNumber = pageNumber;
                tlbCache[tlbIndex]->frameNumber = frameNumber;
                tlbIndex = (tlbIndex + 1) % TLB_ENTRIES;
            }
        }

        if (tlbHit)
        {
            stats.tlb_hits++;
        }
        if (tlbHit || pageHit)
        {
            stats.pagetable_hits++;
        }
        stats.total_memory_accesses++;
        print_access_results(virtualAddress, physicalAddress, dataByte, tlbHit, pageHit);
    }

    free(mainMemory);
    for (unsigned int i = 0; i < TLB_ENTRIES; ++i)
    {
        if (tlbCache[i])
        {
            free(tlbCache[i]);
        }
    }

    return stats;
}