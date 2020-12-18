// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	NodeDiameter = NodeRadius * 2;
	//Round up
	GridSizeX = FMath::RoundHalfFromZero(GridWorldSize.X / NodeDiameter);
	GridSizeY = FMath::RoundHalfFromZero(GridWorldSize.Y / NodeDiameter);
	this->CreateGrid();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrid::CreateGrid()
{
	//Create grid
	TArray<Node> NodeArray;
	NodeArray.Init(Node(), GridSizeY);
	grid.Init(NodeArray, GridSizeX);

	//Node positions are built off the bottom left
	FVector WorldBottomLeft = GetActorLocation() - FVector(1, 0, 0) * GridWorldSize.X / 2 - FVector(0, 1, 0) * GridWorldSize.Y / 2;

	//Go through every node
	for (int y = 0; y < GridSizeY; y++) {
		for (int x = 0; x < GridSizeX; x++) {

			//Calculate the position of the node
			FVector WorldPoint = WorldBottomLeft + FVector(1, 0, 0) * (x * NodeDiameter + NodeRadius) + FVector(0, 1, 0) * (y * NodeDiameter + NodeRadius);
			
			//Sphere collision for overlapping actors, the only thing that matters here is OutActors
			TArray<AActor*> IgnoreActors;
			TArray <TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TArray<AActor*> OutActors;
			UKismetSystemLibrary::SphereOverlapActors((UObject*)GetWorld(), WorldPoint, NodeRadius, ObjectTypes, NULL, IgnoreActors, OutActors);
			
			//Debug boxes
			FColor Colour = FColor::Red;

			//Currently, nodes are unwalkable until made walkable
			bool Walkable = true;
			for (AActor* Actor : OutActors) {
				//if (Actor->ActorHasTag(WalkableTag)) {
					//Walkable = true;
				//	Colour = FColor::Purple;
				//	break;
				//}
				if (Actor->ActorHasTag(UnWalkableTag)) {
					Walkable = false;
					Colour = FColor::Red;
					break;
				}
			}
			//Test out the direction of nodes, there's something wrong here lol
			if (x == 0 && (y == 0 || y == 1)) {
				Colour = FColor::White;
			}
			//More debug box stuff
			float SizeFactor = 1.0f;
			if (bDebug) DrawDebugBox(GetWorld(), WorldPoint, FVector(NodeRadius*SizeFactor, NodeRadius*SizeFactor, NodeRadius*SizeFactor), Colour, true, -1, 0, 10);
			
			//Save the node in the grid
			grid[x][y] = Node(Walkable, WorldPoint, x, y);
		}

	}
}

TArray<Node> AGrid::GetNeighbors(Node CurrentNode) {
	TArray<Node> Neighbors;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			//Ignore the current node
			if (x == 0 && y == 0) {
				continue;
			}

			//Calculate (X,Y) position in grid of neighbor
			int CheckX = CurrentNode.GridPos.X + x;
			int CheckY = CurrentNode.GridPos.Y + y;
		
			//Add to Neighbors array if not outside of grid
			if (CheckX >= 0 && CheckX < GridSizeX && CheckY >= 0 && CheckY < GridSizeY) {
				Neighbors.Add(grid[CheckX][CheckY]);
			}
		}
	}
	return Neighbors;
}

//Calculates how far in a position is using percentages, then returns the corresponding node
Node AGrid::NodeFromWorldPoint(FVector WorldPosition) {
	float PercentX = FMath::Clamp((WorldPosition.X - GetActorLocation().X + GridWorldSize.X / 2) / GridWorldSize.X, 0.0f, 1.0f);
	float PercentY = FMath::Clamp((WorldPosition.Y - GetActorLocation().Y + GridWorldSize.Y / 2) / GridWorldSize.Y, 0.0f, 1.0f);

	/*
	if (PercentX < 0.0f) {
		PercentX = 0.0f;
	}
	if (PercentX > 1.0f) {
		PercentX = 1.0f;
	}
	if (PercentY < 0.0f) {
		PercentY = 0.0f;
	}
	if (PercentY > 1.0f) {
		PercentY = 1.0f;
	}*/

	int x = FMath::RoundHalfFromZero((GridSizeX - 1) * PercentX);
	int y = FMath::RoundHalfFromZero((GridSizeY - 1) * PercentY);
	return grid[x][y];
}