// @FILE: cpu.h

#define ASM __asm__ __volatile__

class CPU_Good {
public:
	static void int_enable(void) { ASM("sti"); }
};

class CPU_Dumb {
public:
	static void int_enable(void);
};