#ifndef _HELPERS_H
#define _HELPERS_H

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <fstream>

template <class M>
void reset_memory(M* m) {
    for (unsigned i = 0; i < 1024; ++i) {
        m->mem[i] = 0;
    }
}

template <class M>
void init_memory(M* m, std::vector<uint32_t> data, unsigned offset) {    
    unsigned i = offset >> 2; // divide by 4(=32bit)
    for (auto it = data.cbegin(); it != data.cend(); ++it, ++i) {
        m->mem[i] = *it;
    }
}

template <class M>
void reset_rf(M* m) {
    for (int i = 0; i < 32; ++i) {
        if (i == 2) m->r[i] = 0x2ffc;
        else m->r[i] = 0;
    }
}

template <class M>
void init_rf(M* m, std::map<int, uint32_t> data) {
    for (const auto& [key, val] : data) {
        m->r[key] = val;
    }
}

std::vector<uint32_t> load_hex_dump(std::string path) {
    std::vector<uint32_t> data;
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line)) {
        if (line.size() == 0 || line == "\n") break;
        data.push_back((uint32_t)std::stoul(line, nullptr, 16));
    }
    return data;
}

template <class M>
void load_instr_to_memory(M* m, std::string path) {
    auto dump = load_hex_dump(path);
    init_memory<M>(m, dump, 0);
}


#endif
