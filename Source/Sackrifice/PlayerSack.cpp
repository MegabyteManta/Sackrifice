// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSack.h"

// Sets default values
APlayerSack::APlayerSack()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();
	//Capsule responsible for collisions
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 88.0f);

	//Capsule responsible for overlaps (bigger than the collision capsule)
	OverlapCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OverlapCapsule"));
	OverlapCapsule->SetupAttachment(RootComponent);
	OverlapCapsule->BodyInstance.SetCollisionProfileName("OverlapAll");
	OverlapCapsule->InitCapsuleSize(68.0f, 196.0f);


	//Don't rotate player, only rotate camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//allow the character to rotate in the direction it is moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	//how much can you control you character in the air
	GetCharacterMovement()->AirControl = 0.2f;

	//Location on character where picked up objects are held
	GripLocation = CreateDefaultSubobject<USceneComponent>(TEXT("GripLocation"));
	GripLocation->SetupAttachment(RootComponent);

	//The Camera is going to be set up as a spring arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//You can see the root component in the BP, for playersack it is the CapsuleComponent
	CameraBoom->SetupAttachment(RootComponent);
	//How far away is the spring arm going to be from the player (aka how far the camera will be away from player)
	CameraBoom->TargetArmLength = 300.0f;
	//rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	//Create the camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//attach camera to end of camera boom and let the boom control the match rotation of the camera
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void APlayerSack::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerSack::OnHit);
	OverlapCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerSack::OnBeginOverlap);
}

// Called every frame
void APlayerSack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerSack::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*
	//Attack (X)
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerSack::Attack);
	*/

	//PickUp (Z)
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &APlayerSack::PickUp);

	//jumping events (Space)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//movement events (Arrows)
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerSack::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerSack::MoveRight);

	//turn events (Mouse)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

// Moving forwards/backwards
void APlayerSack::MoveForward(float Axis)
{
	if (Axis != 0.0f)
	{
		//calculate the forward vector based on the yaw and use it to move forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

// Moving right/left
void APlayerSack::MoveRight(float Axis)
{
	if (Axis != 0.0f)
	{
		//calculate the forward vector based on the yaw and use it to move right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void APlayerSack::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	//if (OtherActor->ActorHasTag("WaxBody")) {
	dummy++;
	UE_LOG(LogTemp, Warning, TEXT("Collided with %d"), dummy);
	//PickUp();
	//}
}

void APlayerSack::OnBeginOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	dummy++;
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with %d"), dummy);
	//OtherActor->Destroy();
}

void APlayerSack::Kill() {
	bDead = true;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	DisableInput(PlayerController);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
	GetMesh()->SetSimulatePhysics(true);
	//GetCapsuleComponent()->SetMobility(EComponentMobility::Static);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerSack::RestartGame, 3.0f, false);
}

void APlayerSack::RestartGame() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


/*
Get the closest interactable object to the player, sets it to the ClosestObject field
Time complexity: O(C*O). We can actually make it a bit faster by modifying GetOverlappingActors
to find the min distance object while it creates the array of objects. This doesn't change the time
complexity, it's just that we loop through the objects twice.
*/
void APlayerSack::GetClosestObject() {
	dummy++;
	
	//List of all object being overlapped (returned by GetOverlappingActors)
	TArray<AActor*> Objects;
	//GetOverlappingActors is O(C*O) where C = number of components on this actor and O = number of objects being overlapped
	GetOverlappingActors(Objects, TSubclassOf<AActor>());
	
	float min = INFINITY;
	AActor* CurrentClosestObject = nullptr;
	//Get the closest object to the player
	for (auto Object : Objects) {
		if (Object->ActorHasTag(TEXT("Holdable")) || Object->ActorHasTag(TEXT("Interactable"))) {
			float Dist = FVector::Dist(GetActorLocation(), Object->GetActorLocation());
			if (Dist < min) {
				min = Dist;
				CurrentClosestObject = Object;
			}
		}
	}

	//save the closest object
	ClosestObject = CurrentClosestObject;
}

/*
If an object can be picked up, pick it up.
If there are multiple, it will pick up the closest
*/
void APlayerSack::PickUp() {
	GetClosestObject();
	//Catch if you try to pick up an object when there aren't any around
	if (ClosestObject == nullptr && !bIsHolding) {
		UE_LOG(LogTemp, Warning, TEXT("ClosestObject is nullptr and you're not holding an object silly"));
		UE_LOG(LogTemp, Warning, TEXT("This could mean that the closest object is not interactable or holdable or you're not close to an object"));
		return;
	}
	if (bIsHolding && HeldObject == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("You picked up an object but somehow the reference to it was lost"));
		bHeldObjectMeshesSimulatePhysics.Empty();
		bIsHolding = false;
		return;
	}

	//PICK UP CLOSEST OBJECT
	if (!bIsHolding && ClosestObject->ActorHasTag(TEXT("Holdable"))) {

		
		//Get closest object's static mesh component
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		ClosestObject->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

		//Catches for weird cases
		if (StaticMeshComponents.Num() > 1) UE_LOG(LogTemp, Warning, TEXT("What the fuck? Multiple static mesh components on object during attachment %d"), StaticMeshComponents.Num());
		if (StaticMeshComponents.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("There are no static meshes on this object during attachment %d"), StaticMeshComponents.Num());
			return;
		}


		//Go through every static mesh and set the collision profile
		//If objects aren't abstracted, you can use this to save the Simulating Physics bool.
		for (int i = 0; i < StaticMeshComponents.Num(); i++) {
			//bHeldObjectMeshesSimulatePhysics.Add(StaticMeshComponents[i]->IsSimulatingPhysics());
			StaticMeshComponents[i]->SetSimulatePhysics(false);
		}

		//Snap object to target and weld both bodies together to become one simulated physics body
		//Welding means that the held object and the actor becomes one rigidbody
		//For some reason upon dropping object, physics simulation is not remembered
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		FVector SpawnLocation = GripLocation->GetComponentLocation();
		//ClosestObject->AttachToComponent(GetMesh(), Rules, FName(TEXT("GripPoint")));
		ClosestObject->AttachToActor(this, Rules, FName(TEXT("GripPoint")));
		ClosestObject->SetActorLocation(SpawnLocation, false, (FHitResult *)nullptr, ETeleportType::ResetPhysics);

		//THIS LINE MAKES IT SO YOU DON'T TELEPORT IF YOU STAND ON THE OBJECT
		//MAKE IT SO THAT IT'S ONLY NULL IF YOU'RE STANDING ON AN OBJECT
		//THIS COULD LEAD TO PROBLEMS
		GetCharacterMovement()->SetBase(NULL);

		/*
		HeldObject = ClosestObject;
		UStaticMeshComponent* RootComp = Cast<UStaticMeshComponent>(HeldObject->GetRootComponent());
		RootComp->SetSimulatePhysics(false);
		HeldObject->SetActorLocation(GripLocation->GetComponentLocation());
		bIsHolding = true;
		*/

		HeldObject = ClosestObject;
		bIsHolding = true;
	}


	//DROP hELD OBJECT
	else if (bIsHolding){

		//Drop held object
		FDetachmentTransformRules DetachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		HeldObject->DetachFromActor(DetachRules);

		//Get held object's static mesh components
		TArray<UStaticMeshComponent*> StaticMeshComponents;
		HeldObject->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

		//catches for weird cases (no static meshes or multiple static meshes)
		if (StaticMeshComponents.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("There are no static meshes on this object during detachment %d"), StaticMeshComponents.Num());
			return;
		}
		if (StaticMeshComponents.Num() > 1) UE_LOG(LogTemp, Warning, TEXT("What the fuck? Multiple static mesh components on object during detachment %d"), StaticMeshComponents.Num());

		//Update the static meshes with blocking collisions
		//Alternaitvely, if objects aren't abstracted, update their original physics simulation
		for (int i = 0; i < StaticMeshComponents.Num(); i++) {
			StaticMeshComponents[i]->SetSimulatePhysics(true);//(bHeldObjectMeshesSimulatePhysics[i]);
			StaticMeshComponents[i]->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		}

		bSetObjectPos = true;

		//reset values
		HeldObject = nullptr;
		bIsHolding = false;
		//bHeldObjectMeshesSimulatePhysics.Empty();
	}

	else if (ClosestObject->ActorHasTag(TEXT("Interactable"))) {
		//INTERACT
		UE_LOG(LogTemp, Warning, TEXT("INTERACT"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Error neither holdable nor interactable"));
	}

}

/*
void APlayerSack::Attack() {
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<AProjectile>(Location, Rotation, SpawnInfo);
}
*/