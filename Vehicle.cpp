
#include "Vehicle.h"
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

Vector2f Limit(Vector2f a, float max) {
	float length = sqrtf(a.x*a.x + a.y*a.y);
	if (length > max && length != 0 ) {
		return Vector2f(max / length * a.x, max / length * a.y);
	}
	else {
		return a;
	}
}

Vector2f Normalize(Vector2f a) {
	float length = sqrtf(a.x*a.x + a.y*a.y);
	if (length != 0) {
		return Vector2f(1.f / length * a.x, 1.f / length * a.y);
	}
	else {
		return a;
	}
}


Vehicle::Vehicle(float posX, float posY) {
	acceleration = Vector2f(0, 0);
	velocity = Vector2f(-rand()%10+5, -rand()%10+5);
	location = Vector2f(posX, posY);

	r = 3; // define triangle's size
	maxspeed = 0.5;//0.1;
	maxforce = 0.003;//0.0005;
	triangle.setPointCount(3);
	triangle.setOutlineColor(Color::Red);
	triangle.setOutlineThickness(5);
	triangle.setFillColor(Color::Red);

	x = posX - r;
	y = posY - r;
	width = 2 * r;
	height = 2 * r;

}

Vehicle::~Vehicle() {
}

void Vehicle::update(int windowWidth, int windowLength) {
	velocity += acceleration;
	velocity = Limit(velocity, maxspeed);
	location += velocity;

	if (location.x < 0)location.x = windowWidth;
	if (location.x > windowWidth)location.x = 0;
	if (location.y < 0)location.y = windowLength;
	if (location.y > windowLength)location.y = 0;
	acceleration = Vector2f(0, 0);

	x = location.x - r;
	y = location.y - r;

}

void Vehicle::applyForce(Vector2f force) {
	acceleration += force;
}

Vector2f Vehicle::seek(Vector2f target) {
	Vector2f desired = target - location;

	// set arriving behavior
	float d = sqrtf(desired.x*desired.x+ desired.y*desired.y);
	desired = Normalize(desired);
	if (d < 80) {
		//set the magnitude according to how close we are.
		float m = d / 80 * maxspeed;
		desired *= m;
	}
	else {
		//Otherwise, proceed at maximum speed.
		desired *= maxspeed;
	}

	Vector2f steer = desired - velocity;
	steer = Limit(steer, maxforce);
	return steer;
	
}

void Vehicle::display() {

	Vector2f unit = Normalize(velocity) * r;
	triangle.setPoint(0, unit * 2.f);
	triangle.setPoint(1, Vector2f(-unit.x*2.f + unit.y, -unit.y*2.f - unit.x));
	triangle.setPoint(2, Vector2f(-unit.x*2.f - unit.y, -unit.y*2.f + unit.x));
	
	triangle.setPosition(location);
	
}

Vector2f Vehicle::separationForce() {
	float desiredSeparation = r*30;
	Vector2f sum(0, 0);
	int count = 0;
	
	auto iter2 = collideObjectsDist.begin();
	for (auto iter = collideObjects.begin(); iter != collideObjects.end(); iter++)
	{
		
		Vehicle * t = *iter;
		float d = *iter2;
		if (d > 0 && d < desiredSeparation) {
			Vector2f diff = location - t->location;
			diff = Normalize(diff);
			diff /= d;
			sum += diff;
			count++;
		}
		iter2++;
	}
	Vector2f steer(0, 0);
	if (count > 0) {
		sum /= (float)count;
		sum = Normalize(sum);
		sum *= maxspeed;
		steer = sum - velocity;
		steer = Limit(steer, maxforce);
	}
	return steer;
}

Vector2f Vehicle::alignForce() {
	float desiredAlign = r*30;
	Vector2f sum(0, 0);
	int count = 0;
	
	auto iter2 = collideObjectsDist.begin();
	for (auto iter = collideObjects.begin(); iter != collideObjects.end(); iter++)
	{
		Vehicle * t = *iter;
		float d = *iter2;
		if (d > 0 && d < desiredAlign) {
			sum += t->velocity;
			count++;
		}
		iter2++;
	}
	Vector2f steer(0, 0);
	if (count > 0) {
		sum /= (float)count;
		sum = Normalize(sum);
		sum *= maxspeed;
		steer = sum-velocity;
		steer = Limit(steer, maxforce);
	}
	return steer;
}

Vector2f Vehicle::cohesionForce() {
	float desiredCohesion = r * 30;
	Vector2f sum(0, 0);
	int count = 0;
	
	auto iter2 = collideObjectsDist.begin();
	for (auto iter = collideObjects.begin(); iter != collideObjects.end(); iter++)
	{
		Vehicle * t = *iter;
		float d = *iter2;
		if (d > 0 && d < desiredCohesion) {
			sum += t->location;
			count++;
		}
		iter2++;
	}

	Vector2f steer(0, 0);
	if (count > 0) {
		sum /= (float)count;
		steer = seek(sum);
	}
	return steer;
}