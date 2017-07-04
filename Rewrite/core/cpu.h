#ifndef CPU_H
#define CPU_H

#include "../classList.h"
#include <corePart.h>

#define OP_TYPE_CPU	0x00
#define OP_TYPE_OUT	0x01
#define OP_TYPE_BRK	0x02
#define OP_TYPE_JSR	0x03

#define FLAG_C  0
#define FLAG_Z  1
#define FLAG_I  2
#define FLAG_D  3
#define FLAG_B  4
#define FLAG_S  5
#define FLAG_V  6
#define FLAG_N  7

struct cpu_state{
    Uint8 accumulator;
    Uint8 indexX;
    Uint8 indexY;
    Uint16 programCounter;
    Uint8 stackPointer;
    Uint8 statusRegister;
};

typedef void(cpu::*opcodeHandler)();
typedef Uint16(cpu::*addressHandler)(bool& hasCrossPage);


class cpu : public corePart
{
    public:
        cpu_state state;
        cpu_state newState;

        bool lineNMI;
        bool lineIRQ;

        Uint8 instructionLen[8];
        Uint8 flagMask[8];
        Uint8 branchFlag[4];
        opcodeHandler opHdl[4];
        addressHandler adHdl[9];

        Uint8 nextInstruction[3];
        Uint8 instructionType;
        Uint8 instructionTicks;
        Uint8 outValue;
        Uint16 outAddress;



        memBus* mb;

        cpu();
        virtual ~cpu();

        void reset();
        void saveState(fstream* statefile);
        void loadState(fstream* statefile);
        void init();
        void init2();

        void processInstruction();
        void opcodeHandler0();
        void opcodeHandler1();
        void opcodeHandler2();
        void opcodeHandler3();
        void runInstruction();
        Uint8 getNextInstructionLength();

        Uint8 getValue(Uint8 addressMode, bool& hasCrossPage);
        Uint16 resolveAddress(Uint8 addressMode, bool& hasCrossPage);
        Uint16 resolveAddress0(bool& hasCrossPage);
        Uint16 resolveAddress1(bool& hasCrossPage);
        Uint16 resolveAddress2(bool& hasCrossPage);
        Uint16 resolveAddress3(bool& hasCrossPage);
        Uint16 resolveAddress4(bool& hasCrossPage);
        Uint16 resolveAddress5(bool& hasCrossPage);
        Uint16 resolveAddress6(bool& hasCrossPage);
        Uint16 resolveAddress7(bool& hasCrossPage);
        Uint16 resolveAddress8(bool& hasCrossPage);

        void pushStack(Uint8 value);
        Uint8 pullStack();
        void compare(Uint8 regValue, Uint8 opValue);

        void updateFlag(Uint8 flag, bool value);
        void setFlag(Uint8 flag);
        void clearFlag(Uint8 flag);
        bool checkFlag(Uint8 flag);

    protected:

    private:
};

#endif // CPU_H
