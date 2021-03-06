// © Coyrights Nilapatri, All Rights Reserved


#include "InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::OnInteract_Implementation(AActor* Caller)
{
	Destroy();
}

void AInteractableActor::StartFocus_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Start Focus"));
	UE_LOG(LogTemp, Display, TEXT("Start Focus"));
}

void AInteractableActor::EndFocus_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("End Focus"));
	UE_LOG(LogTemp, Display, TEXT("End Focus"));
}

