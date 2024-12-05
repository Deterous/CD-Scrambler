#include <array>
#include <fstream>
#include <iostream>

constexpr size_t SECTOR_SIZE = 2352;

static constexpr auto ECMA_130 = []() {
    uint16_t sr = 1;
    std::array<uint8_t, SECTOR_SIZE> arr{};
    for (uint16_t i = 12; i < SECTOR_SIZE; ++i) {
        arr[i] = static_cast<uint8_t>(sr);
        for (uint8_t b = 0; b < 8; ++b) {
            uint16_t c = sr & 1 ^ sr >> 1 & 1;
            sr = (c << 15 | sr) >> 1;
        }
    }
    return arr;
}();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "ECMA-130 Scrambler/Descrambler (Deterous, 2024)"<< std::endl;
        std::cout << "Usage: scramble <file_path> [scrambler_offset = 0]" << std::endl;
        return 1;
    }

    const char* path = argv[1];
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cout << "Error: Could not open file " << path << std::endl;
        return 1;
    }

    int offset = 0;
    if (argc >= 3) {
        offset = std::atoi(argv[2]);
        if (offset < 0 || offset >= SECTOR_SIZE) {
            std::cerr << "Error: Invalid scrambler offset. It must be between 0 and " << 2352 - 1 << std::endl;
            return 1;
        }
    }

    char sector[SECTOR_SIZE];
    std::streampos pos = 0;
    while (!file.eof()) {
        file.seekg(pos);
        file.read(sector, SECTOR_SIZE);
        std::streamsize num_bytes = file.gcount();

        if (num_bytes == 0)
            break;

        while (num_bytes < SECTOR_SIZE && !file.eof()) {
            file.read(sector + num_bytes, SECTOR_SIZE - num_bytes);
            num_bytes += file.gcount();
        }

        for (int i = 0; i < SECTOR_SIZE; ++i)
            sector[i] ^= ECMA_130[offset + i];

        if (file.eof())
            file.clear();

        file.seekp(pos);
        file.write(sector, num_bytes);
        pos += num_bytes;
    }

    file.close();
    std::cout << "Scrambled " << pos << " bytes with an offset of " << offset << std::endl;
    
    return 0;
}
