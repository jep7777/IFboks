#include "Badanalyse.h"

Badanalyse::Badanalyse() : 
    latestTemp_(0.0), latestVolume_(0.0), totalWater_(0.0), totalEnergy_(0.0) {}

void Badanalyse::updateLatestTemperature(double newTemperature){
    latestTemp_ = newTemperature;
}

void Badanalyse::updateLatestVolume(double newVolume){
    latestVolume_ = newVolume;
}

void Badanalyse::updateTotalWater(){
    totalWater_ += latestVolume_;
}

void Badanalyse::updateTotalEnergy(){
    //heat capacity of water is 0.001162 kWh per kg per Kelvin
    //we assume 1 L is 1 kg
    //we also assume the powerplant heated the water up from 7 degree celsius
    double energyToAdd = (latestTemp_ - 7.0) * latestVolume_ * 0.001162;
    if(energyToAdd > 0.0){
        totalEnergy_ += energyToAdd;
    }
}



double Badanalyse::getLatestTemp() const{
    return latestTemp_;
}


double Badanalyse::getTotalWater() const{
    return totalWater_;
}

double Badanalyse::getTotalEnergy() const{
    return totalEnergy_;
}