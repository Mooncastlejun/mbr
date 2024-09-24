#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mbr.h"

typedef struct P_table {
    uint8_t active;
    uint8_t CHS_Address1[3];
    uint8_t Partition_Type;
    uint8_t CHS_Address2[3];
    uint32_t LBA_Address;
    uint32_t Size;
}PARTITION;

void print_partition(PARTITION* info, uint32_t base_lba) {
    uint32_t abs_lba = base_lba + info->LBA_Address;
    if (info->Partition_Type == 0x07) {
        printf("NTFS %u %u\n", abs_lba, info->Size);
    }
    else if (info->Partition_Type == 0x0B || info->Partition_Type == 0x0C) {
        printf("FAT32 %u %u\n", abs_lba, info->Size);
    }
    else if (info->Partition_Type == 0x05) {
        printf("EBR %u %u \n", abs_lba, info->Size);
    }
}

void process_ebr(FILE* fp, uint32_t base_lba, uint32_t ebr_lba) {
    uint8_t ebr[512];
    fseek(fp, ebr_lba * 512, SEEK_SET);
    if (fread(ebr, 1, 512, fp) != 512) {
        perror("Failed to read EBR");
        return;
    }

    PARTITION* ebr_partitions = (PARTITION*)(ebr + 446);
    print_partition(ebr_partitions, ebr_lba);

    if (ebr_partitions[1].Partition_Type != 0x00) {
        process_ebr(fp, base_lba, base_lba + ebr_partitions[1].LBA_Address);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Example: %s <image file>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    uint8_t mbr[512];
    if (fread(mbr, 1, 512, fp) != 512) {
        perror("Failed to read MBR");
        fclose(fp);
        return 1;
    }

    PARTITION* partitions = (PARTITION*)(mbr + 446);
    for (int i = 0; i < 4; ++i) {
        if (partitions[i].Partition_Type == 0x05 || partitions[i].Partition_Type == 0x0F) {
            process_ebr(fp, partitions[i].LBA_Address, partitions[i].LBA_Address);
        }
        else {
            print_partition(&partitions[i], 0);
        }
    }

    fclose(fp);
    return 0;
}

