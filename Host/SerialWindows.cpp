#pragma once

#include <windows.h>
#include <iostream>

class SerialWindows {
public:

  SerialWindows(const char*, unsigned long);
  ~SerialWindows();

  void Initialize(const char*, unsigned long);

  void read(unsigned char&, unsigned int);
  void send(unsigned char*, unsigned int);
  void flush();
  bool isConnected();



private:
  HANDLE handler;
  bool isConnect;

};

SerialWindows::SerialWindows(const char* port, unsigned long BaudRate)
  : isConnect(false)
{
  Initialize(port, BaudRate);
}

SerialWindows::~SerialWindows()
{
  if (isConnect)
  {
    isConnect = false;
    CloseHandle(handler);
  }
}

/*Initialization*/
void SerialWindows::Initialize(const char* sPort, unsigned long BaudRate)
{
  handler = CreateFile(sPort,
    GENERIC_READ,
    NULL,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if (handler == INVALID_HANDLE_VALUE)
  {
    std::cout << "Error opening serialWindows port" << sPort << std::endl;
    return;
  }

  DCB sParams;
  if (!GetCommState(handler, &sParams))
  {
    std::cout << "ERROR!::failed to get current serialWindows parameters" << std::endl;
    return;
  }

  sParams.DCBlength = sizeof(DCB);
  sParams.BaudRate = BaudRate;
  sParams.ByteSize = 8;
  sParams.StopBits = ONESTOPBIT;
  sParams.Parity = PARITY_NONE;

  if (!SetCommState(handler, &sParams))
  {
    std::cout << "ALERT!:Failed to set THE SerialWindows port parameters" << std::endl;
    return;
  }

  isConnect = true;
  PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

void SerialWindows::read(unsigned char& data, unsigned int byteSize)
{
  ReadFile(handler, &data, byteSize, NULL, NULL);
}

void SerialWindows::send(unsigned char* data, unsigned int byteSize)
{
  WriteFile(handler, data, byteSize, NULL, NULL);
}

bool SerialWindows::isConnected()
{
  return isConnect;
}

void SerialWindows::flush() {
  PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}
