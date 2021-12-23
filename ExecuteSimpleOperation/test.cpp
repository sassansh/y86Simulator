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

TEST_CASE("test::halt", "[weight=1][part=test]")
{
    uint8_t programMapActual[1] = {0x00};
    machine_state_t stateActual = {programMapActual, 1, 0, {}, 0};
    uint8_t programMapExpected[1] = {0x00};
    machine_state_t stateExpected = {programMapExpected, 1, 0, {}, 0};
    y86_instruction_t instr = {I_HALT, 0, R_RAX, R_RAX, 0, 0, 0};
    REQUIRE(executeSimpleOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::nop", "[weight=1][part=test]")
{
    uint8_t programMapActual[1] = {0x10};
    machine_state_t stateActual = {programMapActual, 1, 0, {}, 0};
    uint8_t programMapExpected[1] = {0x10};
    machine_state_t stateExpected = {programMapExpected, 1, 1, {}, 0};
    y86_instruction_t instr = {I_NOP, 0, R_RAX, R_RAX, 0, 0, 1};
    REQUIRE(executeSimpleOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::irmovq_0", "[weight=1][part=test]")
{
    uint8_t programMapActual[10] = {0x30, 0xF2, 0x08, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 10, 0, {0, 1, 2}, 0};
    uint8_t programMapExpected[10] = {0x30, 0xF2, 0x08, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 10, 10, {0, 1, 8}, 0};
    y86_instruction_t instr = {I_IRMOVQ, 0, R_NONE, R_RDX, 8, 0, 10};
    REQUIRE(executeSimpleOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::irmovq_1", "[weight=1][part=test]")
{
    uint8_t programMapActual[10] = {0x30, 0xF2, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
    machine_state_t stateActual = {programMapActual, 10, 0, {0, 1, 2}, 0};
    uint8_t programMapExpected[10] = {0x30, 0xF2, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
    machine_state_t stateExpected = {programMapExpected, 10, 10, {0, 1, 0x0102030405060708}, 0};
    y86_instruction_t instr = {I_IRMOVQ, 0, R_NONE, R_RDX, 0x0102030405060708, 0, 10};
    REQUIRE(executeSimpleOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::rmmovq", "[weight=1][part=test]")
{
    uint8_t programMapActual[10] = {0x40, 0x12, 0, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 10, 0, {0, 8, 0}, 0};
    uint8_t programMapExpected[10] = {0x08, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 10, 10, {0, 8, 0}, 0};
    y86_instruction_t instr = {I_RMMOVQ, 0, R_RCX, R_RDX, 0, 0, 10};
    REQUIRE(executeSimpleOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::mrmovq", "[weight=1][part=test]")
{
    uint8_t programMapActual[10] = {0x50, 0x12, 0, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateActual = {programMapActual, 10, 0, {0, 7, 0}, 0};
    uint8_t programMapExpected[10] = {0x50, 0x12, 0, 0, 0, 0, 0, 0, 0, 0};
    machine_state_t stateExpected = {programMapExpected, 10, 10, {0, 0x1250, 0}, 0};
    y86_instruction_t instr = {I_MRMOVQ, 0, R_RCX, R_RDX, 0, 0, 10};
    REQUIRE(executeSimpleOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}
