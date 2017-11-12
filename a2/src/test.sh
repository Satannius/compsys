echo ">>> Building assembler and reference simulator .."
cd ..
cd architecture-tools/
make
cd ..
echo ">>> Building simulator .."
cd src/
make
echo ""

errorcount=0
exitcode=0
for f in ./tests/*.x64
do
    file="${f%.*}"
    echo ">>> Testing ${file}"
    asm=$(../architecture-tools/asm ${file}.x64 ${file}.out)
    trace=$(../architecture-tools/sim ${file}.out ${file}.trace)
    sim=$( ../src/sim ${file}.out ${file}.trace)
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

echo ">>> Removing generated .out and .trace files .."
for f in ./tests/*.x64
do
    file="${f%.*}"
    rm ${file}.out
    rm ${file}.trace
done
echo "files has been removed."

exit $exitcode
