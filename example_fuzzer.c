#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbr.h"

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 최소한 MBR의 크기보다 작은 입력은 처리할 수 없습니다.
    if (size < 512) return 0;

    // 입력 데이터를 FILE*로 변환
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;

    // MBR 데이터 읽기
    uint8_t mbr[512];
    if (fread(mbr, 1, 512, in_file) != 512) {
        fclose(in_file);
        return 0;
    }

    // 파티션 테이블 확인
    PARTITION* partitions = (PARTITION*)(mbr + 446);
    for (int i = 0; i < 4; ++i) {
        if (partitions[i].Partition_Type == 0x05 || partitions[i].Partition_Type == 0x0F) {
            // 확장 파티션인 경우 재귀적으로 처리
            process_ebr(in_file, partitions[i].LBA_Address, partitions[i].LBA_Address);
        } else {
            // 기본 파티션인 경우 정보 출력
            print_partition(&partitions[i], 0);
        }
    }

    // 파일 핸들 닫기
    fclose(in_file);

    // 정상 실행 종료
    return 0;
}
