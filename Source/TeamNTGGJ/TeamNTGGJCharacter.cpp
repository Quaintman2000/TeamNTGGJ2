// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeamNTGGJCharacter.h"
#include "TeamNTGGJProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATeamNTGGJCharacter

ATeamNTGGJCharacter::ATeamNTGGJCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ATeamNTGGJCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	WalkSpeed = MovementComp->MaxWalkSpeed;

	bIsCrouched = false;

	StandingEyeHeight = FirstPersonCameraComponent->GetRelativeTransform().GetLocation().Z;

	// Create UI
	LaughScreen = CreateWidget(Cast<APlayerController>(Controller), LaughScreenClass);
}

//////////////////////////////////////////////////////////////////////////// Input

void ATeamNTGGJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATeamNTGGJCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATeamNTGGJCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ATeamNTGGJCharacter::SetIsSprinting);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ATeamNTGGJCharacter::ToggleCrouch);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ATeamNTGGJCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATeamNTGGJCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATeamNTGGJCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ATeamNTGGJCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ATeamNTGGJCharacter::SetIsLaughing(bool NewIsLaughing)
{
	bIsLaughing = NewIsLaughing;

	if (LaughScreen != nullptr)
	{
		if (bIsLaughing)
			LaughScreen->AddToViewport();
		else
			LaughScreen->RemoveFromViewport();
	}
}

void ATeamNTGGJCharacter::SetIsSprinting(const FInputActionValue& Value)
{
	bool bNewIsSprinting = Value.Get<bool>();

	if (bNewIsSprinting == bIsSprinting) return;
	bIsSprinting = bNewIsSprinting;

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (bIsSprinting == true)
	{
		MovementComp->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		MovementComp->MaxWalkSpeed = WalkSpeed;
	}
}

bool ATeamNTGGJCharacter::GetIsSprinting()
{
	return bIsSprinting;
}

void ATeamNTGGJCharacter::SetOxygenLevel(float NewOxygenLevel)
{
	OxygenLevel = NewOxygenLevel;
}

float ATeamNTGGJCharacter::GetOxygenLevel()
{
	return OxygenLevel;
}

void ATeamNTGGJCharacter::ToggleCrouch()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CanCrouch() && bIsCrouched == false)
	{
		Crouch();
		bIsCrouched = true;
		// Moves our camera down to crouch height.
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, CrouchedEyeHeight));
		// Shrinks our colider to crouch size.
		CapsuleComp->SetCapsuleHalfHeight(40);
	}
	else if(bIsCrouched == true)
	{
		UnCrouch();
		bIsCrouched = false;
		// Moves our camera up to standing height.
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, StandingEyeHeight));
		// Enlarges our colider to original size.
		CapsuleComp->SetCapsuleHalfHeight(96);
	}
}
