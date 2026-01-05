#pragma once
#include <string>

enum class State
{
    UNKNOWN,
    NEW,
    USED
};

class Device
{
private:
    int id;
    std::string brand;
    std::string model;
    std::string IMEI;
    State state;

public:
    Device();
    Device(int id, const std::string &brand, const std::string &model,
           const std::string &IMEI, State state);

    int getId() const;
    const std::string &getBrand() const;
    const std::string &getModel() const;
    const std::string &getIMEI() const;
    State getState() const;

    void setId(int v);
    void setBrand(const std::string &v);
    void setModel(const std::string &v);
    void setIMEI(const std::string &v);
    void setState(State v);
};

bool isValidId(int id);
bool isValidBrand(const std::string &brand);
bool isValidModel(const std::string &model);
bool isValidIMEI(const std::string &deviceIMEI);
bool isValidState(State state);