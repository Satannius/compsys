echo ">>> Building simulator .."
clear
cd ..
cd src/
make
cd ..
cd tests/

echo ""



echo ">>> -------------------------Tester----------------------------- .."
#../architecture-tools/asm ./jump.x64 ./jump.out
#../architecture-tools/sim ./jump.out ./jump.trace
#../src/sim ./jump.out ./jump.trace
echo ">>> -------------------------Tester 2----------------------------- .."

../architecture-tools/asm ./jump.x64 ./jump.out 
../architecture-tools/sim ./jump.out ./jump.trace
../src/sim ./jump.out ./jump.trace