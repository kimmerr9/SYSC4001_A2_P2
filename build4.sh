if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi

g++ p4-parent.cpp -o bin/p4-parent
g++ p4-child.cpp -o bin/p4-child
./bin/p4-parent
