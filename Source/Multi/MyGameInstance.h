// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SettingWidget.h"
#include "EndGameMenu.h"
#include "ErrorMessageMenu.h"
#include "OnlineSessionSettings.h"
#include "MyGameInstance.generated.h"

UCLASS()
class MULTI_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UPROPERTY(EditDefaultsOnly, Category=UI)
    TSubclassOf<UUserWidget> MainMenuTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> LoadingScreenTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> HostMenuTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> JoinMenuTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<USettingWidget> SettingMenuTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> GameplayWidgetTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> InGameMenuTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UEndGameMenu> EndgameTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> SearchGameTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> CantFindgameWidgetTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> playerScoreTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UErrorMessageMenu> ErrorMessageTemplate;
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> DeathMatchEndTemplete;
	UUserWidget* MainMenu;
    UUserWidget* LoadingScreen;
	UUserWidget* HostMenuRef;
	UUserWidget* JoinMenuRef;
	UUserWidget* GameplayWidgetRef;
	UUserWidget* InGameMenuRef;
	UUserWidget* SearchGameWidget;
	UUserWidget* CantFindGameRef;
	UUserWidget* PlayerScoreWidget;
	UUserWidget* DeathMatchEnd;
	UEndGameMenu* EndGameScoreRef;
	USettingWidget* SettingMenuRef;
	UErrorMessageMenu* ErrorMsg;
public:
    UFUNCTION(BlueprintCallable, Category="UI")
    void showMainMenu();
    UFUNCTION(BlueprintCallable, Category = "UI")
    void showLoadingScreen();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showHostMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showJoinMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showSettingMenu(bool bInGame);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showGameplayWidget();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showInGameMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showEndGameMenu(bool bWinnerTeam, float timeForNextMatch);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showSearchMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showCantFindGameMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showPlayerScores();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showErrorMessageMenu(FString &Msg);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void showDeathmatchEndGame(float timeForNextMatch);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseInGameMenu();

protected:
	IOnlineSessionPtr SessionInterface;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName, bool);
private:
	int TeamAmatchesWon = 0;
	int TeamBmatchesWon = 0;
	FName levelName;
	//UPROPERTY(Replicated)
	int maxPlayers = 32;
public:
	UMyGameInstance();
	
	bool GameWithBots = false;
	int TeamAbots = 0, TeamBbots = 0;

	UFUNCTION(BlueprintCallable, Category = "Server")
	void HostServer(int maxP, FName mName, bool lan);
	UFUNCTION(BlueprintCallable, Category = "Server")
	void CreateSinglePlayerGame();
	//UFUNCTION(BlueprintCallable, Category = "Server")
	void JoinServer(FOnlineSessionSearchResult &searchResult);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResetGlobalScores();
	UFUNCTION(BlueprintCallable, Category = "Game")
	void AddPoints(bool isB);
	void CheckScores();
};
