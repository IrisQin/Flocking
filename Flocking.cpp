
#include "Vehicle.h"
#include "VehicleSystem.h"
#include "QuadTreeNode.h"
#include "QuadTreeNode.cpp"  
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

using namespace std;

float Dist(sf::Vector2f a, sf::Vector2f b) {
	return sqrtf(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

string int2Word(int a) {
	if (a) return "ON";
	return "OFF";
}

int main() {
	
	int windowWidth = 1024;
	int windowHeight = 512;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Flocking");
	
	VehicleSystem vs(windowWidth, windowHeight);
	vs.setup(20);
	int toggleForce = 0;

	sf::Font font;
	if (!font.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;
	sf::Text vehicleNumberMessage;
	vehicleNumberMessage.setFont(font);
	vehicleNumberMessage.setCharacterSize(20);
	vehicleNumberMessage.setPosition(windowWidth/35, windowHeight/35);
	vehicleNumberMessage.setFillColor(sf::Color::Green);
	
	
	
	while (window.isOpen()) {

		QuadTreeNode<Vehicle> *quadTree = new QuadTreeNode<Vehicle>(0, 0, windowWidth, windowHeight, 1, 4, ROOT, nullptr);
		for (int i = 0; i < vs.vehicles.size(); i++) {
			quadTree->InsertObject(&vs.vehicles[i]);
		}

		// use quadTree to calculate other possible collide vehicles and distance from them 
		for (int i = 0; i < vs.vehicles.size(); i++) {
			vs.vehicles[i].collideObjects.clear();
			vs.vehicles[i].collideObjectsDist.clear();

			list<Vehicle *> resObjects;
			quadTree->retrieve(&resObjects, &vs.vehicles[i]);
			vs.vehicles[i].collideObjects = resObjects;

			for (auto iter = vs.vehicles[i].collideObjects.begin(); iter != vs.vehicles[i].collideObjects.end(); iter++)
			{
				Vehicle * t = *iter;
				vs.vehicles[i].collideObjectsDist.push_back(Dist(vs.vehicles[i].location, t->location));
			}
		}
		

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == Event::MouseButtonReleased) {
				vs.addVehicle(sf::Vector2f((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y));
			}
			// bits in "toggleforce" control each force's on/off. from right: 1st bit separate, 2ed bit align, 3rd bit cohesion
			if (event.type == Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::S) toggleForce ^= 1;
				if (event.key.code == sf::Keyboard::A) toggleForce ^= 1 << 1;
				if (event.key.code == sf::Keyboard::C) toggleForce ^= 1 << 2;
			}
			
		}

		vs.applyBehavior(toggleForce);
		vs.draw();
		window.clear();
		for (int i = 0; i < vs.vehicles.size(); i++) {
			window.draw(vs.vehicles[i].triangle);
		}
		vehicleNumberMessage.setString("Vehicle number: " + to_string(vs.vehicles.size()) + "\n" + "Separation: " + int2Word(toggleForce&1) + "\n" + "Alignment: " + int2Word(toggleForce & 2) + "\n" + "Cohesion: " + int2Word(toggleForce & 4));
		window.draw(vehicleNumberMessage);
		window.display();
		quadTree->RemoveObjectsAt(0, 0, windowWidth, windowHeight);
		delete quadTree;
	}

	
	return 0;

}