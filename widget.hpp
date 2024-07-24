#include <math.h>
#include <string>

#include <rtxi/gen/gen_sine.h>
#include <rtxi/widgets.hpp>


// This is an generated header file. You may change the namespace, but 
// make sure to do the same in implementation (.cpp) file
namespace AmpShiftSinGen
{

constexpr std::string_view MODULE_NAME = "Amplitude-Shifted Sinusoid Generator";

enum PARAMETER : Widgets::Variable::Id
{
  FREQ=0,
  AMPLITUDE
};

inline  std::vector<Widgets::Variable::Info> get_default_vars() 
{
  return {
    {
      FREQ,
      "Frequency (Hz)",
      "Frequency of generated sinusoid",
      Widgets::Variable::DOUBLE_PARAMETER,
      double{1.0}
    },
    {
      AMPLITUDE,
      "Amplitude",
      "Amplitude of generated sinusoid",
      Widgets::Variable::DOUBLE_PARAMETER,
      double{1.0}
    }
  };
}

inline std::vector<IO::channel_t> get_default_channels()
{
  return {
      {"Output sinusoid", "Output amplitude-shifted sinusoid", IO::OUTPUT},
  };
}

class Panel : public Widgets::Panel
{
  Q_OBJECT
public:
  Panel(QMainWindow* main_window, Event::Manager* ev_manager);

  // Any functions and data related to the GUI are to be placed here
};

class Component : public Widgets::Component
{
public:
  explicit Component(Widgets::Plugin* hplugin);
  void execute() override;

private:
  void initStimulus();

  GeneratorSine sineWave;

  double dt;
};

class Plugin : public Widgets::Plugin
{
public:
  explicit Plugin(Event::Manager* ev_manager);
};

}  // namespace AmpShiftSinGen
