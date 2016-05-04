#include <cstdio>
#include <cassert>
#include <vector>
#include <utility>
#include <functional>
#include <cstdlib>
#include <ctime>

#include "myassert.h"
#include "mylog.h"
#include "instructions.h"

struct Instruction
{
        InstructionCode code;
        const char *name;
        const char *description;
        size_t length;
};

static const Instruction INSTRUCTIONS[] = {
#define INST_DEF(b, d, l) {INST_##b, #b, d, l},
    FOR_INSTRUCTIONS(INST_DEF)
};
#undef INST_DEF

const char* instruction_name(InstructionCode code)
{
    if (code >= INST_INVALID && code < INST_LAST)
        return INSTRUCTIONS[code].name;

    assert(false);
    return nullptr;
}

const char* instruction_description(InstructionCode code)
{
    if (code >= INST_INVALID && code < INST_LAST)
        return INSTRUCTIONS[code].description;

    assert(false);
    return nullptr;
}

std::vector<unsigned char> gen_rnd_instrs(const size_t instr_cnt)
{
    std::vector<unsigned char> buffer;
    for(size_t i = 0; i < instr_cnt; ++i)
    {
        const Instruction& instr = INSTRUCTIONS[std::rand() % INST_LAST];
        char instr_buf[instr.length];
        instr_buf[0] = instr.code;
        if (instr.length > 1)
        {
            for(size_t j = 1; j < instr.length; ++j)
                instr_buf[j] = std::rand() % 255;
        }
        for (size_t j = 0; j < instr.length; ++j)
            buffer.push_back(instr_buf[j]);
    }

    return std::move(buffer);
}

void log_instrs(const std::vector<unsigned char> &instrs)
{
    const unsigned char *pos = instrs.data();
    const unsigned char *end = pos + instrs.size();
    while(pos < end)
    {
        const Instruction curInstr = INSTRUCTIONS[(InstructionCode)*pos];
        switch(curInstr.length) {
            case 1:
                LOG(INFO, "%s(%d)", curInstr.name, curInstr.code);
            break;
            case 3:
                LOG(INFO, "%s(%d) 0x%02x%02x", curInstr.name, curInstr.code,
                        pos[2] & 255, pos[1] & 255);
            break;
            case 5:
                LOG(INFO, "%s(%d) 0x%02x%02x%02x%02x", curInstr.name, curInstr.code,
                        pos[4] & 255, pos[3] & 255, pos[2] & 255, pos[1] & 255);
            break;
            case 9:
                LOG(INFO, "%s(%d) 0x%02x%02x%02x%02x%02x%02x%02x%02x", curInstr.name, curInstr.code,
                        pos[8] & 255, pos[7] & 255, pos[6] & 255, pos[5] & 255,
                        pos[4] & 255, pos[3] & 255, pos[2] & 255, pos[1] & 255);
            break;
            default:
                assert(false);
        }
        pos += curInstr.length;
    }
}

void exec_instrs(const std::vector<unsigned char> &instrs)
{
    (void)instrs;
    // TODO this function should interpret instrs stream.
    // instrs stream parsing is similar to the one done in log_instrs
}

int main()
{
    std::srand(std::time(0));

    // Part 1
    bool assertion1_evaluated = false;
    myassert(assertion1_evaluated = true);
    assert((CONFIG_DEBUG && assertion1_evaluated)
            || (!CONFIG_DEBUG && !assertion1_evaluated));

    // Part 2
    LOG(INFO, "==============================");
    LOG(DEBUG, "%d %d %d %s", 1, 2, 3, "debug");
    LOG(INFO, "%d %d %s", 1, 2, "info");
    LOG(WARN, "%d %s", 1, "warning");
    LOG(ERROR, "%s", "error!");

    // Part 3
    LOG(INFO, "==============================");
    LOG(INFO, "%s %s", instruction_name(INST_DADD),
            instruction_description(INST_DADD));
    LOG(INFO, "%s %s", instruction_name(INST_IPRINT),
            instruction_description(INST_IPRINT));

    auto inst_stream = gen_rnd_instrs(20);
    log_instrs(inst_stream);

    // Part 4
    LOG(INFO, "==============================");
    std::vector<unsigned char> super_code = {
        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
        INST_ILOAD, 0, 0, 0xdd, 0, 0, 0, 0, 0,
        INST_IADD,
        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
        INST_IADD,
        INST_IPRINT
    }; // 0xff + 0x0000dd + 0xff = 0x2db
    log_instrs(super_code);
    exec_instrs(super_code);

    return 0;
}
