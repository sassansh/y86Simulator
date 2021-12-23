#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "instruction.h"

/* Reads one byte from memory, at the specified address. Stores the
     read value into *value. Returns 1 in case of success, or 0 in case
     of failure (e.g., if the address is beyond the limit of the memory
     size). */
int memReadByte(machine_state_t *state, uint64_t address, uint8_t *value)
{
    if (address > state->programSize)
    {
        return 0;
    }
    else
    {
        *value = state->programMap[address];
        return 1;
    }
}

/* Reads one quad-word (64-bit number) from memory in little-endian
     format, at the specified starting address. Stores the read value
     into *value. Returns 1 in case of success, or 0 in case of failure
     (e.g., if the address is beyond the limit of the memory size). */
int memReadQuadLE(machine_state_t *state, uint64_t address, uint64_t *value)
{
    if (address > state->programSize || address + 8 > state->programSize)
    {
        return 0;
    }

    else
    {
        uint64_t memRead = 0;
        for (size_t i = 0; i < 8; i++)
        {
            memRead += (uint64_t)(state->programMap[address + i]) << (8 * i);
        }
        *value = memRead;
        return 1;
    }
}

int checkIFunValid(uint8_t ifun)
{
    return ifun <= 6 && ifun >= 0;
}

int getNextByte(uint8_t *first, uint8_t *last, uint64_t addr, machine_state_t *state)
{
    uint8_t data = 0;
    if (memReadByte(state, addr, &data))
    {
        *first = (data >> 4) & 0xF;
        *last = (data & 0xF);
        return 1;
    }
    return 0;
}

int checkInstSize(machine_state_t *state, y86_instruction_t *instr, uint64_t PC, uint64_t instructionSize)
{
    if (PC + instructionSize > state->programSize)
    {
        instr->icode = I_TOO_SHORT;
        return 0;
    }
    return 1;
}

/* Fetches one instruction from memory, at the address specified by
     the program counter. Does not modify the machine's state. The
     resulting instruction is stored in *instr. Returns 1 if the
     instruction is a valid non-halt instruction, or 0 (zero)
     otherwise. */
int fetchInstruction(machine_state_t *state, y86_instruction_t *instr)
{
    uint64_t PC = state->programCounter;
    instr->location = 0;

    uint8_t icode = 0;
    uint8_t ifun = 0;
    uint8_t rA = 0;
    uint8_t rB = 0;
    uint64_t valC = 0;
    uint64_t instructionSize = 0;

    if (getNextByte(&icode, &ifun, PC, state))
    {

        instr->icode = icode;
        instr->ifun = ifun;

        switch (instr->icode)
        {
        case I_HALT:
            instructionSize = 1;
            instr->ifun = 0x0;
            instr->valP = PC + instructionSize;
            return 0;

        case I_NOP:
        case I_RET:
            instructionSize = 1;
            instr->ifun = 0x0;
            instr->valP = PC + instructionSize;
            break;

        case I_OPQ:
        case I_RRMVXX:
            instructionSize = 2;
            if (!checkInstSize(state, instr, PC, instructionSize))
            {
                return 0;
            }
            if (!checkIFunValid(instr->ifun))
            {
                instr->icode = I_INVALID;
                return 0;
            }

            getNextByte(&rA, &rB, PC + 1, state);

            instr->rA = rA;
            instr->rB = rB;
            instr->valP = PC + instructionSize;
            break;

        case I_IRMOVQ:
        case I_RMMOVQ:
        case I_MRMOVQ:
            instructionSize = 10;
            if (!checkInstSize(state, instr, PC, instructionSize))
            {
                return 0;
            }

            getNextByte(&rA, &rB, PC + 1, state);

            instr->rA = rA;
            instr->rB = rB;

            if (instr->icode == I_IRMOVQ && instr->rA != 0xF || instr->ifun != 0)
            {
                instr->icode = I_INVALID;
                return 0;
            }

            memReadQuadLE(state, PC + 2, &valC);

            instr->valC = valC;
            instr->valP = PC + instructionSize;
            break;

        case I_JXX:
        case I_CALL:
            instructionSize = 9;

            if (!checkInstSize(state, instr, PC, instructionSize))
            {
                return 0;
            }

            if ((instr->icode == I_CALL && instr->ifun != 0) || (instr->icode == I_JXX && !checkIFunValid(instr->ifun)))
            {
                instr->icode = I_INVALID;
                return 0;
            }

            memReadQuadLE(state, PC + 1, &valC);

            instr->valC = valC;
            instr->valP = PC + instructionSize;
            break;

        case I_PUSHQ:
        case I_POPQ:
            instructionSize = 2;
            if (!checkInstSize(state, instr, PC, instructionSize))
            {
                return 0;
            }

            getNextByte(&rA, &rB, PC + 1, state);

            instr->rA = rA;
            instr->rB = rB;

            if (instr->rB != 0xF || instr->ifun != 0)
            {
                instr->icode = I_INVALID;
                return 0;
            }

            instr->valP = PC + instructionSize;
            break;
        default:
            instr->icode = I_INVALID;
            return 0;
        }

        return 1;
    }

    instr->icode = I_TOO_SHORT;
    return 0;
}
