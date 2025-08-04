// #include "InterfaceCLI.cpp"

#include "InterfaceRasp.cpp"

using namespace std;

int main() {
  // CLI Interface;
  RaspInterface Interface;
  while (true) {
    Interface.FSM();
    Interface.outputs();
    Interface.inputs();
    Interface.serialOut();
  }
}
