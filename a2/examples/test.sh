cd ../architecture-tools
make
cd ../examples

echo ">>> Testing test.x64"
../architecture-tools/asm ./test.x64 ./test.out
../architecture-tools/sim ./test.out
