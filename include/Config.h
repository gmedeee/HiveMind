#pragma once

class Config {
public:
    int mapW, mapH;
    int maxTicks;
    int maxStations;
    int clientsCount;

    int drones;
    int robots;
    int scooters;

    int totalPackages;
    int spawnFrequency;

    Config() {
        //valori default in cazul in care nu este citit bine fisierul corect
        mapW = 20; mapH = 20;
        maxTicks = 1000;
        maxStations = 3;
        clientsCount = 10;

        drones = 1; robots = 1; scooters = 1;

        totalPackages = 50;
        spawnFrequency = 10;
    }
};
