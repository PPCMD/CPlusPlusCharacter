// © Coyrights Nilapatri, All Rights Reserved

#include "CharacterClass.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
// #include "Components/StaticMeshComponent.h" // Required for weapon
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
/* #include "Projectile.h"
#include "Kismet/GameplayStatics.h" */ // Required for sound
#include "DrawDebugHelpers.h" // Not required as it is not used
#include "InteractInterface.h"
#include "InteractableActor.h"

// Sets default values
ACharacterClass::ACharacterClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringComponen"));
	CameraSpring->SetupAttachment(RootComponent);

	/* Not recommended as default values alreay exist and using this will make 
	recompiling necessary every time the value needs to be changed. */
	// CameraSpring->TargetArmLength = 400;
	DefaultSpringLength = 350.0f;
	FirstPersonSpringLength = 0.0f;
	CameraSpring->TargetArmLength = DefaultSpringLength;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(CameraSpring);

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComponent);

	// Create a weapon mesh
	/* WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterWeapon"));
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;

	// Setup the attachment
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponLocation = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponLocation"));
	WeaponLocation->SetupAttachment(WeaponMesh);
	WeaponLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
	
	// Default offset from the character's location, the location 
	// for projectiles to spawn
	WeaponOffset = FVector(100.0f, 0.0f, 10.0f); */
	
	BaseTurnRate = 40.0f;
	BaseLookUpRate = 50.0f;

	CameraSpring->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

	CanMove = true;
	CanMoveForward = true;
	CanMoveSide = true;
	CameraCanMove = true;

	GetCharacterMovement()->AirControl = 1.0f;

	TraceDistance = 2000;

	ImpulseForce = 500.0f;

	bApplyRadialForce = true;
	ImpactRadius = 200.0f;
	RadialImpactForce = 2000.0f;
}

void ACharacterClass::BeginPlay()
{
	Super::BeginPlay();

	CharacterMesh->OnComponentBeginOverlap.AddDynamic(this, &ACharacterClass::OnOverlapBegin);

	// WeaponMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void ACharacterClass::MoveForward(float Value)
{
	if ((Controller) && Value != 0.0f)
	{
		if (CanMove && CanMoveForward)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void ACharacterClass::MoveRight(float Value)
{
	if ((Controller) && Value != 0.0f)
	{
		if (CanMove && CanMoveSide)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
}

void ACharacterClass::CameraTurn(float Value)
{
	if (CameraCanMove)
	{
		APawn::AddControllerYawInput(Value);
	}
}

void ACharacterClass::CameraLookUp(float Value)
{
	if (CameraCanMove)
	{
		APawn::AddControllerPitchInput(Value);
	}
}

void ACharacterClass::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterClass::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterClass::InteractPressed()
{
	TraceForward();

	if (FocusedActor)
	{
		IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);

		if (Interface)
		{
			Interface->Execute_OnInteract(FocusedActor, this);
		}
	}
}

void ACharacterClass::CursorFocusStart_Implementation()
{

}

void ACharacterClass::CursorFocusStop_Implementation()
{

}

void ACharacterClass::TraceForward_Implementation()
{
	FVector ActorLocation;
	FRotator ActorRotation;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(ActorLocation, ActorRotation);

	FVector Start = ActorLocation;
	FVector End = Start + (ActorRotation.Vector() * TraceDistance);

	FCollisionQueryParams TraceParameters;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParameters);

	// DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f);

	if (bHit)
	{
		// DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);

		AActor* Interactable = Hit.GetActor();

		if (Interactable)
		{
			if (Interactable != FocusedActor)
			{
				if (FocusedActor)
				{
					IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);

					if (Interface)
					{
						Interface->Execute_EndFocus(FocusedActor);
						CursorFocusStop();
					}
				}

				IInteractInterface* Interface = Cast<IInteractInterface>(Interactable);

				if (Interface)
				{
					Interface->Execute_StartFocus(Interactable);
					CursorFocusStart();
				}

				FocusedActor = Interactable;
			}
		}
		else {
			if (FocusedActor)
			{
				IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);

				if (Interface)
				{
					Interface->Execute_EndFocus(FocusedActor);
					CursorFocusStop();
				}
			}

			FocusedActor = nullptr;
		}
	}
}

void ACharacterClass::SpawnPressed()
{
	FVector Location;
	FRotator Rotation;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * TraceDistance);

	FCollisionQueryParams TraceParameters;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParameters);
	
	if (bHit)
	{
		SpawnObject(Hit.Location, Rotation);
	}
}

void ACharacterClass::SpawnObject(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParameters;
	AInteractableActor* SpawnActorReference = GetWorld()->SpawnActor<AInteractableActor>(ActorToSpawn, Location, Rotation, SpawnParameters);
}

void ACharacterClass::FireForward()
{
	FVector Location;
	FRotator Rotation;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * TraceDistance);

	FCollisionQueryParams TraceParameters;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParameters);

	if (bHit)
	{
		if (Hit.GetActor()->IsRootComponentMovable())
		{
			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());

			if (MeshComponent)
			{
				FVector CameraForward = PlayerCamera->GetForwardVector();
				MeshComponent->AddImpulse(CameraForward * ImpulseForce * MeshComponent->GetMass());
			}
		}

		if (bApplyRadialForce)
		{
			FCollisionShape SphereCollision = FCollisionShape::MakeSphere(ImpactRadius);

			bool bSweepHit = GetWorld()->SweepMultiByChannel(HitActors, Hit.Location, Hit.Location, FQuat::Identity, ECC_WorldStatic, SphereCollision);

			DrawDebugSphere(GetWorld(), Hit.Location, ImpactRadius, 50, FColor::Blue, false, 2.0f);

			if (bSweepHit)
			{
				for (auto& Hit : HitActors)
				{
					UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());

					if (MeshComponent)
					{
						MeshComponent->AddRadialImpulse(Hit.Location, ImpactRadius, RadialImpactForce, ERadialImpulseFalloff::RIF_Constant, true);
					}
				}
			}
		}
	}
}

void ACharacterClass::SwitchView()
{
	if (FirstPersonView)
	{
		CameraSpring->TargetArmLength = DefaultSpringLength;
		FirstPersonView = false;
	} else {
		CameraSpring->TargetArmLength = FirstPersonSpringLength;
		FirstPersonView = true;
	}
}

void ACharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForward();
}

void ACharacterClass::Fire()
{
	/* if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			const FVector SpawnLocation = ((WeaponLocation != nullptr) ? WeaponLocation->GetComponentLocation() : GetActorLocation() + SpawnRotation.RotateVector(WeaponOffset));

			// Set the spawn parameters
			FActorSpawnParameters ActorSpawnParameters;
			ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParameters);
		}
	}
	
	if (FireSound != NULL )
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	} */
}

void ACharacterClass::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IInteractInterface* Interface = Cast<IInteractInterface>(OtherActor);

	if (Interface)
	{
		Interface->Execute_OnInteract(OtherActor, this);
	}
}

// Called to bind functionality to input
void ACharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterClass::InteractPressed);
	PlayerInputComponent->BindAction("Spawn", IE_Pressed, this, &ACharacterClass::SpawnPressed);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterClass::FireForward);
	PlayerInputComponent->BindAction("SwitchView", IE_Pressed, this, &ACharacterClass::SwitchView);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterClass::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacterClass::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterClass::CameraLookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterClass::TurnAtRate);
	PlayerInputComponent->BindAxis("LookRate", this, &ACharacterClass::LookUpAtRate);
}

