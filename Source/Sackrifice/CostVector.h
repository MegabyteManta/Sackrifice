// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Calculate the cost of a node.
 * HCost: Cost from node to target
 * GCost: Cost from node to start
 * FCost: Sum of HCost and GCost
 */
class SACKRIFICE_API CostVector
{
public:
	CostVector(int _HCost = INFINITY, int _GCost = INFINITY);
	~CostVector();

	//You can index into the CostVector because I'm dumb and stupid
	//Just do .FCost, .HCost, or .GCost
	int Costs[3];
	int FCost;
	int HCost;
	int GCost;

	//A cost is greater than another if FCost is greater or if
	//the FCosts are the same and the HCost is greater.
	//Reverse for less than, equal if FCost and HCost are equal
	int& operator[] (const int index);
	friend bool operator> (const CostVector &c1, const CostVector &c2);
	friend bool operator>= (const CostVector &c1, const CostVector &c2);
	friend bool operator< (const CostVector &c1, const CostVector &c2);
	friend bool operator<= (const CostVector &c1, const CostVector &c2);
	friend bool operator== (const CostVector &c1, const CostVector &c2);
	friend bool operator!= (const CostVector &c1, const CostVector &c2);
};

FORCEINLINE CostVector::CostVector(int _HCost, int _GCost)
	: FCost{ _HCost + _GCost }, HCost{ _HCost }, GCost{ _GCost }
{
	Costs[0] = _HCost + _GCost;
	Costs[1] = _HCost;
	Costs[2] = _GCost;
};

FORCEINLINE CostVector::~CostVector()
{
}


FORCEINLINE
int& CostVector::operator[] (const int index)
{
	return Costs[index];
}

FORCEINLINE
bool operator> (const CostVector &c1, const CostVector &c2) {
	return c1.FCost > c2.FCost || c1.FCost == c2.FCost && c1.HCost > c2.HCost;
}

FORCEINLINE
bool operator>= (const CostVector &c1, const CostVector &c2) {
	return c1.FCost > c2.FCost || c1.FCost == c2.FCost && c1.HCost >= c2.HCost;
}

FORCEINLINE
bool operator< (const CostVector &c1, const CostVector &c2) {
	return c1.FCost < c2.FCost || c1.FCost == c2.FCost && c1.HCost < c2.HCost;
}

FORCEINLINE
bool operator<= (const CostVector &c1, const CostVector &c2) {
	return c1.FCost < c2.FCost || c1.FCost == c2.FCost && c1.HCost <= c2.HCost;
}

FORCEINLINE
bool operator== (const CostVector &c1, const CostVector &c2) {
	return c1.FCost == c2.FCost && c1.HCost == c2.HCost;
}

FORCEINLINE
bool operator!= (const CostVector &c1, const CostVector &c2) {
	return c1.FCost == c2.FCost && c1.HCost == c2.HCost;
}