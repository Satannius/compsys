echo ">>> Building simulator .."
cd src/
make
cd ..
cd tests/
echo ""

errorcount=0
exitcode=0
for f in ./tests/*.x64
do
    file="${f%.*}"
    echo ">>> Testing ${file}"
    asm=$(./architecture-tools/asm ${file}.x64 ${file}.out)
    trace=$(./architecture-tools/sim ${file}.out ${file}.trace)
    sim=$( ./src/sim ${file}.out ${file}.trace)
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
