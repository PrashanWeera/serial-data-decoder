 #pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>  // for std::swap

#define PKT_MIN_SIZE (12)  // Minimun valid packet size..

/**
 * This data structure used to store the devoded data. It used channel count as a parameter 
 * to create the internel structure.
*/
struct ScaleData {
    size_t channelCount;
    int* weight;
    int totalWeight;

    /**
     * Constructor 
     * Input : 
     *      channelcount: int, is the number of data channels that comes through serial port.
     * Output : None
    */
    ScaleData(size_t count) : channelCount(count), weight(new int[channelCount]), totalWeight(0) {}

    /**
     * Move constructor 
    */
    ScaleData(ScaleData&& other) noexcept : channelCount(0), weight(nullptr), totalWeight(0) {
        swap(*this, other);
    }

    /**
     * Move assignment operator
    */
    ScaleData& operator=(ScaleData&& other) noexcept {
        swap(*this, other);
        return *this;
    }

    /**
     * Destructor to free the allocated memory
    */
    ~ScaleData() {
        delete[] weight;
    }

    /**
     * Swap function for efficient swapping during move operations 
     * Input : 
     *      first: ScaleData,the firste argument for swap operation.
     *      second: ScaleData,the second argument for swap operation.
     * Output : None
    */
    friend void swap(ScaleData& first, ScaleData& second) noexcept {
        using std::swap;

        // Swap all members
        swap(first.channelCount, second.channelCount);
        swap(first.weight, second.weight);
        swap(first.totalWeight, second.totalWeight);
    }
};

/**
 * This is class used to collect serial data and decode them to required format. If the 
 * serial data received partially it keep collecting data until complete data received.
 * it assume the data stream received in the following format. 
 *  /
 *  A    :   5100 Kg
 *  B    :  17100 Kg
 *  C    :  22100 Kg
 *  D    :  15100 Kg
 *  TOTAL:  59400 Kg
 *  \
 */
class DataCollector
{
public:

    /**
     * This is the signature for the call back function.
    */
    using DataCallBackType = std::function<void (ScaleData&& data)>;

    /**
     * Constructor 
     * Input : 
     *      channelcount: int, is the number of data channels that comes through serial port.
     * Output : None
    */
    DataCollector(int channelcount) : channelCount(channelcount){};

    /**
     * This is the function which accept string serial data. Once it receive new data it
     * pass the data and try for create complete data packet to decode data. It discard all
     * unnecessary data.
     * Input : 
     *      data: string, the data received from the serial port.
     * Output : None
    */
    void CollectData(std::string data);

    /**
     * This is the function for register decoded data.
     * Input : 
     *      callBackFunction: DataCallBackType, the call back function for receive decoded data.
     * Output : Returns True if success, otherwise return false.
    */
    bool RegisterDataNotification(DataCallBackType callBackFunction);
    
private:
    DataCallBackType dataCallBackFunction = nullptr;
    int channelCount;
    std::string buffer; // Buffer to store serial received data

    /**
     * This is the function check for start of the stream character '/' and
     * end of the stream '\'. Ifvalid data stream found it returns the extracted data.
     * Further it sent the startPos and endPos if the stream.
     * Input : 
     *      serialStream: string stream to check for validity
     *      startPos: [out] parameter to return start position of the stream
     *      endPos: [out] parameter to return end position of the stream
     * Output : Returns the extracted stream if scuccess, otherwise return "".
    */
    std::string IsValidFormat(const std::string& serialStream, size_t& startPos, size_t& endPos);

    /**
     * This is the function to clear the data buffer after extracting the valid data.
     * It works in hand with the IsValidFormat function.
     * Input : 
     *      startPos: parameter received from IsValidFormat function
     *      endPos: parameter received from IsValidFormat function
     * Output : None.
    */
    void ClearData(size_t& startPos, size_t& endPos);

    /**
     * This is the function which convert extracted string streamd to ScaleData format and invoke 
     * registered callback function of a valid ScaleData found.
     * Input : 
     *      serialStream: string, extracted serial stream.
     * Output : None.
    */
    void ParseData(const std::string& serialStream);

    /**
     * This function starts the operation of the passing (decoding) the received data. Once the
     * serial buffer updated, this function invoked for processing.
     * Input : None
     * Output : None
    */
    void ProcessBuffer();
};
