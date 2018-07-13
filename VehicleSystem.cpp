
#include "VehicleSystem.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <time.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <iostream>

using namespace sf;

VehicleSystem::VehicleSystem(int windowWidth, int windowLength) {
	width = windowWidth;
	length = windowLength;
}

VehicleSystem::~VehicleSystem() {
}

void VehicleSystem::setup(int number) {
	for (int i = 0; i < number; i++) {
		vehicles.push_back(Vehicle(rand() % width, rand() % length));
	}
}

void VehicleSystem::draw() {
	for (int i = 0; i < vehicles.size(); i++) {
		
		vehicles[i].update(width, length);
		vehicles[i].display();
		
	}
}

void VehicleSystem::applyBehavior(int toggleForce) {
	
	for (int i = 0; i < vehicles.size(); i++) {
		
		Vector2f separate(0, 0);
		Vector2f align(0, 0);
		Vector2f cohesion(0, 0);
		if(toggleForce&1) separate = vehicles[i].separationForce();
		if(toggleForce & 2) align = vehicles[i].alignForce();
		if (toggleForce & 4) cohesion = vehicles[i].cohesionForce();
			
		
		separate *= 1.4f;
		align *= 0.8f;
		cohesion *= 1.2f;
		Vector2f sum = separate + align + cohesion;
		vehicles[i].applyForce(sum);
		
		//if (sum.x != 0)std::cout << i<< " "<< vehicles[i].velocity.x << " " << vehicles[i].velocity.y << std::endl;
		
	}
}

void VehicleSystem::addVehicle(Vector2f pos) {
	vehicles.push_back(Vehicle(pos.x,pos.y));
}

