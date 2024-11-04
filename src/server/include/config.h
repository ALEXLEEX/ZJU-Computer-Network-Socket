/**
 * @file config.h
 * @author lqy
 * @brief Server configuration file.
 * @date 2024-11-04
 */

#ifndef _SERVER_CONFIG_H
#define _SERVER_CONFIG_H

#include <string>
#include <vector>
#include <map>

/* Number of citys in configuration. */
const int CityNums = 24;

/* Number of records in each city's weather information. */
const int WeatherRecordNums = 7;

/**
 * @brief City names lists.
 */
const std::vector<std::string> CityNames = {
    "Beijing",   // 0
    "Shanghai",  // 1
    "Guangzhou", // 2
    "Shenzhen",  // 3
    "Hangzhou",  // 4
    "Nanjing",   // 5
    "Chengdu",   // 6
    "Wuhan",     // 7
    "Chongqing", // 8
    "Suzhou",    // 9
    "Harbin",    // 10
    "Qingdao",   // 11
    "Jinan",     // 12
    "Tianjin",   // 13
    "Changchun", // 14
    "Nanchang",  // 15
    "Xian",      // 16
    "Lanzhou",   // 17
    "Guiyang",   // 18
    "Zhuhai",    // 19
    "Dongguan",  // 20
    "Foshan",    // 21
    "Ningbo",    // 22
    "Hefei"      // 23
};

/**
 * @brief City weather information.
 * @note The weather information is in format of "YYYY-MM-DD: temperature, weather, humidity, wind speed, wind direction."
 */
const std::vector<std::map<std::string, std::string>> WeatherInfo = {
    // 0
    std::map<std::string, std::string>{
        {"2024-11-04", "27 degree, Sunny, 101 humidity, 100 wind speed, Northeast wind direction."},
        {"2024-11-05", "26 degree, Sunny, 102 humidity, 101 wind speed, East wind direction."},
        {"2024-11-06", "25 degree, Windy, 103 humidity, 102 wind speed, Southeast wind direction."},
        {"2024-11-07", "24 degree, Rainy, 104 humidity, 103 wind speed, South wind direction."},
        {"2024-11-08", "23 degree, Sunny, 105 humidity, 104 wind speed, Southwest wind direction."},
        {"2024-11-09", "22 degree, Foggy, 106 humidity, 105 wind speed, West wind direction."},
        {"2024-11-10", "21 degree, Sunny, 107 humidity, 106 wind speed, Northwest wind direction."},
    },
    // 1
    std::map<std::string, std::string>{
        {"2024-11-04", "16 degree, Sunny, 80 humidity, 80 wind speed, Northeast wind direction."},
        {"2024-11-05", "15 degree, Sunny, 81 humidity, 81 wind speed, East wind direction."},
        {"2024-11-06", "14 degree, Windy, 82 humidity, 82 wind speed, Southeast wind direction."},
        {"2024-11-07", "13 degree, Rainy, 83 humidity, 83 wind speed, South wind direction."},
        {"2024-11-08", "12 degree, Sunny, 84 humidity, 84 wind speed, Southwest wind direction."},
        {"2024-11-09", "11 degree, Foggy, 85 humidity, 85 wind speed, West wind direction."},
        {"2024-11-10", "10 degree, Sunny, 86 humidity, 86 wind speed, Northwest wind direction."},
    },
    // 2
    std::map<std::string, std::string>{
        {"2024-11-04", "30 degree, Sunny, 90 humidity, 90 wind speed, Northeast wind direction."},
        {"2024-11-05", "29 degree, Sunny, 91 humidity, 91 wind speed, East wind direction."},
        {"2024-11-06", "28 degree, Windy, 92 humidity, 92 wind speed, Southeast wind direction."},
        {"2024-11-07", "27 degree, Rainy, 93 humidity, 93 wind speed, South wind direction."},
        {"2024-11-08", "26 degree, Sunny, 94 humidity, 94 wind speed, Southwest wind direction."},
        {"2024-11-09", "25 degree, Foggy, 95 humidity, 95 wind speed, West wind direction."},
        {"2024-11-10", "24 degree, Sunny, 96 humidity, 96 wind speed, Northwest wind direction."},
    },
    // 3
    std::map<std::string, std::string>{
        {"2024-11-04", "20 degree, Sunny, 70 humidity, 70 wind speed, Northeast wind direction."},
        {"2024-11-05", "19 degree, Sunny, 71 humidity, 71 wind speed, East wind direction."},
        {"2024-11-06", "18 degree, Windy, 72 humidity, 72 wind speed, Southeast wind direction."},
        {"2024-11-07", "17 degree, Rainy, 73 humidity, 73 wind speed, South wind direction."},
        {"2024-11-08", "16 degree, Sunny, 74 humidity, 74 wind speed, Southwest wind direction."},
        {"2024-11-09", "15 degree, Foggy, 75 humidity, 75 wind speed, West wind direction."},
        {"2024-11-10", "14 degree, Sunny, 76 humidity, 76 wind speed, Northwest wind direction."},
    },
    // 4
    std::map<std::string, std::string>{
        {"2024-11-04", "25 degree, Sunny, 80 humidity, 80 wind speed, Northeast wind direction."},
        {"2024-11-05", "24 degree, Sunny, 81 humidity, 81 wind speed, East wind direction."},
        {"2024-11-06", "23 degree, Windy, 82 humidity, 82 wind speed, Southeast wind direction."},
        {"2024-11-07", "22 degree, Rainy, 83 humidity, 83 wind speed, South wind direction."},
        {"2024-11-08", "21 degree, Sunny, 84 humidity, 84 wind speed, Southwest wind direction."},
        {"2024-11-09", "20 degree, Foggy, 85 humidity, 85 wind speed, West wind direction."},
        {"2024-11-10", "19 degree, Sunny, 86 humidity, 86 wind speed, Northwest wind direction."},
    },
    // 5
    std::map<std::string, std::string>{
        {"2024-11-04", "18 degree, Sunny, 60 humidity, 60 wind speed, Northeast wind direction."},
        {"2024-11-05", "17 degree, Sunny, 61 humidity, 61 wind speed, East wind direction."},
        {"2024-11-06", "16 degree, Windy, 62 humidity, 62 wind speed, Southeast wind direction."},
        {"2024-11-07", "15 degree, Rainy, 63 humidity, 63 wind speed, South wind direction."},
        {"2024-11-08", "14 degree, Sunny, 64 humidity, 64 wind speed, Southwest wind direction."},
        {"2024-11-09", "13 degree, Foggy, 65 humidity, 65 wind speed, West wind direction."},
        {"2024-11-10", "12 degree, Sunny, 66 humidity, 66 wind speed, Northwest wind direction."},
    },
    // 6
    std::map<std::string, std::string>{
        {"2024-11-04", "22 degree, Sunny, 70 humidity, 70 wind speed, Northeast wind direction."},
        {"2024-11-05", "21 degree, Sunny, 71 humidity, 71 wind speed, East wind direction."},
        {"2024-11-06", "20 degree, Windy, 72 humidity, 72 wind speed, Southeast wind direction."},
        {"2024-11-07", "19 degree, Rainy, 73 humidity, 73 wind speed, South wind direction."},
        {"2024-11-08", "18 degree, Sunny, 74 humidity, 74 wind speed, Southwest wind direction."},
        {"2024-11-09", "17 degree, Foggy, 75 humidity, 75 wind speed, West wind direction."},
        {"2024-11-10", "16 degree, Sunny, 76 humidity, 76 wind speed, Northwest wind direction."},
    },
    // 7
    std::map<std::string, std::string>{
        {"2024-11-04", "12 degree, Sunny, 50 humidity, 50 wind speed, Northeast wind direction."},
        {"2024-11-05", "11 degree, Sunny, 51 humidity, 51 wind speed, East wind direction."},
        {"2024-11-06", "10 degree, Windy, 52 humidity, 52 wind speed, Southeast wind direction."},
        {"2024-11-07", "9 degree, Rainy, 53 humidity, 53 wind speed, South wind direction."},
        {"2024-11-08", "8 degree, Sunny, 54 humidity, 54 wind speed, Southwest wind direction."},
        {"2024-11-09", "7 degree, Foggy, 55 humidity, 55 wind speed, West wind direction."},
        {"2024-11-10", "6 degree, Sunny, 56 humidity, 56 wind speed, Northwest wind direction."},
    },
    // 8
    std::map<std::string, std::string>{
        {"2024-11-04", "26 degree, Sunny, 60 humidity, 60 wind speed, Northeast wind direction."},
        {"2024-11-05", "25 degree, Sunny, 61 humidity, 61 wind speed, East wind direction."},
        {"2024-11-06", "24 degree, Windy, 62 humidity, 62 wind speed, Southeast wind direction."},
        {"2024-11-07", "23 degree, Rainy, 63 humidity, 63 wind speed, South wind direction."},
        {"2024-11-08", "22 degree, Sunny, 64 humidity, 64 wind speed, Southwest wind direction."},
        {"2024-11-09", "21 degree, Foggy, 65 humidity, 65 wind speed, West wind direction."},
        {"2024-11-10", "20 degree, Sunny, 66 humidity, 66 wind speed, Northwest wind direction."},
    },
    // 9
    std::map<std::string, std::string>{
        {"2024-11-04", "14 degree, Sunny, 40 humidity, 40 wind speed, Northeast wind direction."},
        {"2024-11-05", "13 degree, Sunny, 41 humidity, 41 wind speed, East wind direction."},
        {"2024-11-06", "12 degree, Windy, 42 humidity, 42 wind speed, Southeast wind direction."},
        {"2024-11-07", "11 degree, Rainy, 43 humidity, 43 wind speed, South wind direction."},
        {"2024-11-08", "10 degree, Sunny, 44 humidity, 44 wind speed, Southwest wind direction."},
        {"2024-11-09", "9 degree, Foggy, 45 humidity, 45 wind speed, West wind direction."},
        {"2024-11-10", "8 degree, Sunny, 46 humidity, 46 wind speed, Northwest wind direction."},
    },
    // 10
    std::map<std::string, std::string>{
        {"2024-11-04", "28 degree, Sunny, 50 humidity, 50 wind speed, Northeast wind direction."},
        {"2024-11-05", "27 degree, Sunny, 51 humidity, 51 wind speed, East wind direction."},
        {"2024-11-06", "26 degree, Windy, 52 humidity, 52 wind speed, Southeast wind direction."},
        {"2024-11-07", "25 degree, Rainy, 53 humidity, 53 wind speed, South wind direction."},
        {"2024-11-08", "24 degree, Sunny, 54 humidity, 54 wind speed, Southwest wind direction."},
        {"2024-11-09", "23 degree, Foggy, 55 humidity, 55 wind speed, West wind direction."},
        {"2024-11-10", "22 degree, Sunny, 56 humidity, 56 wind speed, Northwest wind direction."},
    },
    // 11
    std::map<std::string, std::string>{
        {"2024-11-04", "16 degree, Sunny, 30 humidity, 30 wind speed, Northeast wind direction."},
        {"2024-11-05", "15 degree, Sunny, 31 humidity, 31 wind speed, East wind direction."},
        {"2024-11-06", "14 degree, Windy, 32 humidity, 32 wind speed, Southeast wind direction."},
        {"2024-11-07", "13 degree, Rainy, 33 humidity, 33 wind speed, South wind direction."},
        {"2024-11-08", "12 degree, Sunny, 34 humidity, 34 wind speed, Southwest wind direction."},
        {"2024-11-09", "11 degree, Foggy, 35 humidity, 35 wind speed, West wind direction."},
        {"2024-11-10", "10 degree, Sunny, 36 humidity, 36 wind speed, Northwest wind direction."},
    },
    // 12
    std::map<std::string, std::string>{
        {"2024-11-04", "30 degree, Sunny, 40 humidity, 40 wind speed, Northeast wind direction."},
        {"2024-11-05", "29 degree, Sunny, 41 humidity, 41 wind speed, East wind direction."},
        {"2024-11-06", "28 degree, Windy, 42 humidity, 42 wind speed, Southeast wind direction."},
        {"2024-11-07", "27 degree, Rainy, 43 humidity, 43 wind speed, South wind direction."},
        {"2024-11-08", "26 degree, Sunny, 44 humidity, 44 wind speed, Southwest wind direction."},
        {"2024-11-09", "25 degree, Foggy, 45 humidity, 45 wind speed, West wind direction."},
        {"2024-11-10", "24 degree, Sunny, 46 humidity, 46 wind speed, Northwest wind direction."},
    },
    // 13
    std::map<std::string, std::string>{
        {"2024-11-04", "18 degree, Sunny, 20 humidity, 20 wind speed, Northeast wind direction."},
        {"2024-11-05", "17 degree, Sunny, 21 humidity, 21 wind speed, East wind direction."},
        {"2024-11-06", "16 degree, Windy, 22 humidity, 22 wind speed, Southeast wind direction."},
        {"2024-11-07", "15 degree, Rainy, 23 humidity, 23 wind speed, South wind direction."},
        {"2024-11-08", "14 degree, Sunny, 24 humidity, 24 wind speed, Southwest wind direction."},
        {"2024-11-09", "13 degree, Foggy, 25 humidity, 25 wind speed, West wind direction."},
        {"2024-11-10", "12 degree, Sunny, 26 humidity, 26 wind speed, Northwest wind direction."},
    },
    // 14
    std::map<std::string, std::string>{
        {"2024-11-04", "32 degree, Sunny, 30 humidity, 30 wind speed, Northeast wind direction."},
        {"2024-11-05", "31 degree, Sunny, 31 humidity, 31 wind speed, East wind direction."},
        {"2024-11-06", "30 degree, Windy, 32 humidity, 32 wind speed, Southeast wind direction."},
        {"2024-11-07", "29 degree, Rainy, 33 humidity, 33 wind speed, South wind direction."},
        {"2024-11-08", "28 degree, Sunny, 34 humidity, 34 wind speed, Southwest wind direction."},
        {"2024-11-09", "27 degree, Foggy, 35 humidity, 35 wind speed, West wind direction."},
        {"2024-11-10", "26 degree, Sunny, 36 humidity, 36 wind speed, Northwest wind direction."},
    },
    // 15
    std::map<std::string, std::string>{
        {"2024-11-04", "20 degree, Sunny, 10 humidity, 10 wind speed, Northeast wind direction."},
        {"2024-11-05", "19 degree, Sunny, 11 humidity, 11 wind speed, East wind direction."},
        {"2024-11-06", "18 degree, Windy, 12 humidity, 12 wind speed, Southeast wind direction."},
        {"2024-11-07", "17 degree, Rainy, 13 humidity, 13 wind speed, South wind direction."},
        {"2024-11-08", "16 degree, Sunny, 14 humidity, 14 wind speed, Southwest wind direction."},
        {"2024-11-09", "15 degree, Foggy, 15 humidity, 15 wind speed, West wind direction."},
        {"2024-11-10", "14 degree, Sunny, 16 humidity, 16 wind speed, Northwest wind direction."},
    },
    // 16
    std::map<std::string, std::string>{
        {"2024-11-04", "34 degree, Sunny, 20 humidity, 20 wind speed, Northeast wind direction."},
        {"2024-11-05", "33 degree, Sunny, 21 humidity, 21 wind speed, East wind direction."},
        {"2024-11-06", "32 degree, Windy, 22 humidity, 22 wind speed, Southeast wind direction."},
        {"2024-11-07", "31 degree, Rainy, 23 humidity, 23 wind speed, South wind direction."},
        {"2024-11-08", "30 degree, Sunny, 24 humidity, 24 wind speed, Southwest wind direction."},
        {"2024-11-09", "29 degree, Foggy, 25 humidity, 25 wind speed, West wind direction."},
        {"2024-11-10", "28 degree, Sunny, 26 humidity, 26 wind speed, Northwest wind direction."},
    },
    // 17
    std::map<std::string, std::string>{
        {"2024-11-04", "22 degree, Sunny, 0 humidity, 0 wind speed, Northeast wind direction."},
        {"2024-11-05", "21 degree, Sunny, 1 humidity, 1 wind speed, East wind direction."},
        {"2024-11-06", "20 degree, Windy, 2 humidity, 2 wind speed, Southeast wind direction."},
        {"2024-11-07", "19 degree, Rainy, 3 humidity, 3 wind speed, South wind direction."},
        {"2024-11-08", "18 degree, Sunny, 4 humidity, 4 wind speed, Southwest wind direction."},
        {"2024-11-09", "17 degree, Foggy, 5 humidity, 5 wind speed, West wind direction."},
        {"2024-11-10", "16 degree, Sunny, 6 humidity, 6 wind speed, Northwest wind direction."},
    },
    // 18
    std::map<std::string, std::string>{
        {"2024-11-04", "36 degree, Sunny, 10 humidity, 10 wind speed, Northeast wind direction."},
        {"2024-11-05", "35 degree, Sunny, 11 humidity, 11 wind speed, East wind direction."},
        {"2024-11-06", "34 degree, Windy, 12 humidity, 12 wind speed, Southeast wind direction."},
        {"2024-11-07", "33 degree, Rainy, 13 humidity, 13 wind speed, South wind direction."},
        {"2024-11-08", "32 degree, Sunny, 14 humidity, 14 wind speed, Southwest wind direction."},
        {"2024-11-09", "31 degree, Foggy, 15 humidity, 15 wind speed, West wind direction."},
        {"2024-11-10", "30 degree, Sunny, 16 humidity, 16 wind speed, Northwest wind direction."},
    },
    // 19
    std::map<std::string, std::string>{
        {"2024-11-04", "24 degree, Sunny, 20 humidity, 20 wind speed, Northeast wind direction."},
        {"2024-11-05", "23 degree, Sunny, 21 humidity, 21 wind speed, East wind direction."},
        {"2024-11-06", "22 degree, Windy, 22 humidity, 22 wind speed, Southeast wind direction."},
        {"2024-11-07", "21 degree, Rainy, 23 humidity, 23 wind speed, South wind direction."},
        {"2024-11-08", "20 degree, Sunny, 24 humidity, 24 wind speed, Southwest wind direction."},
        {"2024-11-09", "19 degree, Foggy, 25 humidity, 25 wind speed, West wind direction."},
        {"2024-11-10", "18 degree, Sunny, 26 humidity, 26 wind speed, Northwest wind direction."},
    },
    // 20
    std::map<std::string, std::string>{
        {"2024-11-04", "38 degree, Sunny, 30 humidity, 30 wind speed, Northeast wind direction."},
        {"2024-11-05", "37 degree, Sunny, 31 humidity, 31 wind speed, East wind direction."},
        {"2024-11-06", "36 degree, Windy, 32 humidity, 32 wind speed, Southeast wind direction."},
        {"2024-11-07", "35 degree, Rainy, 33 humidity, 33 wind speed, South wind direction."},
        {"2024-11-08", "34 degree, Sunny, 34 humidity, 34 wind speed, Southwest wind direction."},
        {"2024-11-09", "33 degree, Foggy, 35 humidity, 35 wind speed, West wind direction."},
        {"2024-11-10", "32 degree, Sunny, 36 humidity, 36 wind speed, Northwest wind direction."},
    },
    // 21
    std::map<std::string, std::string>{
        {"2024-11-04", "26 degree, Sunny, 40 humidity, 40 wind speed, Northeast wind direction."},
        {"2024-11-05", "25 degree, Sunny, 41 humidity, 41 wind speed, East wind direction."},
        {"2024-11-06", "24 degree, Windy, 42 humidity, 42 wind speed, Southeast wind direction."},
        {"2024-11-07", "23 degree, Rainy, 43 humidity, 43 wind speed, South wind direction."},
        {"2024-11-08", "22 degree, Sunny, 44 humidity, 44 wind speed, Southwest wind direction."},
        {"2024-11-09", "21 degree, Foggy, 45 humidity, 45 wind speed, West wind direction."},
        {"2024-11-10", "20 degree, Sunny, 46 humidity, 46 wind speed, Northwest wind direction."},
    },
    // 22
    std::map<std::string, std::string>{
        {"2024-11-04", "40 degree, Sunny, 50 humidity, 50 wind speed, Northeast wind direction."},
        {"2024-11-05", "39 degree, Sunny, 51 humidity, 51 wind speed, East wind direction."},
        {"2024-11-06", "38 degree, Windy, 52 humidity, 52 wind speed, Southeast wind direction."},
        {"2024-11-07", "37 degree, Rainy, 53 humidity, 53 wind speed, South wind direction."},
        {"2024-11-08", "36 degree, Sunny, 54 humidity, 54 wind speed, Southwest wind direction."},
        {"2024-11-09", "35 degree, Foggy, 55 humidity, 55 wind speed, West wind direction."},
        {"2024-11-10", "34 degree, Sunny, 56 humidity, 56 wind speed, Northwest wind direction."},
    },
    // 23
    std::map<std::string, std::string>{
        {"2024-11-04", "28 degree, Sunny, 60 humidity, 60 wind speed, Northeast wind direction."},
        {"2024-11-05", "27 degree, Sunny, 61 humidity, 61 wind speed, East wind direction."},
        {"2024-11-06", "26 degree, Windy, 62 humidity, 62 wind speed, Southeast wind direction."},
        {"2024-11-07", "25 degree, Rainy, 63 humidity, 63 wind speed, South wind direction."},
        {"2024-11-08", "24 degree, Sunny, 64 humidity, 64 wind speed, Southwest wind direction."},
        {"2024-11-09", "23 degree, Foggy, 65 humidity, 65 wind speed, West wind direction."},
        {"2024-11-10", "22 degree, Sunny, 66 humidity, 66 wind speed, Northwest wind direction."},
    }
};

#endif /* _SERVER_CONFIG_H */