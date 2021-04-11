#include "Serial.cpp"
#include "List.cpp"
#include <windows.h>

class SerialWindows : public Serial {
public:
  HANDLE hSerial;
  SerialWindows(std::string, int baud);
  std::string read();
};


