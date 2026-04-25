#pragma once

class Brugerindstillinger{
    public:
        Brugerindstillinger();

        void incTempMaxWater();
        void decTempMaxWater();
        void incTempMaxEnergy();
        void decTempMaxEnergy();


        void resetTempSettings();
        void saveTempSettings();

        //getters
        double getMaxEnergy() const;
        int getMaxWater() const;
        double getTempMaxEnergy() const;
        double getTempMaxWater() const;

    private:
        double maxEnergy_;      //in kWh
        int maxWater_;          //in L
        double tempMaxEnergy_;
        double tempMaxWater_;

};