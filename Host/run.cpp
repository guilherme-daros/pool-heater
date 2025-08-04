/*
  •listar todos os eventos ocorridos em um determinado intervalo de datas;
  •Informar  o  tempo  total(em  horas  e  minutos)  que  o  sistema  manteve
      o  aquecedor alimentado, em um determinado intervalo de datas;
  •listar o período do dia com mais utilização do sistema de controle.
*/

#include "List.cpp"
#include "SerialWindows.cpp"
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

void listAll();
void uptimeBetween();
void biggestPeriodBetween();

SerialWindows serial("COM4", 9600);
ostringstream buffer;
List logger;
unsigned char data;
int sel;

int main() {
  buffer << " ";
  while (1) {
    serial.flush();
    while (buffer.str() != "") {
      buffer.clear();
      buffer.str("");
      while (data != '\n') {
        serial.read(data, 1);
        buffer << data;
      }
      logger.insertBeforeFirst(buffer.str());
    }

    cout << "==================PoolHeaterViewer==================" << endl;
    cout << "[0] - Exit" << endl;
    cout << "[1] - List All Events" << endl;
    cout << "[2] - Show events in a period of time" << endl;
    cout << "[3] - Show bigger on-period in a period of time" << endl;
    cout << "Option: ";
    cin >> sel;
    string time1, time2;
    switch (sel) {
    case 0:
      system("PAUSE");
      return 0;
      break;
    case 1:
      listAll();
    case 2:
      cout << "Begin time: ";
      cin >> time1;
      cout << "End time: ";
      cin >> time2;
      cout << "Logs betwenn " << time1 << " and " << time2 << endl;
      uptimeBetween();
      time1 = time2 = "";
    case 3:
      cout << "Begin time: ";
      cin >> time1;
      cout << "End time: ";
      cin >> time2;
      cout << "Logs betwenn " << time1 << " and " << time2 << endl;
      biggestPeriodBetween();
      time1 = time2 = "";
    default:
      break;
    }
  }
}
