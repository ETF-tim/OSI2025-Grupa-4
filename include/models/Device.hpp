#pragma once
#include <string>

enum class State
{
    IN_REPAIR,
    WAITING_FOR_PARTS,
    DIAGNOSIS,
    CLIENT_APPROVAL,
    REPAIRED,
    CANNOT_BE_REPAIRED,
    RETURNED_TO_CLIENT,
    CANCELED
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
    Device()=default;
    Device(int id,const std::string& brand,const std::string& model,
        const std::string& IMEI,State state)
        :id(id),brand(brand),model(model),IMEI(IMEI),state(state){}

        int getId() const noexcept{return id;}
        void setId(int v) noexcept{id=v;}

        const std::string& getBrand() const noexcept{return brand;}
        void setBrand(const std::string& v) noexcept{brand=v;}

        const std::string& getModel() const noexcept{return model;}
        void setModel(const std::string& v) noexcept{model=v;}

        const std::string& getIMEI() const noexcept{return IMEI;}
        void setIMEI(const std::string& v) noexcept{IMEI=v;}

        State getState() const noexcept{return state;}
        void setState(State v) noexcept{state=v;}
};

bool isValidId(int id);
bool isValidBrand(const std::string& brand);
bool isValidModel(const std::string& model);
bool isValidIMEI(const std::string& IMEI);
bool isValidState(State state);