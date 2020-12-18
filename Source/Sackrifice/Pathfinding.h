// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Node.h"
#include "Grid.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Pathfinding.generated.h"

UCLASS()
class SACKRIFICE_API APathfinding : public AActor
{
	GENERATED_BODY()

	//Create path from Seeker to Target and save it under Grid->Path
	//O(P) where P is the number of nodes in the path
	TArray<FVector> RetracePath(Node StartNode, Node EndNode);

	//Get distance between nodes
	//O(c)
	int GetDistance(Node NodeA, Node NodeB);

	//Get waypoints
	TArray<FVector> SimplifyPath(TArray<Node> Path);

public:	
	// Sets default values for this actor's properties
	APathfinding();

	//Reference to the Grid in scene
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGrid* Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDebug = false;

	/*
	//Seeker seeks Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Seeker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target;
	*/

	//Find shortest path from Seeker to Target
	//O(NlogN) N nodes, logN to find largest node in heap
	TArray<FVector> FindPath(FVector StartPos, FVector TargetPos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
