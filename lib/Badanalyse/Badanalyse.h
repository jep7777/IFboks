#pragma once

class Badanalyse{
    public:
        Badanalyse();        

        void updateLatestTemperature(double);
        void updateLatestVolume(double);
        void updateTotalWater();
        void updateLatestFlowRate(double);
        void updateTotalEnergy();
        void resetValues();

        double getLatestTemp() const;
        double getLatestFlowRate() const;
        double getTotalWater() const;
        double getTotalEnergy() const;
    private:
        double latestTemp_;
        double latestVolume_;
        double latestFlowRate_;
        double totalWater_;
        double totalEnergy_;



};