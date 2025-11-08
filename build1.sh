# @author Justin Kim
# @author Dorian Bansoodeb

if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi

g++ p1-parent.cpp -o bin/p1-parent
./bin/p1-parent
