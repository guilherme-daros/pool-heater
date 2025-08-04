#include <iostream>
using namespace std;
#include "PoolControl.cpp"

class CLI : public PoolControl {
public:
  CLI();
  ~CLI();
  void inputs();
  void outputs();
};
CLI::~CLI() {}

CLI::CLI() {
  timer.setTimer(0);
  state = INPUT_WAIT;
  pumpToHeater_delay = 5;
  timeout_delay = 60 * min;
  timeout = true;
  pump = heater = false;
  timer_reset = setup = false;
  timer_ending = false;
}

void CLI::outputs() {
  cout << "============Controlador de Piscina============" << endl;
  cout << "Estado Atual:" << state << endl;
  cout << "Bomba: " << (pump ? "On" : "Off") << endl;
  cout << "Aquecedor: " << (heater ? "On" : "Off") << endl;
  cout << "Timeout Setado: " << timeout_delay << " Segundos" << endl;
  cout << "Tempo Restante: " << timer.getTime() << endl;
  cout << "Tempo Acabando: " << (timer_ending ? "On" : "Off") << endl;
  cout << "==============================================" << endl;
}
void CLI::inputs() {
  int sel = 0;
  cout << "Actions -> [1] - Ciclar" << endl;
  cout << "           [2] - Setar Timeout" << endl;
  cout << "           [3] - Reset Timeout" << endl;
  cout << "Action: ";
  cin >> sel;
  switch (sel) {
  case 1:
    break;
  case 2:
    setup = true;
    cout << "Insira o valor desejado: ";
    cin >> timeout_delay;
    setup = false;
    break;
  case 3:
    timer_reset = true;
    break;
  default:
    cout << "not in range" << endl;
    sel = 0;
  }
  sel = 0;
  for (int i = 0; i < 10; i++) {
    cout << endl;
  };
  timer.setTimer(timer.getTime() - 1);
}