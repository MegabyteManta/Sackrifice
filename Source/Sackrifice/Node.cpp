// Fill out your copyright notice in the Description page of Project Settings.


#include "Node.h"

Node::Node(bool _Walkable, FVector _WorldPosition, int _X, int _Y)
{
	Walkable = _Walkable;
	WorldPosition = _WorldPosition;
	X = _X;
	Y = _Y;
	GridPos = FVector2D(X, Y);
}

Node::Node()
{
}

Node::~Node()
{
	//parent = nullptr;
}
