#pragma once
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
#include <list>

using namespace sf;

Vector2f Limit(Vector2f a, float max);
Vector2f Normalize(Vector2f a);
float Dist(Vector2f a, Vector2f b);

class Vehicle {

	float r; // for size
	float maxforce;
	float maxspeed;

public:

	Vector2f location;
	Vector2f velocity;
	Vector2f acceleration;
	ConvexShape triangle;
	
	// variables for quadtree
	float x;
	float y;
	float width;
	float height;

	std::list<Vehicle *> collideObjects;
	std::list<float> collideObjectsDist;

	Vehicle(float posX, float posY);
	~Vehicle();

	void update(int windowWidth, int windowLength);
	void applyForce(Vector2f force);
	Vector2f seek(Vector2f target);
	void display();
	Vector2f separationForce();
	Vector2f alignForce();
	Vector2f cohesionForce();
};