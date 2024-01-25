#include "gtest/gtest.h"
#include "SerialReader.h"
#include "DataCollector.h"


class Serialtest : public ::testing::Test
{
public:

protected:
    SerialReader * serialInstance = SerialReader::GetInstance("/dev/pts/8", 2400);
    std::string data;

};

TEST_F(Serialtest, TestSerialOpen)
{
    // Need admin priviladge
    //EXPECT_TRUE(serialInstance->ListenPort());
}

TEST_F(Serialtest, TestSerialRegister)
{
    EXPECT_FALSE(serialInstance->RegisterCallback(NULL));
    EXPECT_TRUE(serialInstance->RegisterCallback([] (std::string& data){
        std::cout << data << std::endl;
    }));
}

int channelCount = 4;

// Test fixture for DataCollector class
class DataCollectorTest : public ::testing::Test {
protected:
    // Member variables for the test fixture
    DataCollector dataCollector;  // 4 channels for testing
    

    DataCollectorTest() : dataCollector(channelCount) {};
 
};

// Test case to check if DataCollector correctly registers data notification
TEST_F(DataCollectorTest, RegisterDataNotificationTest) 
{
    ASSERT_FALSE(dataCollector.RegisterDataNotification(nullptr));  // Null callback should fail
    ASSERT_TRUE(dataCollector.RegisterDataNotification([](ScaleData data) {
        // Inside the lambda function, you can perform additional assertions
        std::cout<< data.totalWeight << std::endl;
    }));
}


// Test case to check if DataCollector correctly collects data
TEST_F(DataCollectorTest, CollectDataTest) 
{
    std::string testDataString = "/\nA    :   1000 Kg\nB    :   2000 Kg\nC    :   3000 Kg\nD    :   4000 Kg\nTOTAL:  10000 Kg\n\\";
    dataCollector.CollectData(testDataString);
    testDataString = "/\nA    :   1000 Kg\nB    :   2000 Kg\nC    :   3000 Kg\nD    :   4000 Kg\nTOTAL:  10000 Kg\n";
    dataCollector.CollectData(testDataString);
    testDataString = "\\";
    dataCollector.CollectData(testDataString);
}

