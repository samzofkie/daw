CXX := g++
opts:= -Wall -lm -std=c++17 `pkg-config --cflags --libs gtkmm-4.0 cairomm-1.0 libpulse-simple x11`
objs := scxx.o pcm_draw_area.o pahandler.o pcm.o xwindow.o

synth: $(objs)
	g++ $(objs) -o synth $(opts)

scxx.o: scxx.cc
	g++ -c scxx.cc $(opts)

pcm_draw_area.o: pcm_draw_area.cc pcm_draw_area.h
	g++ -c pcm_draw_area.cc $(opts)

pahandler.o: pahandler.h pahandler.cc
	g++ -c pahandler.cc $(opts)

pcm.o: pcm.h pcm.cc
	g++ -c pcm.cc $(opts)

xwindow.o: xwindow.h xwindow.cc
	g++ -c xwindow.cc
