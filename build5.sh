if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi

g++ p5-parent.cpp -o bin/p5-parent
g++ p5-child.cpp -o bin/p5-child
./bin/p5-parent
