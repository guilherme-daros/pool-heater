#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {


    termios serial;
    string str_obj = "GeeksForGeeks";
    char* str = &str_obj[0];
    char buffer[15];

    if (argc == 1) {
        cout << "Usage: " << argv[0] << "[device]" << endl;
        return -1;
    }

    cout << "Opening " << argv[1] << endl;

    int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        perror(argv[1]);
        return -1;
    }

    if (tcgetattr(fd, &serial) < 0) {
        perror("Getting configuration");
        return -1;
    }

    // Set up Serial Configuration
    serial.c_iflag = 0;
    serial.c_oflag = 0;
    serial.c_lflag = 0;
    serial.c_cflag = 0;

    serial.c_cc[VMIN] = 0;
    serial.c_cc[VTIME] = 0;

    serial.c_cflag = B115200 | CS8 | CREAD;

    tcsetattr(fd, TCSANOW, &serial); // Apply configuration

    // Attempt to send and receive
    cout << "Sending: " << str << endl;;

    int wcount = write(fd, str, strlen(str));
    if (wcount < 0) {
        perror("Write");
        return -1;
    }
    else {
        cout << "Sent " << wcount << " characters" << endl;
    }
    for (;;);
    int rcount = read(fd, buffer, sizeof(buffer));
    if (rcount < 0) {
        perror("Read");
        return -1;
    }
    else {
        cout << "Received: " << rcount << " characters" << endl;
    }

    buffer[rcount] = '\0';
    cout << "Received: " << buffer << endl;

    close(fd);
}