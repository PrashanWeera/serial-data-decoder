 #pragma once

#include <nlohmann/json.hpp>
#include "SerialReader.h"
#include "DataCollector.h"

#define MAX_CHANNELS (10)

/**
 * This is the higher level class which connect with SerialReader class and
 * Data collector class to perform overall operation.
 * 
*/
class DataPasser
{
public:
    /**
     * The constructor 
     * Input :
     *      port : String, The serial port name to be listen.
     *      baudrate : int, The baudrate to communicate with serial port.
     *      channels : int, The number of channels in the serial data stream.
     *      printintervalseconds : int, JSON data printing inteval.
     * Return : None
    */
    DataPasser(std::string port, int baudrate, int channels, int printintervalseconds): 
                                            serialPort(port), 
                                            baudRate(baudrate), 
                                            channelCount(channels), 
                                            printIntervalSeconds(printintervalseconds)
    {
        serialComm = SerialReader::GetInstance(port, baudrate);
        collector = new DataCollector(channelCount);
    }


    /**
     * The Start function start overall operation.
     * Input : None
     * Return : True if the operation successfully starts, otherwise return false.
    */
    bool Start();

    /**
     * The Stop function stop overall operation.
     * Input : None
     * Return : True if the operation successfully starts, otherwise return false.
    */
    void Stop();

    /**
     * The distructor
     * Input : None
     * Return : None
    */
    ~DataPasser();

private:

    std::string serialPort;
    int baudRate;
    int channelCount;
    int printIntervalSeconds;

    SerialReader* serialComm;
    DataCollector* collector;

    //Call back functions..
    void SerialDataCallback(std::string& data); //Call back function for serial data from serial port
    void WeightDataCallBack(ScaleData data); //Call back function for decoded data from DataCollector

    /**
     * This function converts ScaleData to JSON format
     * Input : ScaleData data
     * Return : None
    */
    void ConvertJson(ScaleData data);

    /**
     * The function set timer for 10 second. It used for printing the JSON data to standard output
     * Input : int, the intevel of the timer to print the JSON data.
     * Return : None
    */
    bool SetTimer(int secondIntervel);

};