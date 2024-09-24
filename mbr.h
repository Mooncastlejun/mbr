#ifndef MBR_H
#define MBR_H

#include <stdint.h>
#include <stdio.h>

// 파티션 테이블 구조체 정의
typedef struct P_table {
    uint8_t active;
    uint8_t CHS_Address1[3];
    uint8_t Partition_Type;
    uint8_t CHS_Address2[3];
    uint32_t LBA_Address;
    uint32_t Size;
} PARTITION;

// 파티션 정보를 출력하는 함수 선언
void print_partition(PARTITION* info, uint32_t base_lba);

// EBR(Extended Boot Record)를 처리하는 함수 선언
void process_ebr(FILE* fp, uint32_t base_lba, uint32_t ebr_lba);
// 함수 정의
const char* partition_type_to_string(uint8_t type);
double partition_size_in_gb(uint32_t size_in_sectors);
void print_boot_code(const uint8_t* mbr);
void check_active_partition(PARTITION* partition);
void list_ebr_chain(FILE* fp, uint32_t base_lba, uint32_t ebr_lba);
#endif // MBR_H
