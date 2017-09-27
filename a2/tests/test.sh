cd ../src
make
cd ../tests

# echo ">>> Testing hlt.."
# ../architecture-tools/asm ./hlt.x64 ./hlt.out
# ../architecture-tools/sim ./hlt.out hlt.trace
# ../src/sim ./hlt.out ./hlt.trace
# echo ""

# echo ">>> Testing nop.."
# ../architecture-tools/asm ./nop.x64 ./nop.out
# ../architecture-tools/sim ./nop.out nop.trace
# ../src/sim ./nop.out ./nop.trace
# echo ""

# echo ">>> Testing movq_RtoR.."
# ../architecture-tools/asm ./movq_RtoR.x64 ./movq_RtoR.out
# ../architecture-tools/sim ./movq_RtoR.out movq_RtoR.trace
# ../src/sim ./movq_RtoR.out ./movq_RtoR.trace
# echo ""

# echo ">>> Testing movq_ItoR.."
# ../architecture-tools/asm ./movq_ItoR.x64 ./movq_ItoR.out
# ../architecture-tools/sim ./movq_ItoR.out movq_ItoR.trace
# ../src/sim ./movq_ItoR.out ./movq_ItoR.trace
# echo ""

# echo ">>> Testing movq_RtoM1.."
# ../architecture-tools/asm ./movq_RtoM1.x64 ./movq_RtoM1.out
# ../architecture-tools/sim ./movq_RtoM1.out movq_RtoM1.trace
# ../src/sim ./movq_RtoM1.out ./movq_RtoM1.trace
# echo ""

echo ">>> Testing movq_RtoM2.."
../architecture-tools/asm ./movq_RtoM2.x64 ./movq_RtoM2.out
../architecture-tools/sim ./movq_RtoM2.out movq_RtoM2.trace
../src/sim ./movq_RtoM2.out ./movq_RtoM2.trace
echo ""
