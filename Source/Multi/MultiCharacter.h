// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/EngineTypes.h"
#include "MultiCharacter.generated.h"

UCLASS(config=Game)
class AMultiCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMultiCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	float swayHorizontalTarget = 0;
	float swayVerticalTarget = 0;
	float DefaulCamerahieght;
	float AimSensitivityMultiplier;
	bool IsHeadshot;
	float AimSpeedDivider;
	//move to player controller
	float ZoomSensitivity;


	//Timer
	FTimerHandle AutoShootTimerHandle;
	FTimerHandle InteractTimer;
	FTimerHandle InteractingTiemr;
	FTimerHandle LeaveProtectedZoneHandle;
	FTimerHandle GrenadeTimerHandle;
	FTimerHandle ActivateRegenTimerHandle;
	FTimerHandle RegenTimerHandle;
	FTimerHandle GrenadeTimeHandler;
	//Recoil
	float yawAcum = 0;
	float pitchAcum = 0;
	float actualYaw = 0;
	float actualPitch = 0;
	float InterpSpeed = 0;
	//Impact
	FVector ImpactPoint;
	//CharacterInfo
	bool bDead = false;
	bool bReloading = false;
	bool bInteracting = false;
	bool bCanShoot = false;
	bool bRunning = false;
	bool bWantToJump = false;
	bool bAiming = false;
	bool bCanRun = false;
	bool bFirstShoot = false;
	bool bWantToRun = false;
	bool bWantToAim = false;
	bool bAimingAtAlly = false;
	bool bChangingWeapon = false;
	bool bPawnTeam = false;
	bool bIsInSafeZone = true;
	bool bWantsToCrouch = false;
	bool bRecentlyDamaged = false;
	bool bThrowingGrenade = false;
	bool bCanUseGrendare = false;
	bool bCanUseMelle = false;
	bool bAttacking = false;
	bool bOnVahicle = false;
	bool bFiringBurst = false;
	bool bLeaningLeft = false;
	bool bLeaningRight = false;
	bool bAimingAtEnemy = false;
	bool bCanUseSkill = false;
	bool bMovingforward = false;
	//customvariables
	float InteractRange;
	float DefaultMovementSpeed;
	float RunSpeedModifier;
	float Delta;
	float DefautFOV;
	float CurrentFOV;
	float InteractTime;
	float DefaultRunSpeedModifer;
	float ActualLeanAngle;
	float MelleAttackRange;
	float OnHitDamage;
	float Targetlean;
	float DefaultCrouchSpeed;
	float TargetFov;
	float TargetSpread;
	float Saturation;
	float AbilityCoolDown;
	float LeanAngle;
	FName PlayerName;
	int BulletsFired;

protected:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void LeanRight();
	void LeanLeft();
	void StartRun();
	void StopRun();
	void CrouchFunction();
	void JumpFunc();
	void StopJumpingFunc();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void ResetRecoil();
	void CancelInteract();
	void CheckRun();
	void CheckAim();
	void ForceUncrouch();
	void HoldBreath();
	void ResetLeaning();
	void ServerRun();
	void StopHoldBreath();
	void ServerWalk();
	void UpdateRun();
	void ServerResetLeaning();
	void ServerSetLeaning(bool bRight, bool bIsLeaning);
	
};

