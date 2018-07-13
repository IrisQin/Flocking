#include "QuadTreeNode.h"  


template <typename T>
QuadTreeNode<T>::QuadTreeNode(
	float _x, float _y, float _width, float _height,
	int _level, int _maxLevel,
	QuadType _quadType,
	QuadTreeNode *_parent) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	level(_level),
	maxLevel(_maxLevel),
	quadType(_quadType)
{
	parent = _parent;
	upRightNode = nullptr;
	upLeftNode = nullptr;
	bottomLeftNode = nullptr;
	bottomRightNode = nullptr;
}

template <typename T>
QuadTreeNode<T>::~QuadTreeNode()
{
	if (level == maxLevel)
		return; 
	parent = nullptr;

}

template <typename T>
bool QuadTreeNode<T>::IsContain(float px, float py, float w, float h, T *object) const
{
	if (object->x >= px
		&&object->x + object->width <= px + w
		&&object->y >= py
		&&object->y + object->height <= py + h)
		return true;
	return false;
}

template <typename T>
bool QuadTreeNode<T>::IsContain(float px, float py, float w, float h, QuadTreeNode<T> *quadTreeNode) const
{
	if (quadTreeNode->x >= px
		&&quadTreeNode->x + quadTreeNode->width <= px + w
		&&quadTreeNode->y >= py
		&&quadTreeNode->y + quadTreeNode->height <= py + h)
		return true;
	return false;
}

template <typename T>
void QuadTreeNode<T>::InsertObject(T *object)
{ 
	if (level == maxLevel)
	{
		objects.push_back(object);
		return;
	}

	if (IsContain(x + width / 2, y, width / 2, height / 2, object))
	{
		if (!upRightNode) 
			upRightNode = new QuadTreeNode(x + width / 2, y, width / 2, height / 2, level + 1, maxLevel, UP_RIGHT, this);
		upRightNode->InsertObject(object);
		return;
	}
	else if (IsContain(x, y, width / 2, height / 2, object))
	{
		if (!upLeftNode)
			upLeftNode = new QuadTreeNode(x, y, width / 2, height / 2, level + 1, maxLevel, UP_LEFT, this);
		upLeftNode->InsertObject(object);
		return;
	}
	else if (IsContain(x, y + height / 2, width / 2, height / 2, object))
	{
		if (!bottomLeftNode)
			bottomLeftNode = new QuadTreeNode(x, y + height / 2, width / 2, height / 2, level + 1, maxLevel, BOTTOM_LEFT, this);
		bottomLeftNode->InsertObject(object);
		return;
	}
	else if (IsContain(x + width / 2, y + height / 2, width / 2, height / 2, object))
	{
		if (!bottomRightNode)
			bottomRightNode = new QuadTreeNode(x + width / 2, y + height / 2, width / 2, height / 2, level + 1, maxLevel, BOTTOM_RIGHT, this);
		bottomRightNode->InsertObject(object);
		return;
	}

	if (IsContain(x, y, width, height, object))
		objects.push_back(object);
}

template <typename T>
std::list<T *> QuadTreeNode<T>::GetObjectsAt(float px, float py, float w, float h)
{
	std::list<T *> resObjects;

	if (IsContain(px, py, w, h, this))
	{
		resObjects.insert(resObjects.end(), objects.begin(), objects.end()); 
		if (level == maxLevel)
			return resObjects;
	}
 
	if (upRightNode)
	{
		std::list<T *> upRightChild;
		upRightChild = upRightNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), upRightChild.begin(), upRightChild.end());
	}
	if (upLeftNode)
	{
		std::list<T *> upLeftChild;
		upLeftChild = upLeftNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), upLeftChild.begin(), upLeftChild.end());
	}
	if (bottomLeftNode)
	{
		std::list<T *> bottomLeftChild;
		bottomLeftChild = bottomLeftNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), bottomLeftChild.begin(), bottomLeftChild.end());
	}
	if (bottomRightNode)
	{
		std::list<T *> bottomRightChild;
		bottomRightChild = bottomRightNode->GetObjectsAt(px, py, w, h);
		resObjects.insert(resObjects.end(), bottomRightChild.begin(), bottomRightChild.end());
	}
	return resObjects;
}

template <typename T>
void QuadTreeNode<T>::RemoveObjectsAt(float px, float py, float w, float h)
{
	if (IsContain(px, py, w, h, this))
	{
		objects.clear();
		if (level == maxLevel)
			return;

	}

	if (upRightNode&&IsContain(px, py, w, h, upRightNode))
	{
		upRightNode->RemoveObjectsAt(px, py, w, h);
		delete upRightNode;
		upRightNode = nullptr;

	}
	if (upLeftNode&&IsContain(px, py, w, h, upLeftNode))
	{
		upLeftNode->RemoveObjectsAt(px, py, w, h);
		delete upLeftNode;
		upLeftNode = nullptr;

	}
	if (bottomLeftNode&&IsContain(px, py, w, h, bottomLeftNode))
	{
		bottomLeftNode->RemoveObjectsAt(px, py, w, h);
		delete bottomLeftNode;
		bottomLeftNode = nullptr;

	}
	if (bottomRightNode&&IsContain(px, py, w, h, bottomRightNode))
	{
		bottomRightNode->RemoveObjectsAt(px, py, w, h);
		delete bottomRightNode;
		bottomRightNode = nullptr;
	}
}

template <typename T>
QuadTreeNode<T>* QuadTreeNode<T>::getIndex(T *object) {
	
	QuadTreeNode* index = nullptr;
	double verticalMidpoint = x + width / 2;
	double horizontalMidpoint = y + height / 2;

	boolean topQuadrant = (object->y < horizontalMidpoint && object->y + object->height < horizontalMidpoint);
	boolean bottomQuadrant = (object->y > horizontalMidpoint);

	if (object->x < verticalMidpoint && object->x + object->width < verticalMidpoint) {
		if (topQuadrant) {
			index = upLeftNode;
		}
		else if (bottomQuadrant) {
			index = bottomLeftNode; 
		}
	}

	else if (object->x > verticalMidpoint) {
		if (topQuadrant) {
			index = upRightNode;
		}
		else if (bottomQuadrant) {
			index = bottomRightNode;
		}
	}

	return index;
}

template <typename T>
std::list<T *>* QuadTreeNode<T>::retrieve(std::list<T *>* returnObjects, T *object) {
	
	QuadTreeNode* index = getIndex(object);
	if (index) {
		index->retrieve(returnObjects, object);
	}

	returnObjects->insert(returnObjects->end(), objects.begin(),objects.end());

	return returnObjects;
}