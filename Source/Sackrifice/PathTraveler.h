// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Node.h"
#include "Grid.h"
#include "Pathfinding.h"
#include "Math/UnrealMathUtility.h"
#include "GenericPlatform/GenericPlatformMath.h" 
#include "Math/Vector.h" 
#include "GameFramework/Actor.h" 
#include "DrawDebugHelpers.h"
#include "PathTraveler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SACKRIFICE_API UPathTraveler : public UActorComponent
{
	GENERATED_BODY()

	TArray<FVector> Waypoints;
	FVector CurrentWaypoint;
	int WaypointIndex;
	//int WaypointPrevious;
	Node TargetNodeOld = Node();
	#define FOLLOWING_PATH 0
	#define FOLLOWING_PLAYER 1 
	int state = FOLLOWING_PATH;

	int PositionIndex = 0;

	//Follow a specified path
	void FollowPath(float DeltaTime);
	//Follow the player
	void FollowPlayer(float DeltaTime);
	//Function that actually moves the actor
	void Travel(float DeltaTime, FVector TargetLocation);
	//Is the target in the actor's field of view?
	bool TargetInFieldOfView();

public:	
	// Sets default values for this component's properties
	UPathTraveler();

	//Reference to Grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGrid* Grid;

	//Reference to Pathfinder
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APathfinding* Pathfinder;

	//Target that is sought after
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target;
	
	//Specify a specific path for the actor to travel on
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> Positions;

	//Repeat the specified path?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bRepeatPath = true;

	//Start seeking the target if in this radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InnerRadius = 100.0f;

	//Start seeking target if target is in view and in this radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float OuterRadius = 500.0f;

	//Stop followingplayer/path if reached them
	bool ReachedPlayer = false;
	bool PathEnded = false;

	bool CurrentWaypointsNotSet = true;
	
	//Actor's velocity
	UPROPERTY(EditAnywhere)
		float Velocity = 0;
	
	//Actor's rotation rate
	UPROPERTY(EditAnywhere)
		float RotationRate = 0;

	//Anything greater than the view threshold is within view
	UPROPERTY(EditAnywhere)
		float ViewThreshold;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
