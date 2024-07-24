#include <QTimer>
#include <math.h>

#include "widget.hpp"

AmpShiftSinGen::Plugin::Plugin(Event::Manager* ev_manager)
    : Widgets::Plugin(ev_manager, std::string(AmpShiftSinGen::MODULE_NAME))
{
}

AmpShiftSinGen::Panel::Panel(QMainWindow* main_window, Event::Manager* ev_manager)
    : Widgets::Panel(std::string(AmpShiftSinGen::MODULE_NAME), main_window, ev_manager)
{
  setWhatsThis("Amplitude-Shifted Sinusoid Generator");
  createGUI(AmpShiftSinGen::get_default_vars(), {});  // this is required to create the GUI
  refresh();
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

AmpShiftSinGen::Component::Component(Widgets::Plugin* hplugin)
    : Widgets::Component(hplugin,
                         std::string(AmpShiftSinGen::MODULE_NAME),
                         AmpShiftSinGen::get_default_channels(),
                         AmpShiftSinGen::get_default_vars())
    , dt(static_cast<double>(RT::OS::getPeriod()) * 1e-9)
{
}

void AmpShiftSinGen::Component::execute()
{
  switch (this->getState()) {
    case RT::State::EXEC:
      writeoutput(0, (this->sineWave.get() + getValue<double>(AMPLITUDE)));
      break;
    case RT::State::INIT:
      this->initStimulus();
      this->setState(RT::State::EXEC);
      break;
    case RT::State::MODIFY:
      this->initStimulus();
      this->setState(RT::State::PAUSE);      
      break;
    case RT::State::PERIOD:
      this->dt = static_cast<double>(RT::OS::getPeriod()) * 1e-9;
      this->initStimulus();
      this->setState(RT::State::EXEC);
      break;
    case RT::State::PAUSE:
      writeoutput(0, 0);
      break;
    case RT::State::UNPAUSE:
      this->setState(RT::State::EXEC);
      break;
    default:
      break;
  }
}

void AmpShiftSinGen::Component::initStimulus() {
  auto f = getValue<double>(FREQ);
  auto A = getValue<double>(AMPLITUDE);
  this->dt = static_cast<double>(RT::OS::getPeriod()) * 1e-9;

  this->sineWave.clear();
  this->sineWave.init(f, A, this->dt);
}


///////// DO NOT MODIFY BELOW //////////
// The exception is if your plugin is not going to need real-time functionality. 
// For this case just replace the craeteRTXIComponent return type to nullptr. RTXI
// will automatically handle that case and won't attach a component to the real
// time thread for your plugin.

std::unique_ptr<Widgets::Plugin> createRTXIPlugin(Event::Manager* ev_manager)
{
  return std::make_unique<AmpShiftSinGen::Plugin>(ev_manager);
}

Widgets::Panel* createRTXIPanel(QMainWindow* main_window,
                                Event::Manager* ev_manager)
{
  return new AmpShiftSinGen::Panel(main_window, ev_manager);
}

std::unique_ptr<Widgets::Component> createRTXIComponent(
    Widgets::Plugin* host_plugin)
{
  return std::make_unique<AmpShiftSinGen::Component>(host_plugin);
}

Widgets::FactoryMethods fact;

extern "C"
{
Widgets::FactoryMethods* getFactories()
{
  fact.createPanel = &createRTXIPanel;
  fact.createComponent = &createRTXIComponent;
  fact.createPlugin = &createRTXIPlugin;
  return &fact;
}
};

//////////// END //////////////////////
