// Copyright Epic Games, Inc. All Rights Reserved.

#include "CollisionTestingCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACollisionTestingCharacter

ACollisionTestingCharacter::ACollisionTestingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	AddSignificanceThreshold(2.0, 1000.0f);
	AddSignificanceThreshold(1.0f, 2500.0f);
	AddSignificanceThreshold(0.0f, 5000.0f);

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACollisionTestingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACollisionTestingCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollisionTestingCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACollisionTestingCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACollisionTestingCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACollisionTestingCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACollisionTestingCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACollisionTestingCharacter::OnResetVR);
}




void ACollisionTestingCharacter::OnResetVR()
{
	// If CollisionTesting is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in CollisionTesting.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACollisionTestingCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACollisionTestingCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACollisionTestingCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACollisionTestingCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACollisionTestingCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACollisionTestingCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void ACollisionTestingCharacter::BeginPlay()
{
	Super::BeginPlay();

	//if (!IsNetMode(NM_DedicatedServer))
	//{
	//	USignificanceManager* SignificanceManager = USignificanceManager::Get(GetWorld());
	//	if (SignificanceManager)
	//	{
	//		auto Significance = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint) -> float
	//		{
	//			return SignificanceFunction(ObjectInfo, ViewPoint);
	//		};

	//		auto PostSignificance = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
	//		{
	//			return PostSignificanceFunction(ObjectInfo, OldSignificance, Significance, bFinal);
	//		};

	//		SignificanceManager->RegisterObject(this, TEXT("Character"), Significance, USignificanceManager::EPostSignificanceType::Sequential, PostSignificance);



	//		/*SignificanceManager->RegisterObject(this, TEXT("Character"), &SignificanceFunction, USignificanceManager::EPostSignificanceType::Sequential, &PostSignificanceFunction);*/
	//	}
	//}

	DynamicMaterial = GetMesh()->CreateDynamicMaterialInstance(0, Significance2Material);

	//GetMesh()->SetMaterial(0, DynamicMaterial);

	if (!IsNetMode(NM_DedicatedServer))
	{
		RegeisterWithSignificancemanager(this, GetWorld(), TEXT("Character"));
	}


	/*ParticleSystem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "head");
	ParticleSystem->SetWorldRotation(FQuat::MakeFromEuler(FVector::UpVector));

	ParticleSystem->ActivateSystem();*/
	//ParticleSystem->DeactivateSystem();

}

//UParticleSystemComponent* ACollisionTestingCharacter::GetParticleSystem() 
//{
//	return ParticleSystem;
//}

void ACollisionTestingCharacter::PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
{
	if (OldSignificance == Significance) return;

	if (ObjectInfo->GetTag() == TEXT("Character"))
	{
		ACollisionTestingCharacter* Character = CastChecked<ACollisionTestingCharacter>(ObjectInfo->GetObject());

		if (Significance == 0.0f)
		{

			Character->GetCharacterMovement()->SetComponentTickInterval(0.25f);
			//Character->GetMesh()->SetMaterial(0,Significance0Material);
			DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Red);
			//Character->GetParticleSystem()->DeactivateSystem();

		}
		else if (Significance == 1.0f)
		{
			Character->GetCharacterMovement()->SetComponentTickInterval(0.1f);
			//Character->GetMesh()->SetMaterial(0,Significance1Material);
			//Character->GetParticleSystem()->DeactivateSystem();
			//if (Character->GetParticleSystem()->IsActive()) Character->GetParticleSystem()->DeactivateSystem();
			DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Yellow);
		}
		else if (Significance == 2.0f)
		{
			Character->GetCharacterMovement()->SetComponentTickInterval(0.0f);
			//Character->GetMesh()->SetMaterial(0,Significance2Material);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("HighestSignificance")) + Character->GetName() );
			//if(!Character->GetParticleSystem()->IsActive()) Character->GetParticleSystem()->ActivateSystem();
			DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Green);


			
		}
	}
}

//float ACollisionTestingCharacter::GetSignificanceByDistance(float distance)
//{
//	const int32 NumThresholds = SignificanceSettings.SignificanceThresholds.Num();
//	if (distance >= SignificanceSettings.SignificanceThresholds[NumThresholds - 1].MaxDistance)
//	{
//		return SignificanceSettings.SignificanceThresholds[NumThresholds - 1].Significance;
//	}
//	else
//	{
//		for (int32 i = 0; i < NumThresholds; i++)
//		{
//			FSignificanceSettings::FSignificanceThresholds& Thresholds = SignificanceSettings.SignificanceThresholds[i];
//			if (distance <= Thresholds.MaxDistance)
//			{
//				return Thresholds.Significance;
//			}
//		}
//	}
//
//	return 0.0f;
//}

float ACollisionTestingCharacter::SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint)
{
	if (ObjectInfo->GetTag() == TEXT("Character"))
	{
		ACollisionTestingCharacter* Character = CastChecked<ACollisionTestingCharacter>(ObjectInfo->GetObject());

		const float Distance = (Character->GetActorLocation() - ViewPoint.GetLocation()).Size();

		return GetSignificanceByDistance(Distance);

	}

	return 0.0f;
}


//
////Significance Manager Functions
//float ACollisionTestingCharacter::SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint)
//{
//	if (ObjectInfo->GetTag() == TEXT("Character"))
//	{
//		ACollisionTestingCharacter* Character = CastChecked<ACollisionTestingCharacter>(ObjectInfo->GetObject());
//
//		const float Distance = (Character->GetActorLocation() - ViewPoint.GetLocation()).Size();
//
//		return GetSignificanceByDistance(Distance);
//
//	}
//
//	return 0.0f;
//}
//
//void ACollisionTestingCharacter::PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
//{
//
//
//	if (ObjectInfo->GetTag() == TEXT("Character"))
//	{
//		ACollisionTestingCharacter* Character = CastChecked<ACollisionTestingCharacter>(ObjectInfo->GetObject());
//
//		if (Significance == 0.0f)
//		{
//			
//			Character->GetCharacterMovement()->SetComponentTickInterval(0.25f);
//
//		}
//		else if(Significance == 1.0f)
//		{
//			Character->GetCharacterMovement()->SetComponentTickInterval(0.1f);
//		}
//		else if (Significance == 2.0f)
//		{
//			Character->GetCharacterMovement()->SetComponentTickInterval(0.0f);
//		}
//	}
//}
//
//float ACollisionTestingCharacter::GetSignificanceByDistance(float distance)
//{
//	const int32 NumThresholds = SignificanceSettings.SignificanceThresholds.Num();
//	if (distance >= SignificanceSettings.SignificanceThresholds[NumThresholds - 1].MaxDistance)
//	{
//		return SignificanceSettings.SignificanceThresholds[NumThresholds - 1].Significance;
//	}
//	else
//	{
//		for (int32 i = 0; i < NumThresholds; i++)
//		{
//			FSignificanceSettings::FSignificanceThresholds& Thresholds = SignificanceSettings.SignificanceThresholds[i];
//			if (distance <= Thresholds.MaxDistance)
//			{
//				return Thresholds.Significance;
//			}
//		}
//	}
//
//	return 0.0f;
//}
