  
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include "SerialReader.h"

bool threadExit{false};  //Thread loop variable.
std::mutex threadExitMutex{}; //Used to lock the threadExit variable
SerialReader* SerialReader::instance = nullptr; //Instance of the class

int SerialReader::ConvertBaud()
{
    int rate = 0;
    switch (baudRate)
    {
    case 1200:
        rate = B1200;
        break;
    case 1800:
        rate = B1800;
        break;
    case 2400:
        rate = B2400;
        break;
    case 4800:
        rate = B4800;
        break;
    case 9600:
        rate = B9600;
        break;
    case 19200:
        rate = B19200;
        break;
    case 38400:
        rate = B38400;
        break;    
    default:
        break;
    }
    return rate;
}

//Thread for listening the serial port.
void* ThreadSerialListen(void* arg) 
{
    SerialListenData serialThreadData = *reinterpret_cast<SerialListenData*>(arg);

    try 
    {
        bool runThread = true;
        while (runThread)
        {
            /* code */
            // Reading data from the serial port
            char buffer[256]; //Buffer to hold serial data..
            
            ssize_t bytesRead = read(serialThreadData.serialPort, buffer, sizeof(buffer));
            if (bytesRead > 0) 
            {
                std::string data(buffer, bytesRead);
                serialThreadData.callBackFunction(data);
            } 
            else
            {   //No data or error ... try next time ...
                // std::cout<< "No data .."<<std::endl;
            }


            // Check the exit condition
            {
                std::unique_lock<std::mutex> lock(threadExitMutex);
                runThread = !threadExit;
            }

            usleep(1000000); //Sleep of 1 second before second read..
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Exception in ThreadSerialListen: " << e.what() << std::endl;
    }

    // Close the serial port
    close(serialThreadData.serialPort);
    return 0;
}

bool SerialReader::ListenPort()
{
    std::cout<<"Opening port :"<< commPort <<std::endl;
    // Open the serial port
    seriPortHandle = open(commPort.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (seriPortHandle == -1) {
        std::cout<<"Error opening serial port "<< commPort <<std::endl;
        return false;
    }

    // Configure the serial port
    struct termios tty;
    if (tcgetattr(seriPortHandle, &tty) != 0) {
        std::cout<<"Error getting serial port attributes"<<std::endl;
        close(seriPortHandle);
        return false;
    }

    int baud = ConvertBaud(); //Convert baud rate c_cflag bit compatible format. 
    if(baud <= 0)
    {
        std::cout<<"Error: Unsupported Baud Rate."<<std::endl;
        close(seriPortHandle);
        return false;
    }

    // Set baud rate for output
    if (cfsetospeed(&tty, baud) == -1) {
        std::cout<<"Error setting output baud rate"<<std::endl;
        close(seriPortHandle);
        return false;
    }
     
    // Set baud rate for input
    if (cfsetispeed(&tty, baud) == -1) {
        std::cout<<"Error setting input baud rate"<<std::endl;
        close(seriPortHandle);
        return false;
    }

    tty.c_cflag |= (CLOCAL | CREAD);  // Enable receiver and ignore modem control lines
    tty.c_cflag &= ~PARENB;           // No parity
    tty.c_cflag &= ~CSTOPB;           // One stop bit
    tty.c_cflag &= ~CSIZE;            // Clear data size bits
    tty.c_cflag |= CS8;               // 8 data bits
    tty.c_cc[VTIME] = 1;              // inter character time out ~0.1 second.
                                      // Note : this character gap is not working could be due
                                      // to serial emulation ...

    if (tcsetattr(seriPortHandle, TCSANOW, &tty) != 0) {
        std::cout<<"Error setting serial port attributes"<<std::endl;
        close(seriPortHandle);
        return false;
    }

    //Passing data to serial listner thread.
    serialThreadData.serialPort = seriPortHandle;
    serialThreadData.callBackFunction = this->callback;
    
    // Create a thread for listen Serial data.
    int result = pthread_create(&serialListenThread, nullptr, ThreadSerialListen, (void*)&serialThreadData);
    if (result != 0) {
        std::cout<<"Error ThreadSerialListen"<<std::endl;
        close(seriPortHandle);
        return false;
    }

    return true;
}


void SerialReader::StopListen()
{
    void* threadResult;
    //Scope for the mutex lock
    {
        std::unique_lock<std::mutex> lock(threadExitMutex);
        threadExit = true;
    }
    pthread_join(serialListenThread, &threadResult); //Wait until thread complete.
}


SerialReader::~SerialReader()
{
    StopListen();
}


// Constructor initilise the comport and baud rate.
SerialReader::SerialReader(std::string port, int baudrate) : commPort(port), baudRate (baudrate)
{

}


bool SerialReader::RegisterCallback(const SerialCallBack callback)
{
    if((this->callback == nullptr) && (callback != nullptr))
    {
        this->callback = callback;
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * GetInstance function return the single instance of the SerialReader. The
 * multiple invoke of this function return only one instance.
 * Input : port is strin serial port that application required to access.
 * Output : returns the instance of the SerialReader. If function failes it returns NULL.
*/
SerialReader * SerialReader::GetInstance(std::string port, int baudrate)
{
    if(SerialReader::instance == nullptr)
    {
        SerialReader::instance = new SerialReader(port,baudrate);
    }
    return SerialReader::instance;
}