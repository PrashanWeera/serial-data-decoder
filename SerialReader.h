 #pragma once

 #include <string>
 #include <functional>

/**
 * This structure used pass data to serial port listening thread. 
*/
struct SerialListenData
{
    int serialPort; //Handle of the serial port.
    std::function<void (std::string& data)> callBackFunction; //call back function for send serial data..
};

/**
 * This  SerialReader class listening to serial data stream from the COM port. It 
 * contains a worker thread which listening to serial data. Once the data is available it
 * send back the data to registred call back function.
*/
class SerialReader
{

public:
    using SerialCallBack = std::function<void (std::string& data)>; //Signature for the call back function.

    /**
     * The RegisterCallback function used to register call back function for receive serial data. 
     * Input :
     *      callback SerialCallBack callback function.
     * Return : If function succeed it returns the true else it returns false.
    */
    bool RegisterCallback(const SerialCallBack callback);

    /**
     * The function start serial listening functionality. Internally is start worker thread and listern to 
     * serial data. Once data available it send through registered call back function.  
     * Input : None
     * Return : If function succeed it returns the true else it returns false.
    */
    bool ListenPort();

    /**
     * The function stops the serial listening functionality. Internally it stops the worker thread. 
     * Input : None
     * Return : If function succeed it returns the true else it returns false.
    */
    void StopListen();

    /**
     * The distructor
     * Input : None
     * Return : None
    */
    ~SerialReader();


    /**
     * GetInstance function return the single instance of the SerialReader. The
     * multiple invoke of this function return only one instance.
     * Input : 
     *      port: string, is serial port that application required to access.
     *      baudRate: int, is the data rate for serial communcation.
     * Output : returns the instance of the SerialReader. If function failes it returns NULL.
    */
    static SerialReader * GetInstance(std::string port, int baudRate);

private:
    //Use for singleton implementation..
    SerialReader(const SerialReader& instance) = delete; //Delete copy constructor
    SerialReader& operator=(const SerialReader& instance) = delete; // Delete assignment operator

    /**
     * Private constructor, for singleton implementatio. 
     * Input : 
     *      port: string, is serial port that application required to access.
     *      baudRate: int, is the data rate for serial communcation.
     * Output : None.
    */
    SerialReader(std::string port, int baudRate);

    //Private variables..
    SerialCallBack callback = nullptr; //Callback function.
    std::string commPort;
    int baudRate;
    static SerialReader * instance; //Use for singleton implementation.
    int seriPortHandle{}; //Handle of the serial port
    SerialListenData serialThreadData{}; //Used to pass data to worker thread.
    pthread_t serialListenThread; //Thread handle..

   
    /**
     * This function used to convert BaudRage from numaric value to c_cflag bit comatible way.
     * Input : None
     * Output : Int, retusn the c_cflag bit compatible baud rate.
    */
    int ConvertBaud();

};