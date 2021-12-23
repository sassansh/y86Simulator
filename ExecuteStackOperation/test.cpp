#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <stdint.h>
#include <stdio.h>

extern "C"
{
#include "instruction.h"
}

#define CC_ZERO_MASK 0x1
#define CC_SIGN_MASK 0x2
#define CC_CARRY_MASK 0x4
#define CC_OVERFLOW_MASK 0x8

void equals(machine_state_t actual, machine_state_t expected)
{
    REQUIRE(actual.programSize == expected.programSize);
    REQUIRE(actual.programCounter == expected.programCounter);
    REQUIRE(actual.conditionCodes == expected.conditionCodes);

    for (int i = 0; i < actual.programSize; i++)
    {
        REQUIRE(actual.programMap[i] == expected.programMap[i]);
    }
    for (int i = 0; i < 16; i++)
    {
        REQUIRE(actual.registerFile[i] == expected.registerFile[i]);
    }
}

TEST_CASE("test::call", "[weight=1][part=test]")
{
    uint8_t programMapActual[16] = {0x80, 0x8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 16, 0, {}, 0};
    uint8_t programMapExpected[16] = {0x80, 0x8, 0, 0, 0, 0, 0, 0, 0x9, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 16, 8, {}, 0};
    stateActual.registerFile[R_RSP] = 0x10;
    stateExpected.registerFile[R_RSP] = 0x08;
    y86_instruction_t instr = {I_CALL, 0, R_RAX, R_RAX, 8, 0, 9};
    REQUIRE(executeStackOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::ret", "[weight=1][part=test]")
{
    uint8_t programMapActual[16] = {0x90, 0, 0, 0, 0, 0, 0, 0, 0x07, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 16, 0, {}, 0};
    uint8_t programMapExpected[16] = {0x90, 0, 0, 0, 0, 0, 0, 0, 0x07, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 16, 7, {}, 0};
    stateActual.registerFile[R_RSP] = 0x08;
    stateExpected.registerFile[R_RSP] = 0x10;
    y86_instruction_t instr = {I_RET, 0, R_RAX, R_RAX, 0, 0, 1};
    REQUIRE(executeStackOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::pushq", "[weight=1][part=test]")
{
    uint8_t programMapActual[16] = {0xA0, 0x1F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 16, 0, {0, 6}, 0};
    uint8_t programMapExpected[16] = {0xA0, 0x1F, 0, 0, 0, 0, 0, 0, 0x06, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 16, 2, {0, 6}, 0};
    stateActual.registerFile[R_RSP] = 0x10;
    stateExpected.registerFile[R_RSP] = 0x08;
    y86_instruction_t instr = {I_PUSHQ, 0, R_RCX, R_NONE, 0, 0, 2};
    REQUIRE(executeStackOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::popq", "[weight=1][part=test]")
{
    uint8_t programMapActual[16] = {0xA0, 0x1F, 0, 0, 0, 0, 0, 0, 0x09, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 16, 0, {0, 4}, 0};
    uint8_t programMapExpected[16] = {0xA0, 0x1F, 0, 0, 0, 0, 0, 0, 0x09, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 16, 2, {0, 9}, 0};
    stateActual.registerFile[R_RSP] = 0x08;
    stateExpected.registerFile[R_RSP] = 0x10;
    y86_instruction_t instr = {I_POPQ, 0, R_RCX, R_NONE, 0, 0, 2};
    REQUIRE(executeStackOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

