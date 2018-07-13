#pragma once
#include <list>  

enum QuadType
{
	ROOT,   
	UP_RIGHT,    
	UP_LEFT,      
	BOTTOM_LEFT,  
	BOTTOM_RIGHT  
};

template <typename T>
class QuadTreeNode
{
public:
	QuadTreeNode(float _x, float _y, float _width, float _height, int _level, int _maxLevel, QuadType _quadType, QuadTreeNode *_parent);
	~QuadTreeNode();

	void InsertObject(T *object);
	std::list<T *> GetObjectsAt(float px, float py, float w, float h); 
	void RemoveObjectsAt(float px, float py, float w, float h);  
	std::list<T *>* retrieve(std::list<T *>* returnObjects, T *object);

private:
	bool IsContain(float px, float py, float w, float h, T *object) const; 
	bool IsContain(float px, float py, float w, float h, QuadTreeNode<T> *quadTreeNode) const;   
	QuadTreeNode* getIndex(T *object);

	std::list<T *> objects;

	QuadTreeNode *parent;
	QuadTreeNode *upRightNode;
	QuadTreeNode *upLeftNode;
	QuadTreeNode *bottomLeftNode;
	QuadTreeNode *bottomRightNode;
	QuadType quadType;

	float x;
	float y;
	float width;
	float height;

	int level; 
	int maxLevel; 
};