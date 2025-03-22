#ifndef _DEFS_H
#define _DEFS_H

#include <cstdint>

namespace ALUCtl {
    constexpr uint8_t AND                     = 0b0000;
    constexpr uint8_t OR                      = 0b0001;
    constexpr uint8_t XOR                     = 0b0010;
    constexpr uint8_t ADD                     = 0b0011;
    constexpr uint8_t SUB                     = 0b0100;
    constexpr uint8_t SHIFT_LEFT              = 0b0101;
    constexpr uint8_t SHIFT_RIGHT_LOGICAL     = 0b0110;
    constexpr uint8_t SHIFT_RIGHT_ARITHMETIC  = 0b0111;
    constexpr uint8_t LESS_THAN               = 0b1000;
    constexpr uint8_t GREATER_EQUAL           = 0b1001;
    constexpr uint8_t LESS_THAN_U             = 0b1010;
    constexpr uint8_t GREATER_EQUAL_U         = 0b1011;
    constexpr uint8_t EQUAL                   = 0b1100;
    constexpr uint8_t NOT_EQUAL               = 0b1101;
}

namespace ALUOp {
    constexpr uint8_t R_TYPE = 0b00;
    constexpr uint8_t I_TYPE = 0b01;
    constexpr uint8_t ADD    = 0b10;
    constexpr uint8_t B      = 0b11;
}

#endif
