echo ">>> Building simulator .."
clear
cd ..
cd src/
make
cd ..
cd tests/

echo ""



echo ">>> -------------------------Tester----------------------------- .."
../architecture-tools/asm ./arithmetic_test.x64 ./arithmetic_test.out
../architecture-tools/sim ./arithmetic_test.out ./arithmetic_test.trace
../src/sim ./arithmetic_test.out ./arithmetic_test.trace