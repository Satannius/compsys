echo ">>> Testing hlt.."
../architecture-tools/asm ./hlt.x64 ./hlt.out
../architecture-tools/sim ./hlt.out hlt.trace
../src/sim ./hlt.out ./hlt.trace
echo ""

echo ">>> Testing nop.."
../architecture-tools/asm ./nop.x64 ./nop.out
../architecture-tools/sim ./nop.out nop.trace
../src/sim ./nop.out ./nop.trace
echo ""

echo ">>> Testing movq.. (%rax,%rcx)"
../architecture-tools/asm ./movq.x64 ./movq.out
../architecture-tools/sim ./movq.out movq.trace
../src/sim ./movq.out ./movq.trace
echo ""
