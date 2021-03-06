// © Copyrights Nilapatri, All Rights Reserved


#include "PowerComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPowerComponent::UPowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	DefaultPower = 5.0f;
	MaxPower = 10.0f;
}


// Called when the game starts
void UPowerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	Power = MaxPower;

	AActor* ActorOwner = GetOwner();

	if (ActorOwner)
	{
		ActorOwner->OnTakeAnyDamage.AddDynamic(this, &UPowerComponent::TakeDamage);
	}
}

// Called every frame
/* void UPowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
} */

void UPowerComponent::TakeDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	/* if (Damage >= 1)
	{
		Power -= Damage;
	} */

	Power = FMath::Clamp(Power - Damage, 0.0f, MaxPower);
}

