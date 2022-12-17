CXX := g++
opts:= -Wall -g3 -std=c++17 `pkg-config --cflags --libs x11 cairo libpulse-simple`
objs := main.o xwindow.o header.o track.o grid.o pcm.o pahandler.o

daw: $(objs)
	g++ $(objs) -o daw $(opts)

main.o: main.cc
	g++ -c main.cc $(opts)

xwindow.o: xwindow.h xwindow.cc
	g++ -c xwindow.cc $(opts)

header.o: header.h header.cc
	g++ -c header.cc $(opts)

track.o: track.h track.cc
	g++ -c track.cc $(opts)

grid.o: grid.h grid.cc
	g++ -c grid.cc $(opts)

pcm.o: pcm.h pcm.cc
	g++ -c pcm.cc $(opts)

pahandler.o: pahandler.h pahandler.cc
	g++ -c pahandler.cc $(opts)
