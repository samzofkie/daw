CXX := g++
opts:= -Wall -lm -std=c++17 `pkg-config --cflags --libs x11 cairo libpulse-simple`
objs := main.o xwindow.o header.o track_manager.o track.o pcm.o pahandler.o

daw: $(objs)
	g++ $(objs) -o daw $(opts)

main.o: main.cc
	g++ -c main.cc $(opts)

xwindow.o: xwindow.h xwindow.cc
	g++ -c xwindow.cc $(opts)

header.o: header.h header.cc
	g++ -c header.cc $(opts)

track_manager.o: track_manager.h track_manager.cc
	g++ -c track_manager.cc $(opts)

track.o: track.h track.cc
	g++ -c track.cc $(opts)

pcm.o: pcm.h pcm.cc
	g++ -c pcm.cc $(opts)

pahandler.o: pahandler.h pahandler.cc
	g++ -c pahandler.cc $(opts)
