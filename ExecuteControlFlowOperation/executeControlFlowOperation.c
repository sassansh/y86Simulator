#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "instruction.h"

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

/* Stores the specified quad-word (64-bit) value into memory, at the
   specified start address, using little-endian format. Returns 1 in
   case of success, or 0 in case of failure (e.g., if the address is
   beyond the limit of the memory size). */
int memWriteQuadLE(machine_state_t *state, uint64_t address, uint64_t value)
{
    if (address > state->programSize || address + 8 > state->programSize)
    {
        return 0;
    }
    else
    {
        for (size_t i = 0; i < 8; i++)
        {
            state->programMap[address + i] = (uint8_t)((value >> 8 * i) & 0xFF);
        }
        return 1;
    }
}

int checkConditionCode(machine_state_t *state, uint8_t ifun)
{

    switch (ifun)
    {
    case C_NC:
        return 1;
    case C_LE:
        return (state->conditionCodes == (0 | CC_ZERO_MASK) || state->conditionCodes == (0 | CC_SIGN_MASK));
    case C_L:
        return (state->conditionCodes == (0 | CC_SIGN_MASK));
    case C_E:
        return (state->conditionCodes == (0 | CC_ZERO_MASK));

    case C_NE:
        return (state->conditionCodes != (0 | CC_ZERO_MASK));

    case C_GE:
        return (state->conditionCodes != (0 | CC_SIGN_MASK));

    case C_G:
        return (state->conditionCodes != (0 | CC_ZERO_MASK) && state->conditionCodes != (0 | CC_SIGN_MASK));
    default:
        return 1;
    }
}

/* Executes the instruction specified by *instr, modifying the
   machine's state (memory, registers, condition codes, program
   counter) in the process. Returns 1 if the instruction was executed
   successfully, or 0 if there was an error. Typical errors include an
   invalid instruction or a memory access to an invalid address. */
int executeControlFlowOperation(machine_state_t *state, y86_instruction_t *instr)
{
    int success = 1;

    switch (instr->icode)
    {
    case I_RRMVXX:
        state->programCounter = instr->valP;
        if (checkConditionCode(state, instr->ifun))
        {
            state->registerFile[instr->rB] = state->registerFile[instr->rA];
        }

        break;
    case I_JXX:

        if (checkConditionCode(state, instr->ifun))
        {
            state->programCounter = instr->valC;
        }
        else
        {
            state->programCounter = instr->valP;
        }

        break;
    default:
        return 0;
    }
    return success;
}