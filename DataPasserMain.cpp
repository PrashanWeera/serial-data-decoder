#include <iostream>
#include <unistd.h>
#include "SerialReader.h"
#include "DataPasser.h"

/**
 * This is the entry point for serial data decoder. 
 * This application require serial port as command line 
 * argument.
 * Ex: DataPasser /dev/ttyUSB0
 * 
*/

int main(int argc, char* argv[]) 
{
    DataPasser* datapasser = nullptr;
    // Check the argument. The argument count should be 2
    if(argc == 2)
    {
        std::string serial_port = argv[1]; // only argument should be serial port
        int baudRate = 2400; // Baud Rate for serial listening
        int Channels = 4;  //Number of channels
        int printInterval = 10;  // 10s Print interval for JSON data.

        //Invoke data passer.
        datapasser = new DataPasser(serial_port, baudRate, Channels, printInterval);
        if(datapasser->Start() == false)
        {
            std::cout<< "Application exit." << std::endl;
            return -1;
        }
        else
        {
            std::cout<< "Lisining for serial data ..." << std::endl;
        }
    }
    else
    {
        std::cout << "Please provide the serial port in the command line as below."<< std::endl;
        std::cout << "DataPasser /dev/ttyUSB0" << argc << std::endl;
        return 0;
    }
   
    while (true)
    {
        //Wait in the main thread...
        sleep(2);
    }

    if(datapasser != nullptr)
    {
        delete datapasser;
        datapasser = nullptr;
    }
    
    return 0;
}
