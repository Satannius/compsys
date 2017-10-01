echo ">>> Building simulator .."
clear
cd ..
cd src/
make
cd ..
cd tests/

echo ""



echo ">>> -------------------------Tester----------------------------- .."
#../architecture-tools/asm ./Ptest.x64 ./Ptest.out
#../architecture-tools/sim ./Ptest.out ./Ptest.trace
#../src/sim ./Ptest.out ./Ptest.trace
echo ">>> -------------------------Tester 2----------------------------- .."

../architecture-tools/asm ./Ptest.x64 ./Ptest.out 
../architecture-tools/sim ./Ptest.out ./Ptest.trace
../src/sim ./Ptest.out ./Ptest.trace