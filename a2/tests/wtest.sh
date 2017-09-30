echo ">>> Building simulator .."
clear
cd ..
cd src/
make
cd ..
cd tests/

echo ""



echo ">>> -------------------------Tester----------------------------- .."
../architecture-tools/asm ./cmove.x64 ./cmove.out
../architecture-tools/sim ./cmove.out ./cmove.trace
#../src/sim ./cmove.out ./cmove.trace
echo ">>> -------------------------Tester 2----------------------------- .."

../architecture-tools/asm ./cmove.x64 ./cmove.out 
../architecture-tools/sim ./cmove.out ./cmove.trace
../src/sim ./cmove.out ./cmove.trace