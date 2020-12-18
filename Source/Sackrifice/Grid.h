// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Node.h"
#include "GameFramework/Actor.h" 
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h" 
#include "DrawDebugHelpers.h"
#include "Grid.generated.h"

UCLASS()
class SACKRIFICE_API AGrid : public AActor
{
	GENERATED_BODY()

	//Diameter of a node (should probs use this instead of node radius)
	float NodeDiameter;

	//Number of nodes in grid
	int GridSizeX;
	int GridSizeY;

	//Creates the grid lol
	//O(N*X) where N = number of Nodes and X is number of components in scene
	void CreateGrid();
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//UStaticMeshComponent* Mesh;

	//A Grid is a 2D array of Nodes
	//We can overload [] later
	TArray<TArray<Node>> grid;
	
	//Tag to give to actors to make them walkable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WalkableTag;

	//Tag to give to actors to make them unwalkable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName UnWalkableTag;

	//Size of the grid (X,Y) in world units
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D GridWorldSize;

	//Radius of a node in world units
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NodeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDebug = false;

	//Path from seeker to target
	TArray<Node> Path;
	
	//Get the neighbors of a node (including diagonal)
	//O(c)
	TArray<Node> GetNeighbors(Node CurrentNode);

	//Get a node given a world position
	//O(c)
	Node NodeFromWorldPoint(FVector WorldPosition);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
