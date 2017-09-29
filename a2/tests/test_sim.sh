echo ">>> Building simulator .."
cd ../architecture-tools
make
cd ../tests

echo ""

echo ">>> Testing sim_test_hlt.x64 .."
../architecture-tools/asm ./sim_test_hlt.x64 ./sim_test_hlt.out
../architecture-tools/sim ./sim_test_hlt.out

echo ">>> Testing movq.x64 .."
../architecture-tools/asm ./sim_test_movq.x64 ./sim_test_movq.out
../architecture-tools/sim ./sim_test_movq.out

echo ">>> Testing nop.x64 .."
../architecture-tools/asm ./sim_test_movq.x64 ./sim_test_movq.out
../architecture-tools/sim ./sim_test_movq.out
