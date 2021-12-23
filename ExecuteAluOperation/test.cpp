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


TEST_CASE("test::addq_0", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x60, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 1, 2}, 0};
    uint8_t programMapExpected[2] = {0x60, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 1, 3}, 0};
    y86_instruction_t instr = {I_OPQ, A_ADDQ, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeAluOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::addq_1", "[weight=1][part=test]")
{
    uint64_t nOne = (uint64_t)-1;
    uint64_t nTwo = (uint64_t)-2;
    uint64_t nThree = (uint64_t)-3;
    uint8_t programMapActual[2] = {0x60, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, nOne, nTwo}, 0};
    uint8_t programMapExpected[2] = {0x60, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, nOne, nThree}, 0 | CC_SIGN_MASK};
    y86_instruction_t instr = {I_OPQ, A_ADDQ, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeAluOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::addq_2", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x60, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 0, 0}, 0};
    uint8_t programMapExpected[2] = {0x60, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 0, 0}, 0 | CC_ZERO_MASK};
    y86_instruction_t instr = {I_OPQ, A_ADDQ, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeAluOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::subq_0", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x61, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 1, 2}, 0};
    uint8_t programMapExpected[2] = {0x61, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 1, 1}, 0};
    y86_instruction_t instr = {I_OPQ, A_SUBQ, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeAluOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::subq_1", "[weight=1][part=test]")
{
    uint64_t nOne = (uint64_t)-1;
    uint64_t nTwo = (uint64_t)-2;
    uint8_t programMapActual[2] = {0x61, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, nOne, nTwo}, 0};
    uint8_t programMapExpected[2] = {0x61, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, nOne, nOne}, 0 | CC_SIGN_MASK};
    y86_instruction_t instr = {I_OPQ, A_SUBQ, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeAluOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}

TEST_CASE("test::subq_2", "[weight=1][part=test]")
{
    uint8_t programMapActual[2] = {0x61, 0x12};
    machine_state_t stateActual = {programMapActual, 2, 0, {0, 0, 0}, 0};
    uint8_t programMapExpected[2] = {0x61, 0x12};
    machine_state_t stateExpected = {programMapExpected, 2, 2, {0, 0, 0}, 0 | CC_ZERO_MASK};
    y86_instruction_t instr = {I_OPQ, A_SUBQ, R_RCX, R_RDX, 0, 0, 2};
    REQUIRE(executeAluOperation(&stateActual, &instr) == 1);
    equals(stateActual, stateExpected);
}
