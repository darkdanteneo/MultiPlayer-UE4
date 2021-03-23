// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTI_API UEndGameMenu : public UUserWidget
{
	GENERATED_BODY()
public:
    bool bWin;
    float timeForNextMatch;
};
