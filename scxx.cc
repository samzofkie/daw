#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "pcm.h"
#include "pcm_draw_area.h"
#include "pahandler.h"


using namespace std;

class SynthWindow : public Gtk::Window
{ 
  protected:
    Gtk::Grid grid;
    Gtk::Button play_button;
    Gtk::CheckButton waveform_button_sine, waveform_button_sq;
    PCM pcm;
    vector<PCMDrawArea*> draw_areas;
    PAHandler PASink;

    void play_wav() {
      PASink.play(pcm); 
    };

  public:
  	SynthWindow() : 
      play_button("plae"),
      waveform_button_sine("sine"),
      waveform_button_sq("square"),
      pcm(100000, 440),//"/home/sa/snare.wav"),
      PASink(1000)
    {
		  set_title("S.CXX!");
  
      int draw_area_width = 600;
      int draw_area_height = 25; 
      for (int i=0; i < (int)(pcm.data.size() / draw_area_width)+1; i++) {
        vector<int16_t>::iterator s = pcm.data.begin() + (i*draw_area_width);
        vector<int16_t>::iterator e;
        
        uint prospect = ((i+1)*draw_area_width);
        if (prospect > pcm.data.size())
          e = pcm.data.end();
        else
          e = pcm.data.begin() + prospect;

        draw_areas.push_back( new PCMDrawArea(s,e, draw_area_width, draw_area_height));
      }

      grid.attach(play_button,0,0);
      grid.attach(waveform_button_sine,0,1);
      grid.attach(waveform_button_sq,0,2);

      waveform_button_sq.set_group(waveform_button_sine);

      play_button.signal_clicked().connect(sigc::mem_fun(*this,
            &SynthWindow::play_wav) );
      for (int i=0; i<(int)draw_areas.size(); i++) {
        grid.attach(*draw_areas[i],1,i);
      }
      set_child(grid);
 		}

    ~SynthWindow()
    {
      for (auto pcm : draw_areas)
        delete pcm;
    } 
};


int main(int argc, char *argv[])
{  
  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  return app->make_window_and_run<SynthWindow>(argc, argv);
}
