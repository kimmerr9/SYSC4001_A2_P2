# @author Justin Kim
# @author Dorian Bansoodeb

if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi

g++ p2-parent.cpp -o bin/p2-parent
g++ p2-child.cpp -o bin/p2-child
./bin/p2-parent
