// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "PlayerSack.generated.h"

UCLASS()
class SACKRIFICE_API APlayerSack : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* FollowCamera;

	//Capsule responsible for overlap events
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* OverlapCapsule;

	//For using physics grabbing
	//UPROPERTY(VisibleAnywhere)
		//UPhysicsHandleComponent* PhysicsHandle;

	//movement lol
	void MoveForward(float Axis);
	void MoveRight(float Axis);

	//Overlap event
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//Collision event
	UFUNCTION()
		void OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			FVector NormalImpulse, const FHitResult & Hit);

	//Closest object to the player
	UPROPERTY()
		AActor* ClosestObject = nullptr;

	//Find the closest object
	UFUNCTION()
		void GetClosestObject();
		
	//Pick up the closest object
	UFUNCTION()
		void PickUp();

	/*
	//Launch Eagles
	UFUNCTION()
		void Attack();
	*/

	//Are you holding an object?
	UPROPERTY()
		bool bIsHolding = false;

	//Reference to object you're holding
	UPROPERTY()
		AActor* HeldObject;

	bool bSetObjectPos = false;

	//Save the physics simulation of the object's meshes you're holding
	//This is because attaching the object to the player causes it
	//to lose physics simulation.
	//IF YOU NOTICE SOMETHING WEIRD WITH PICKING UP/DROPPING OBJECTS, 
	//IT COULD BE THIS. I'M AFRAID THAT GETCOMPONENTS DOESNT ALWAYS
	//GET THE COMPONENTS IN THE SAME ORDER. IF IT DOESNT, THIS ARRAY
	//WONT WORK!!!!
	UPROPERTY()
		TArray<bool> bHeldObjectMeshesSimulatePhysics;

	//Replacing the whole mesh doesn't work. Gonna have to find a way
	//to create a new static mesh as just saving the pointer doesn't
	//actually save a new instance of the object
	//UStaticMeshComponent* HeldObjectMesh;

	//debug counter
	int dummy = 0;

	//Location on the player where the object will be held
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* GripLocation;

	void RestartGame();

public:
	// Sets default values for this character's properties
	APlayerSack();

	bool bDead = false;
	UFUNCTION(BlueprintCallable)
		void Kill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
