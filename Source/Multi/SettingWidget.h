// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTI_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    bool bInGame;
    void loadSettingWidget();
};
