// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CostVector.h"
#include "Node.h"

/**
 * Node of a Grid.
 */
class SACKRIFICE_API Node
{
public:
	Node(bool _Walkable, FVector _WorldPosition, int _X, int _Y);
	Node();
	~Node();

	//Is the node walkable?
	bool Walkable;
	
	//Position in World Space
	FVector WorldPosition;// = FVector(99999, 99999, 99999);

	//Position (X,Y) on the grid
	int X;
	int Y;
	FVector2D GridPos;

	//Cost of the Node (contains FCost, HCost, and GCost)
	CostVector Cost;

	
	//Node* parent;

	//For path reconstruction
	FVector ParentPos;// = FVector(99999, 99999, 99999);
	bool HasParent = false;

	//Node is less than if the cost vector is less
	//Node is equal if the world positions are the same
	//There can never be 2 nodes at 1 world position
	friend bool operator< (const Node &c1, const Node &c2);
	friend bool operator== (const Node &c1, const Node &c2);
	friend bool operator!= (const Node &c1, const Node &c2);
	friend uint32 GetTypeHash(const Node &c);
};

//This was SOME SHIT, defining a hash function for TSet
FORCEINLINE
uint32 GetTypeHash(const Node &c) {
	return GetTypeHash(c.WorldPosition);
}

FORCEINLINE
bool operator< (const Node &c1, const Node &c2) {
	return c1.Cost < c2.Cost;
}

FORCEINLINE
bool operator== (const Node &c1, const Node &c2) {
	return c1.WorldPosition == c2.WorldPosition;
}

FORCEINLINE
bool operator!= (const Node &c1, const Node &c2) {
	return c1.WorldPosition != c2.WorldPosition;
}