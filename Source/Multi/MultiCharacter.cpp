// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MultiCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

AMultiCharacter::AMultiCharacter()
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
}
void AMultiCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMultiCharacter::JumpFunc);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMultiCharacter::StopJumpingFunc);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMultiCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMultiCharacter::StopRun);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMultiCharacter::CrouchFunction);
	PlayerInputComponent->BindAction("LeanRight", IE_Pressed, this, &AMultiCharacter::LeanRight);
	PlayerInputComponent->BindAction("LeanLeft", IE_Pressed, this, &AMultiCharacter::LeanLeft);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMultiCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMultiCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMultiCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMultiCharacter::LookUpAtRate);
}

void AMultiCharacter::ResetRecoil()
{
	pitchAcum = 0;
	yawAcum = 0;
	bFirstShoot = true;
	//TODO:
}
void AMultiCharacter::CancelInteract()
{

}
void AMultiCharacter::CheckRun()
{
	if (!bAiming && bCanRun && !bThrowingGrenade && !GetCharacterMovement()->IsFlying() && !bAttacking && bWantToRun && !bWantToAim)
	{
		if (!bRunning)
		{
			ResetLeaning();
			bChangingWeapon = false;
			//TODO:
			//Mesh->GetAnimInstance()->Montage_Stop(0.3f, NULL);
			//GetPlayerControllerFromNetId(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(NULL, 1.f, ECameraAnimPlaySpace::CameraLocal, FRotator::ZeroRotator);
			GetCharacterMovement()->MaxWalkSpeed = RunSpeedModifier * DefaultRunSpeedModifer;
			ServerRun();
		}
	}
	else
	{
		if (bRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
			
			GetPlayerControllerFromNetId(GetWorld(), 0)->PlayerCameraManager->StopAllCameraShakes(true);
			ServerWalk();
			CheckAim();

		}
	}
}
void AMultiCharacter::CheckAim()
{

}
void AMultiCharacter::ForceUncrouch()
{
	bWantsToCrouch = false;
	UnCrouch(false);
	CheckRun();
}
void AMultiCharacter::ServerRun()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeedModifier * DefaultRunSpeedModifer;
	bRunning = true;
}
void AMultiCharacter::ServerWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
	bRunning = false;
}
void AMultiCharacter::UpdateRun()
{
	if (bCanRun && bWantToRun)
	{
		ResetLeaning();
		bChangingWeapon = false;
		//TODO:
		//Mesh->GetAnimInstance()->Montage_Stop(0.3f, NULL);
		//GetPlayerControllerFromNetId(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(NULL, 1.f, ECameraAnimPlaySpace::CameraLocal, FRotator::ZeroRotator);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeedModifier * DefaultRunSpeedModifer;
		ServerRun();
	}
}
void AMultiCharacter::ResetLeaning()
{
	Targetlean = 0;
	ServerResetLeaning();
}
void AMultiCharacter::ServerResetLeaning()
{
	bLeaningRight = false;
	bLeaningLeft = false;
}
void AMultiCharacter::ServerSetLeaning(bool bRight, bool bIsLeaning)
{
	if (bRight)
	{
		if (bIsLeaning)
		{
			bLeaningRight = false;
		} 
		else
		{
			bLeaningRight = true;
			bLeaningLeft = false;
		}
	}
	else
		if (bIsLeaning)
		{
			bLeaningLeft = false;
		} 
		else
		{
			bLeaningLeft = true;
			bLeaningRight = false;
		}
}

void AMultiCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	swayHorizontalTarget = FMath::Clamp((Rate*2.f)-90.f, -94.f, -86.f);
	if (bAiming)
	{
		AddControllerYawInput(Rate * ZoomSensitivity);
	}
	else
	{
		AddControllerYawInput(Rate);
	}
	if (AutoShootTimerHandle.IsValid())
	{
		if (Rate != 0 && GetWorld()->GetTimerManager().IsTimerActive(AutoShootTimerHandle))
		{
			yawAcum = 0;
			actualYaw = 0;
		}
	}
}
void AMultiCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	float zoom = Rate;
	swayVerticalTarget = FMath::Clamp((Rate * -2.f), -1.f, 1.f);
	if (!bDead)
	{
		if (bLeaningLeft || bLeaningRight)
		{
			if (bAiming)
			{
				zoom = Rate * ZoomSensitivity;
			}
			else
			{
				zoom = Rate;
			}
			if (bLeaningRight)
			{
				AddControllerYawInput(FMath::Sin(LeanAngle) * -zoom);
			}
			else
			{
				AddControllerYawInput(FMath::Sin(LeanAngle) * zoom);
			}
		}
		AddControllerPitchInput(zoom);
		if (UKismetSystemLibrary::K2_IsTimerActiveHandle(GetWorld(), AutoShootTimerHandle))
		{
			if ((zoom + pitchAcum) > 0.f && pitchAcum == 0)
			{
				ResetRecoil();
				actualPitch = 0;
				pitchAcum = 0;
			}
			else
			{
				pitchAcum > 0 ? pitchAcum = 0 : pitchAcum += zoom;
				actualPitch += zoom;
			}
		}
		else
		{
			if (zoom != 0)
			{
				ResetRecoil();
				actualPitch = 0;
				pitchAcum = 0;
			}
		}
	}
}
void AMultiCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		/*
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);*/
		CancelInteract();
	}
	AddMovementInput(this->GetActorForwardVector(), Value, false);
	bMovingforward = !(Value < 0);
	if ((this->GetVelocity().Size() > 250) && Value > 0 && !UKismetSystemLibrary::K2_IsTimerActiveHandle(GetWorld(), AutoShootTimerHandle))
	{
		bCanRun = Value > 0;
		CheckRun();
		CheckAim();
	}
	else
	{
		bCanRun = false;
		if (bRunning)
		{
			CheckRun();
			CheckAim();
		}
	}
}
void AMultiCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		float speed = bRunning ? Value / 3 : Value;
		AddMovementInput(this->GetActorRightVector(), speed, false);
		CancelInteract();
	}
}
void AMultiCharacter::LeanRight()
{
	if (!bRunning && !bReloading)
	{
		if (bLeaningRight)
		{
			Targetlean = 0.f;
			ServerSetLeaning(true, true);
		}
		else
		{
			Targetlean = LeanAngle;
			ServerSetLeaning(true, false);
		}
		//TODO::PlaySound()
	}
}
void AMultiCharacter::LeanLeft()
{
	if (!bRunning && !bReloading)
	{
		if (bLeaningLeft)
		{
			Targetlean = 0.f;
			ServerSetLeaning(false, true);
		}
		else
		{
			Targetlean = -LeanAngle;
			ServerSetLeaning(false, false);
		}
	}
}
void AMultiCharacter::StartRun()
{
	bWantToRun = true;
	if (!bChangingWeapon && !bThrowingGrenade)
	{
		if (bIsCrouched)
		{
			ForceUncrouch();
		}
		//TODO: hold breath and delay
		CheckRun();
	}
}
void AMultiCharacter::StopRun()
{
	bWantToRun = false;
	//TODO:stopholdbreath
	if (!bAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
		GetPlayerControllerFromNetId(GetWorld(), 0)->PlayerCameraManager->StopAllCameraShakes(true);
		ServerWalk();
		CheckAim();
	}
}
void AMultiCharacter::CrouchFunction()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		if (bWantsToCrouch)
		{
			ForceUncrouch();
		}
		else
		{
			bWantsToCrouch = true;
			Crouch();
			if (!bThrowingGrenade && !bAiming && !bChangingWeapon && !bReloading && !bAttacking)
			{
				//TODO::
			}
			else
			{
				CheckRun();
			}
		}
	}
}
void AMultiCharacter::JumpFunc()
{
	Jump();
	bWantToJump = CanJump();
}
void AMultiCharacter::StopJumpingFunc()
{
	StopJumping();
	bWantToJump = false;
}

