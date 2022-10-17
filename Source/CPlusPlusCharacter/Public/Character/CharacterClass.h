// © Copyrights Nilapatri, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// #include "InteractInterface.h" Use public IInteractInterface, if using
#include "CharacterClass.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
/* class UStaticMeshComponent;
class USceneComponent; */
class AInteractableActor;

UCLASS()
class CPLUSPLUSCHARACTER_API ACharacterClass : public ACharacter/*, public IInteractInterface*/
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterClass();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraSpring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	USkeletalMeshComponent* CharacterMesh;

	/* UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USceneComponent* WeaponLocation; */
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool CanMoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool CanMoveSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool CameraCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultSpringLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float FirstPersonSpringLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ViewSwitchTime;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<AInteractableActor> ActorToSpawn;

	UFUNCTION()
	void SpawnObject(FVector Location, FRotator Rotation);

/*public:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<class AProjectile> ProjectileClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FireSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector WeaponOffset; */

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void CameraTurn(float Value);
	void CameraLookUp(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);

	void InteractPressed();
	void Fire();
	void SpawnPressed();
	void SwitchView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool FirstPersonView;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void CursorFocusStart();
	void CursorFocusStart_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void CursorFocusStop();
	void CursorFocusStop_Implementation();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Impulse
	UPROPERTY(EditAnywhere, Category = "Impulse")
	float ImpulseForce;

	void FireForward();
	void RadialFire();

	// Radial Impulse
	UPROPERTY(EditAnywhere, Category = "Impulse")
	bool bApplyRadialForce;
	UPROPERTY(EditAnywhere, Category = "Impulse")
	float ImpactRadius;
	UPROPERTY(EditAnywhere, Category = "Impulse")
	float RadialImpactForce;

	TArray<FHitResult> HitActors;

public:
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	AActor* FocusedActor;
	
	
};
