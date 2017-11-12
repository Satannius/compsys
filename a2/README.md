# CompSys Architecture Tools

## Build

Use `make asm`, `make sim`, `make trace` or just `make`.

**OPTIONAL**: To build the parser use `make parser`. Requires FLEX/BISON. However, the generated C-sources are already included with the source code for maximum portability.

## Usage

The tools include an assembler `asm`, a simulator `sim`, and a trace-prettifier `trace`.

To assemble a program use the assembler as follows:

    ./asm input-file [output-file]

E.g.

    ./asm heapsort.x64 heapsort.x64.out

If you leave out `output-file` the assembler will print out a prettified dump of the generated code.

To simulate an assembled program use the simulator as follows:

    ./sim input-file [trace-file]

E.g.

    ./sim heapsort.x64.out heapsort.trace

The `trace-file` argument is optional. If given the simulator will generate a trace-file.

To prettify a trace-file use the trace-prettifier as follows:

    ./trace trace-file

E.g.

    ./trace heapsort.trace

## Assembler-files

An assembler-file consists of *instructions*, *labels*, and *directives*. For a list of the supported instructions, see [Supported Instructions](#supported-instructions) at the bottom. Currently, the assembler understands the following directives:

##### `.align ALIGNMENT`

Sets the encoding position to a multiple of `ALIGNMENT`, where `ALIGNMENT` is a power of two. E.g. in the following program,

```
    .align 8
data:
    .quad 1234
```

the align directive will ensure that the content of the quad directive is 8-byte aligned.

##### `.pos POSITION`

Sets the encoding position to `POSITION`, where `POSITION` must be greater than or equal to the current encoding position. The position directive can be used to set up the stack as follows:

```
    movq stack, %rsp            // Set up the stack pointer.
    ...
    .pos 0x200
stack:
```

This will ensure that the stack starts at address 0x200.

##### `.quad DATA`

Inserts an eight-byte data value at the current encoding position.

##### `.rand SEED COUNT REPEAT`

Uses `SEED` to generate `COUNT` random quads that are repeated in blocks `REPEAT` times. The result is encoded at the current encoding position. For example, the following directive

```
.rand 1234 3 3
```

generates three blocks of three random quads and inserts them as follows:

```
.quad   BB0CF61B2F1821AD
.quad   E10BBE78D9E9B9BD
.quad   920179BF19EBC8FE
.quad   BB0CF61B2F1821AD
.quad   E10BBE78D9E9B9BD
.quad   920179BF19EBC8FE
.quad   BB0CF61B2F1821AD
.quad   E10BBE78D9E9B9BD
.quad   920179BF19EBC8FE
```

Both `COUNT` and `REPEAT` are optional and are set to `1` when not provided.

## Alignment

When using `MOVQ` for transferring registers to memory or memory to registers the memory addresses must be 8-byte aligned. E.g. the following program will crash the simulator:

```
    movq data, %rax
    movq (%rax), %rbx
    hlt

data:
    .quad 0xABCD
```

Use the alignment directive to ensure that data is aligned:

```
    movq data, %rax
    movq (%rax), %rbx
    hlt

    .align 8
data:
    .quad 0xABCD
```

## Source code layout

The source code is divided into separate modules as follows:

  1) Files prefixed with `asm_` contains source code for the assembler.
  2) Files prefixed with `sim_` contains source code for the simulator.
  3) Files prefixed with `isa_` contains source code for the instruction set. (Used by both the simulator and the assembler.)
  4) Files prefixed with `ano_` contains source code for the annotation library.
  5) Files prefixed with `trace_` contains source code for the trace-prettifier.

## Supported Instructions

  * `hlt` / `halt`
  * `nop`
  * `movq`
  * `addq`
  * `subq`
  * `andq`
  * `xorq`
  * `cmpq`
  * `shrq`
  * `jmp`
  * `jle`
  * `jl`
  * `je`
  * `jne`
  * `jge`
  * `jg`
  * `cmovle`
  * `cmovl`
  * `cmove`
  * `cmovne`
  * `cmovge`
  * `cmovg`
  * `call`
  * `ret`
  * `pushq`
  * `popq`
