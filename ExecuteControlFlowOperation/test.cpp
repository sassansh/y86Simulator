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

TEST_CASE("test::rrmovq", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x20, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 1, 2}, 0};
    uint8_t programMapExpected[2] = {0x20, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 1, 1}, 0};
    y86_instruction_t instr = {I_RRMVXX, C_NC, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeControlFlowOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::cmovle_0", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x21, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 1, 2}, 0};
    uint8_t programMapExpected[2] = {0x21, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 1, 2}, 0};
    y86_instruction_t instr = {I_RRMVXX, C_LE, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeControlFlowOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::cmovle_1", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x21, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 1, 2}, 0 | CC_ZERO_MASK};
    uint8_t programMapExpected[2] = {0x21, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 1, 1}, 0 | CC_ZERO_MASK};
    y86_instruction_t instr = {I_RRMVXX, C_LE, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeControlFlowOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::jmp", "[weight=1][part=test]")
{
    uint8_t programMapActual[9] = {0x70, 0x01, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 9, 0, {}, 0};
    uint8_t programMapExpected[9] = {0x70, 0x01, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 9, 1, {}, 0};
    y86_instruction_t instr = {I_JXX, C_NC, R_RAX, R_RAX, 1, 0, 9};
    REQUIRE(executeControlFlowOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::jle_0", "[weight=1][part=test]")
{
    uint8_t programMapActual[9] = {0x71, 0x01, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 9, 0, {}, 0};
    uint8_t programMapExpected[9] = {0x71, 0x01, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 9, 9, {}, 0};
    y86_instruction_t instr = {I_JXX, C_LE, R_RAX, R_RAX, 1, 0, 9};
    REQUIRE(executeControlFlowOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::jle_1", "[weight=1][part=test]")
{
    uint8_t programMapActual[9] = {0x71, 0x01, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 9, 0, {}, 0 | CC_ZERO_MASK};
    uint8_t programMapExpected[9] = {0x71, 0x01, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 9, 1, {}, 0 | CC_ZERO_MASK};
    y86_instruction_t instr = {I_JXX, C_LE, R_RAX, R_RAX, 1, 0, 9};
    REQUIRE(executeControlFlowOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}
