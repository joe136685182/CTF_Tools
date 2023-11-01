#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

void crc32_gen_array(uint32_t crc, int pos, uint8_t *buf, int len)
{
    assert (pos >= 0);
    assert (buf != 0);
    assert (pos + 4 <= len);

    uint32_t rem = 0xFFFFFFFF;
    for (int i = 0; i < pos; ++i)
    {
        rem ^= buf[i];
        for (int j = 0; j < 8; ++j)
            rem = (rem >> 1) ^ (rem & 0x00000001 ? 0xEDB88320 : 0);
    }

    for (int i = 0; i < 4; ++i)
        buf[pos + i] = (rem >> (8 * i)) & 0xFF;

    rem = ~crc;
    for (int i = len - 1; i >= pos; --i)
    {
        for (int j = 0; j < 8; ++j)
            rem = (rem << 1) ^ (rem & 0x80000000 ? 0xDB710641 : 0);
        rem ^= buf[i];
    }

    for (int i = 0; i < 4; ++i)
        buf[pos + i] = (rem >> (8 * i)) & 0xFF;
}


int main(int argc, char* argv[])
{
    const uint8_t length = 20;  //  Length of needed string
    unsigned char buf[length+1];
    memset(buf, (uint32_t)'A', length);  // Init buf array
    uint32_t crc_v = 0;
    if (argc == 2)  // Cmdline mode
        crc_v = strtoul(argv[1], NULL, 16);
    else  // Debug mode
        crc_v = 0x12345678;
//    printf("0x%08X\n", crc_v);
    crc32_gen_array (crc_v, 0, buf, length);

    printf("0x");
    for (int i = 0; i < length; i++)
        printf("%02x", buf[i]);
    printf("\n");
    return 0;
}
