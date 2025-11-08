# @author Justin Kim
# @author Dorian Bansoodeb

if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi

g++ p3-parent.cpp -o bin/p3-parent
g++ p3-child.cpp -o bin/p3-child
./bin/p3-parent
