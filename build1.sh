if [ ! -d "bin" ]; then
    mkdir bin
else
    rm -f bin/*
fi

g++ p1-parent.cpp -o bin/p1-parent
./bin/p1-parent

# run chmod +x ./build#.sh
# compile + run $> ./build#.sh