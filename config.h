#ifndef SIDE_RACER_CONFIG_H
#define SIDE_RACER_CONFIG_H

#include <fstream>

class config {
private:
    char **configNames;
    char **configCharValues;
//    double *configValues;
    unsigned int numberOfConfigValues;
public:
    config();
    ~config();

    void print() const;
    char *getCharValue(const char *findConfigName) const;
    double getDoubleValue(const char *findConfigName) const;
    int getIntValue(const char *findConfigName) const;
    bool getBoolValue(const char *findConfigName) const;
    bool setValue(const char *configName, char *value); ///< Function to set value in config file. If it returns false it failed to set the value.

    bool defaultInitialization();

    void readInputFileStream(std::basic_ifstream<char, std::char_traits<char>> &configFile);
    void saveFile();
};

config *getConfig();
#endif //SIDE_RACER_CONFIG_H
