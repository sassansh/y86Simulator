#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include <stdint.h>
#include <stdio.h>

extern "C"
{
#include "instruction.h"
}

static y86_instruction_t invokeFetchInstruction(uint8_t *programMap, const uint64_t programSize)
{
    machine_state_t state = {programMap, programSize, 0, {}, 0};
    y86_instruction_t instr = {I_INVALID, 0, R_RAX, R_RAX, 0, 2, 0};
    fetchInstruction(&state, &instr);
    return instr;
}

TEST_CASE("test::halt", "[weight=1][part=test]")
{
    const uint64_t programSize = 1;
    uint8_t programMap[programSize] = {0};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_HALT);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 1);
}

TEST_CASE("test::nop", "[weight=1][part=test]")
{
    const uint64_t programSize = 1;
    uint8_t programMap[programSize] = {0x10};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_NOP);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 1);
}

TEST_CASE("test::rrmovq", "[weight=1][part=test]")
{
    const uint64_t programSize = 2;
    uint8_t programMap[programSize] = {0x20, 0x12};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_RRMVXX);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.rA == 0x1);
    REQUIRE(instr.rB == 0x2);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 2);
}

TEST_CASE("test::cmovle", "[weight=1][part=test]")
{
    const uint64_t programSize = 2;
    uint8_t programMap[programSize] = {0x21, 0x12};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_RRMVXX);
    REQUIRE(instr.ifun == 0x1);
    REQUIRE(instr.rA == 0x1);
    REQUIRE(instr.rB == 0x2);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 2);
}

TEST_CASE("test::irmovq", "[weight=1][part=test]")
{
    const uint64_t programSize = 10;
    uint8_t programMap[programSize] = {0x30, 0xF2, 0x08, 0, 0, 0, 0, 0, 0, 0};
    y86_instruction_t
        instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_IRMOVQ);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.rA == 0xF);
    REQUIRE(instr.rB == 0x2);
    REQUIRE(instr.valC == 0x08);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 10);
}

TEST_CASE("test::addq", "[weight=1][part=test]")
{
    const uint64_t programSize = 2;
    uint8_t programMap[programSize] = {0x60, 0x23};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_OPQ);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.rA == 0x2);
    REQUIRE(instr.rB == 0x3);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 2);
}

TEST_CASE("test::jmp", "[weight=1][part=test]")
{
    const uint64_t programSize = 9;
    uint8_t programMap[programSize] = {0x70, 0x09, 0, 0, 0, 0, 0, 0, 0};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_JXX);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.valC == 0x9);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 9);
}

TEST_CASE("test::call", "[weight=1][part=test]")
{
    const uint64_t programSize = 9;
    uint8_t programMap[programSize] = {0x80, 0x09, 0, 0, 0, 0, 0, 0, 0};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_CALL);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.valC == 0x9);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 9);
}

TEST_CASE("test::pushq", "[weight=1][part=test]")
{
    const uint64_t programSize = 2;
    uint8_t programMap[programSize] = {0xA0, 0x4F};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_PUSHQ);
    REQUIRE(instr.ifun == 0x0);
    REQUIRE(instr.rA == 0x4);
    REQUIRE(instr.rB == 0xF);
    REQUIRE(instr.location == 0);
    REQUIRE(instr.valP == 2);
}

TEST_CASE("test::invalid", "[weight=1][part=test]")
{
    const uint64_t programSize = 1;
    uint8_t programMap[programSize] = {0xCD};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_INVALID);
}

TEST_CASE("test::too_short", "[weight=1][part=test]")
{
    const uint64_t programSize = 1;
    uint8_t programMap[programSize] = {0xB0};
    y86_instruction_t instr = invokeFetchInstruction(programMap, programSize);
    REQUIRE(instr.icode == I_TOO_SHORT);
}