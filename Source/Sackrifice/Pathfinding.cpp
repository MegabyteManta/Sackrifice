// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"

// Sets default values
APathfinding::APathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APathfinding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FindPath(Seeker->GetActorLocation(), Target->GetActorLocation());
}

TArray<FVector> APathfinding::FindPath(FVector StartPos, FVector TargetPos) {
	//Start/Target nodes found from world positions
	Node StartNode = Grid->NodeFromWorldPoint(StartPos);
	Node TargetNode = Grid->NodeFromWorldPoint(TargetPos);

	//TArrays can be heaped
	//Open is the set of neighbors visited but not yet travelled
	TArray<Node> OpenHeap;
	TSet<Node> OpenSet;

	//Closed is the set of nodes travelled
	TSet<Node> ClosedSet;

	//Start off with the start Node
	OpenSet.Add(StartNode);
	OpenHeap.HeapPush(StartNode);

	//Finds path, continuously adding visited nodes to OpenHeap
	//O(N) if path never found
	while (OpenHeap.Num() > 0) {
		
		
		//Get the current Node from the top of the heap
		Node CurrentNode;
		OpenHeap.HeapPop(CurrentNode);
		//DrawDebugBox(GetWorld(), CurrentNode.WorldPosition, FVector(Grid->NodeRadius*0.5f, Grid->NodeRadius*0.5f, Grid->NodeRadius*0.5f), FColor::Green, true, -1, 0, 10);

		//Remove the current Node from the Open set, and add it to the Closed Set
		//i.e. the Node is being travelled, so it never needs to be visited again
		OpenSet.Remove(CurrentNode);
		ClosedSet.Add(CurrentNode);

		//If the current Node is the Target Node, the path was found
		if (CurrentNode == TargetNode) {
			TArray<FVector> Waypoints = RetracePath(StartNode, CurrentNode);
			return Waypoints;
		}

		//Go through all of the current Node's neighbors, calculate their costs,
		//and add them to the Open set
		for (Node Neighbor : Grid->GetNeighbors(CurrentNode)) {
			//Don't check unwalkable or travelled Nodes
			if (!Neighbor.Walkable || ClosedSet.Contains(Neighbor)) {
				continue;
			}

			//Calculate the cost from start to the neighbor, and replace parent and cost if cost is less
			int NewCostToNeighbor = CurrentNode.Cost.GCost + GetDistance(CurrentNode, Neighbor);
			if (NewCostToNeighbor < Neighbor.Cost.GCost || !OpenSet.Contains(Neighbor)) {
				Neighbor.Cost = CostVector(GetDistance(Neighbor, TargetNode), NewCostToNeighbor);
				//Neighbor.parent = &CurrentNode;
				Neighbor.HasParent = true;
				Neighbor.ParentPos = CurrentNode.WorldPosition;
				Grid->grid[Neighbor.X][Neighbor.Y] = Neighbor;

				//Add the neighbor to Open (visited)
				if (!OpenSet.Contains(Neighbor)) {
					OpenHeap.HeapPush(Neighbor);
					OpenSet.Add(Neighbor);
				}
			}
		}
	}
	return TArray<FVector>();
}

//Backtracking, get path after finding end node
TArray<FVector> APathfinding::RetracePath(Node StartNode, Node EndNode) {
	TArray<Node> Path = TArray<Node>();
	Node CurrentNode = EndNode;

	//Reconstruct path (backwards)
	while (CurrentNode != StartNode) {
		Path.Add(CurrentNode);
		CurrentNode = Grid->NodeFromWorldPoint(CurrentNode.ParentPos);
	}
	Path.Add(StartNode);

	//Path created backwards, so reverse it
	Algo::Reverse(Path);

	//Set the path on the Grid
	//Grid->Path = Path;
	
	//Make an array of positions of the nodes, this is the path
	//UE_LOG(LogTemp, Warning, TEXT("I SHOULD BE GETTING COLORED"));
	TArray<FVector> NodePostitions;
	for (Node node : Path) {
		if (bDebug) DrawDebugBox(GetWorld(), node.WorldPosition, FVector(Grid->NodeRadius*0.5f, Grid->NodeRadius*0.5f, Grid->NodeRadius*0.5f), FColor::Yellow, true, -1, 0, 10);
		NodePostitions.Add(node.WorldPosition);
	}

	return NodePostitions;
	return SimplifyPath(Path);
}

//This is for simplifying path down to points when direction changes
//currently unused
TArray<FVector> APathfinding::SimplifyPath(TArray<Node> Path) {
	TArray<FVector> Waypoints;
	FVector2D DirectionOld = FVector2D().ZeroVector;
	for (int i = 1; i < Path.Num(); i++) {
		FVector2D DirectionNew = FVector2D(Path[i-1].X - Path[i].X, Path[i - 1].Y - Path[i].Y);
		if (DirectionNew != DirectionOld) {
			Waypoints.Add(Path[i-1].WorldPosition);
			DirectionOld = DirectionNew;
		}
	}
	Waypoints.Add(Path[Path.Num()-1].WorldPosition);
	return Waypoints;
}

//Get distance between Nodes
int APathfinding::GetDistance(Node NodeA, Node NodeB) {
	int DstX = UKismetMathLibrary::Abs(NodeA.X - NodeB.X);
	int DstY = UKismetMathLibrary::Abs(NodeA.Y - NodeB.Y);

	if (DstX > DstY) return 14 * DstY + 10 * (DstX - DstY);
	return 14 * DstX + 10 * (DstY - DstX);
}
