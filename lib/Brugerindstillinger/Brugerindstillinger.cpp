#include "Brugerindstillinger.h"

Brugerindstillinger::Brugerindstillinger() :
    maxEnergy_(0.0), maxWater_(0), tempMaxEnergy_(0.0), tempMaxWater_(0) {}

void Brugerindstillinger::incTempMaxWater(){
    if(tempMaxWater_ < 99){
        tempMaxWater_++;
    }
}

void Brugerindstillinger::decTempMaxWater(){
    if(tempMaxWater_ > 0){
        tempMaxWater_--;
    }
}


void Brugerindstillinger::incTempMaxEnergy(){
    if(tempMaxEnergy_ < 99.9){
        tempMaxEnergy_ += 0.1;
    }
}

void Brugerindstillinger::decTempMaxEnergy(){
    if(tempMaxEnergy_ > 0.0){
        tempMaxEnergy_ -= 0.1;
        if(tempMaxEnergy_ < 0.0){ //to make sure it doesn't become negative
            tempMaxEnergy_ = 0.0;
        }
    }
}

void Brugerindstillinger::resetTempSettings(){
    tempMaxEnergy_ = maxEnergy_;
    tempMaxWater_ = maxWater_;
}

void Brugerindstillinger::saveTempSettings(){
    maxEnergy_ = tempMaxEnergy_;
    maxWater_ = tempMaxWater_;
}

bool Brugerindstillinger::checkMaxWaterExceeded(double totalWater) const{
    return(totalWater > static_cast<double>(maxWater_));
}

bool Brugerindstillinger::checkMaxEnergyExceeded(double totalEnergy) const{
    return(totalEnergy > maxEnergy_);
}

double Brugerindstillinger::getMaxEnergy() const {
    return maxEnergy_;
}

int Brugerindstillinger::getMaxWater() const {
    return maxWater_;
}

double Brugerindstillinger::getTempMaxEnergy() const {
    return tempMaxEnergy_;
}

double Brugerindstillinger::getTempMaxWater() const {
    return tempMaxWater_;
}


