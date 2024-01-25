#include "DataCollector.h"
#include <cstring>

void DataCollector::CollectData(std::string data)
{
    // Concatenate the partial data to the buffer
    buffer += data;

    // Try to process complete sets of data in the buffer
    ProcessBuffer();
}

bool DataCollector::RegisterDataNotification(DataCallBackType callBackFunction)
{ 
    if((dataCallBackFunction == nullptr) && (callBackFunction != nullptr))
    {
        dataCallBackFunction = callBackFunction;
        return true;
    }
    else
    {
        return false;
    }
}

// Function to check if the serial stream has a valid format
std::string DataCollector::IsValidFormat(const std::string& serialStream,size_t& startPos, size_t& endPos)
{
    std::string completeStrPacket = "";

    // Find the position of the last '/'
    startPos = buffer.find_first_of('/');

    // Find the position of the last '\'
    endPos = buffer.find_last_of('\\');

    if((startPos != std::string::npos) && (endPos != std::string::npos))
    {
        endPos += 1; // +1 is to include last character'\'
        completeStrPacket = buffer.substr(startPos, endPos); // extract data.
    }

    return completeStrPacket;
}


// Function to clear un necessary data ...
void DataCollector::ClearData(size_t& startPos, size_t& endPos)
{
    if(startPos == std::string::npos)
    {
        // Start of the data stream is not found .. clean buffer ..
        buffer.clear();
        return;
    }
    else if(endPos == std::string::npos)
    {
        // End of the data stream is not found 
        // wait for more data ... do nothing ... 
        return ;
    }
    else
    { 
        // Valid data was already extracted ... clear up to end ..
        buffer.erase(0, endPos ); // Erase up to last extracted data ... 
    }
}




// Function to parse the data and update the ScaleData structure
void DataCollector::ParseData(const std::string& serialStream) {
    std::istringstream iss(serialStream);
    char discard;
    std::string strDiscard;
    int length = serialStream.length();
    ScaleData scaleData(channelCount);
/*
serialStream format..
    /
    A    :   5100 Kg
    B    :  17100 Kg
    C    :  22100 Kg
    D    :  15100 Kg
    TOTAL:  59400 Kg
    \
*/

    // Skip the initial '/'
    iss >> discard;
    if(discard != '/')
    {
        std::cout << "Error passing Data... stating position /" << std::endl;
        return;
    }

    // Extract individual data
    for (size_t i = 0; i < (size_t)channelCount; i++)
    {
        iss.ignore(length, ':') >> scaleData.weight[i];
    }

    iss.ignore(length, ':') >> scaleData.totalWeight;

    
    iss >> strDiscard; // Skip the trailing 'Kg'
    iss >> discard; // Skip the trailing '\'
    if(discard != '\\')
    {
        std::cout << "Error passing Data... ending position \\" << std::endl;
        return;
    }

    //Notify the valid data.
    if(dataCallBackFunction != nullptr)
    {
        dataCallBackFunction(std::move(scaleData)); // Sending a copy of scalData..
    }
}


// Function to process complete sets of data in the buffer
void DataCollector::ProcessBuffer() 
{
    bool exit = false;
    size_t startPos = std::string::npos;
    size_t endPos = std::string::npos;

    // If a complete set of data is found, extract and process it
    while (exit != true) 
    {
        std::string strPacket = IsValidFormat(buffer, startPos, endPos);

        // Check if the complete data has a valid format
        if (strPacket.length() > PKT_MIN_SIZE ) // the packet much larger than 
        {
            // Parse and store data
            ParseData(strPacket);
        }
        else 
        {
           //No valid packet .. do nothing, because ClearData will clear unnecessary data ...
        }

        // Clear Unnecessary data 
        ClearData(startPos, endPos);
        if(endPos == std::string::npos)
        {
            //If end character not found exit no more complete data ..
            exit = true;
        }
    }
}