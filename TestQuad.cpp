//#include "QuadTreeNode.h"
//#include "QuadTreeNode.cpp"  
//#include "Vehicle.h"
//
//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
//#include <SFML/System.hpp>
//#include <SFML/OpenGL.hpp>
//#include <SFML/Main.hpp>
//#include <time.h>
//#include <cmath>
//#include <ctime>
//#include <cstdlib>
//#include <sstream>
//#include <cstring>
//#include <iostream>
//#include <queue>
//
//using namespace std;
//
//
//
//int main() {
//
//	
//
//	int windowWidth = 200;
//	int windowHeight = 200;
//	//sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "TestQuad");
//
//	QuadTreeNode<Vehicle> *quadTree = new QuadTreeNode<Vehicle>(0, 0, windowWidth, windowHeight, 1, 5, ROOT, nullptr);
//
//	quadTree->InsertObject(new Vehicle(101, 101));
//	quadTree->InsertObject(new Vehicle(1, 100));
//	quadTree->InsertObject(new Vehicle(25, 25));
//	quadTree->InsertObject(new Vehicle(62.5, 12.5));
//	quadTree->InsertObject(new Vehicle(62.5, 62.5));
//	quadTree->InsertObject(new Vehicle(63.5, 63.6));
//	quadTree->InsertObject(new Vehicle(125, 25));
//	quadTree->InsertObject(new Vehicle(112.5, 62.5));
//
//	//quadTree->RemoveObjectsAt(100, 0, 110, 110);
//	//list<Vehicle *> resObjects = quadTree->GetObjectsAt(0, 0, 50, 51);
//	//cout << quadTree->getIndex(new Vehicle(104, 104)) << endl;
//	list<Vehicle *> resObjects;
//	quadTree->retrieve(&resObjects, new Vehicle(62.5, 62.5));
//
//	delete quadTree;
//
//	
//
//
//	return 0;
//
//}