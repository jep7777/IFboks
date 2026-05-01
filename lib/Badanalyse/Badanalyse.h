#pragma once

class Badanalyse{
    public:
        Badanalyse();        

        void updateLatestTemperature(double);
        void updateLatestVolume(double);
        void updateTotalWater();
        void updateTotalEnergy();

        double getLatestTemp() const;
        double getTotalWater() const;
        double getTotalEnergy() const;
    private:
        double latestTemp_;
        double latestVolume_;
        double totalWater_;
        double totalEnergy_;



};