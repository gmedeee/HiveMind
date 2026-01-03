#include "../include/Agent.h"

// Drone
const char Drone::symbol = '^';
int Drone::speed = 3;
int Drone::batteryMax = 100;
int Drone::consumption = 10;
int Drone::cost = 15;
int Drone::capacity = 1;

const char Drone::getSymbol() const
{
    return Drone::symbol;
}
int Drone::getSpeed() const
{
    return Drone::speed;
}
int Drone::getBatteryMax() const
{
    return Drone::batteryMax;
}
int Drone::getConsumption() const
{
    return Drone::consumption;
}
int Drone::getCost() const
{
    return Drone::cost;
}
int Drone::getCapacity() const
{
    return Drone::capacity;
}
bool Drone::canTraverse(char) const
{
    return true; // ignora #
}

// Robot
const char Robot::symbol = 'R';
int Robot::speed = 1;
int Robot::batteryMax = 300;
int Robot::consumption = 2;
int Robot::cost = 1;
int Robot::capacity = 4;

const char Robot::getSymbol() const
{
    return Robot::symbol;
}
int Robot::getSpeed() const
{
    return Robot::speed;
}
int Robot::getBatteryMax() const
{
    return Robot::batteryMax;
}
int Robot::getConsumption() const
{
    return Robot::consumption;
}
int Robot::getCost() const
{
    return Robot::cost;
}
int Robot::getCapacity() const
{
    return Robot::capacity;
}
bool Robot::canTraverse(char cell) const
{
    return cell != '#'; // atata timp cat != # return true, altfel false, nu pot trece peste pereti
}

// Scooter
const char Scooter::symbol = 'S';
int Scooter::speed = 2;
int Scooter::batteryMax = 200;
int Scooter::consumption = 5;
int Scooter::cost = 4;
int Scooter::capacity = 2;

const char Scooter::getSymbol() const
{
    return Scooter::symbol;
}
int Scooter::getSpeed() const
{
    return Scooter::speed;
}
int Scooter::getBatteryMax() const
{
    return Scooter::batteryMax;
}
int Scooter::getConsumption() const
{
    return Scooter::consumption;
}
int Scooter::getCost() const
{
    return Scooter::cost;
}
int Scooter::getCapacity() const
{
    return Scooter::capacity;
}
bool Scooter::canTraverse(char cell) const
{
    return cell != '#';
}
