cd ../src
make
cd ../tests

#echo ">>> Testing hlt.."
#../architecture-tools/asm ./hlt.x64 ./hlt.out
#../architecture-tools/sim ./hlt.out ./hlt.trace
#../src/sim ./hlt.out ./hlt.trace
#echo ""

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

# echo ">>> Testing movq_RtoM.."
# ../architecture-tools/asm ./movq_RtoM.x64 ./movq_RtoM.out
# ../architecture-tools/sim ./movq_RtoM.out movq_RtoM.trace
# ../src/sim ./movq_RtoM.out ./movq_RtoM.trace
# echo ""

# echo ">>> Testing movq_MtoR.."
# ../architecture-tools/asm ./movq_MtoR.x64 ./movq_MtoR.out
# ../architecture-tools/sim ./movq_MtoR.out movq_MtoR.trace
# ../src/sim ./movq_MtoR.out ./movq_MtoR.trace
# echo ""

#  echo ">>> Testing compq.."
#  ../architecture-tools/asm ./compq.x64 ./compq.out
#  ../architecture-tools/sim ./compq.out ./compq.trace
#  ../src/sim ./compq.out ./compq.trace
#  echo ""

# echo ">>> Testing movq_bug.."
# ../architecture-tools/asm ./movq_bug.x64 ./movq_bug.out
# ../architecture-tools/sim ./movq_bug.out movq_bug.trace
# ../src/sim ./movq_bug.out ./movq_bug.trace
# echo ""