# ARM64 Instruction List

## 1. **Data Movement Instructions**
- `mov`           - Move or copy data between registers
- `ldr`           - Load data from memory to register
- `str`           - Store data from register to memory
- `ldrb` / `strb` - Load/store a single byte (8 bits)
- `ldrh` / `strh` - Load/store a halfword (16 bits)
- `ldrsw`         - Load signed 32-bit word into a 64-bit register
- `adr`           - Load a memory address into a register
- `adrp`          - Load the page address into a register
- `mrs`           - Move from system register to general-purpose register
- `msr`           - Move to system register from general-purpose register

## 2. **Arithmetic Instructions**
- `add`           - Add two registers or immediate value
- `sub`           - Subtract one register or immediate from another
- `mul`           - Multiply two registers
- `smaddl`        - Signed multiply-accumulate on long values
- `madd`          - Multiply and add
- `neg`           - Negate a register value (two's complement)
- `cmp`           - Compare two values (subtract without saving result)
- `cmn`           - Compare negative

## 3. **Logical and Bitwise Instructions**
- `and`           - Bitwise AND
- `orr`           - Bitwise OR
- `eor`           - Bitwise XOR
- `bic`           - Bitwise AND with complement (clear bits)
- `lsl`           - Logical shift left
- `lsr`           - Logical shift right
- `asr`           - Arithmetic shift right (sign extend)
- `ror`           - Rotate bits right
- `not`           - Bitwise NOT (flip all bits)

## 4. **Comparison Instructions**
- `cmp`           - Compare two registers
- `tst`           - Test bits (AND two values, set flags, but discard result)

## 5. **Branching and Control Flow Instructions**
- `b`             - Unconditional branch (jump)
- `bl`            - Branch with link (call a subroutine)
- `ret`           - Return from subroutine
- `cbz`           - Branch if register is zero
- `cbnz`          - Branch if register is not zero
- `tbz`           - Test and branch if bit is zero
- `tbnz`          - Test and branch if bit is not zero
- `b.eq`, `b.ne`  - Branch if equal/not equal
- `b.gt`, `b.ge`  - Branch if greater than/greater or equal
- `b.lt`, `b.le`  - Branch if less than/less or equal

## 6. **Load and Store Multiple Instructions**
- `ldp`           - Load pair of registers from memory
- `stp`           - Store pair of registers to memory
- `ldm`           - Load multiple registers (ARM32, not in ARM64)
- `stm`           - Store multiple registers (ARM32, not in ARM64)

## 7. **Data Processing Instructions**
- `add` / `sub`   - Add or subtract two registers
- `mul` / `madd`  - Multiply, optionally with accumulation
- `div`           - Unsigned division
- `sdiv` / `udiv` - Signed/unsigned division
- `csel`          - Conditional selection between two registers
- `csinv`         - Conditional select invert
- `cinc`          - Conditional increment
- `cinv`          - Conditional inversion

## 8. **SIMD (NEON) and Vector Instructions**
- `addv`          - Add vector elements
- `movi`          - Move immediate value into SIMD register
- `dup`           - Duplicate element across the SIMD register
- `ld1` / `st1`   - Load/store single-element structure
- `mla`           - Multiply-add
- `saddw` / `uaddw` - Add wide signed/unsigned

## 9. **Floating-Point Instructions**
- `fadd`          - Floating-point add
- `fsub`          - Floating-point subtract
- `fmul`          - Floating-point multiply
- `fdiv`          - Floating-point divide
- `fsqrt`         - Floating-point square root
- `fcmp`          - Floating-point comparison
- `fmov`          - Move floating-point value between registers

## 10. **System Instructions**
- `svc`           - Supervisor call (system call)
- `hvc`           - Hypervisor call
- `dc`            - Data cache operations
- `ic`            - Instruction cache operations
- `at`            - Address translation operations
- `mrs`           - Access system registers
- `msr`           - Write to system register