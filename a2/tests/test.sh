echo ">>> Building simulator .."
cd ../src
make
cd ../tests
echo ""

errorcount=0
exitcode=0
for f in ./*.x64
do
    echo ">>> Testing ${f}"
    asm=$(../architecture-tools/asm ./${f} ./${f}.out)
    trace=$(../architecture-tools/sim ./${f}.out ${f}.trace)
    sim=$( ../src/sim ./${f}.out ./${f}.trace)
    actual=${sim:(-4)}
    expected="Done"
    if [ "${actual}" != "${expected}" ]; then
            ((errorcount++))
            echo "    Failed :-("
            echo ""
            exitcode=1
    else
        echo "    Success :-)"
        echo ""
    fi
done

echo "Tests completed with ${errorcount} error(s) detected."

exit $exitcode
