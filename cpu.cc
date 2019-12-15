// @FILE: cpu.cc

#include "cpu.h"

void CPU_Dumb::int_enable(void) { ASM("sti"); }
