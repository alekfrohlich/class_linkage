// @FILE: test_extern_c.cc

namespace namespace1 {
    extern "C" void foo() {}
}

namespace namespace2 {
    extern "C" void foo() {}
}

// /tmp/ccXtHp8y.s: Assembler messages:
// /tmp/ccXtHp8y.s:22: Error: symbol `foo' is already defined
