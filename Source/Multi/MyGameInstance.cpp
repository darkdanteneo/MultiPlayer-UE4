// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"

UMyGameInstance::UMyGameInstance()
{

}
void UMyGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
		}
	}
}
void UMyGameInstance::OnCreateSessionComplete(FName ServerName, bool bIsSucess)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeed: %d"),bIsSucess);
	UGameplayStatics::OpenLevel(GetWorld(), levelName, true, FString(TEXT("listen")));
}

void UMyGameInstance::showMainMenu()
{
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	if (MainMenu == nullptr){	MainMenu = CreateWidget(GetWorld(), MainMenuTemplate);	}
	MainMenu->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::showLoadingScreen()
{
	if (LoadingScreen == nullptr) { LoadingScreen = CreateWidget(GetWorld()->GetFirstPlayerController(), LoadingScreenTemplate); }
	LoadingScreen->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}
void UMyGameInstance::showHostMenu()
{
	if (HostMenuRef == nullptr) { HostMenuRef = CreateWidget(GetWorld()->GetFirstPlayerController(), HostMenuTemplate); }
	HostMenuRef->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
void UMyGameInstance::showJoinMenu()
{
	if (JoinMenuRef == nullptr) { JoinMenuRef = CreateWidget(GetWorld()->GetFirstPlayerController(), JoinMenuTemplate); }
	JoinMenuRef->AddToViewport();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
void UMyGameInstance::showSettingMenu(bool bGame)
{
	if (SettingMenuRef == nullptr){	SettingMenuRef = CreateWidget<USettingWidget>(GetWorld()->GetFirstPlayerController(), SettingMenuTemplate);	}
	SettingMenuRef->bInGame = bGame;
	if (!SettingMenuRef->IsInViewport()) { SettingMenuRef->AddToViewport(999); }
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	SettingMenuRef->SetVisibility(ESlateVisibility::Visible);
	SettingMenuRef->loadSettingWidget();
}
void UMyGameInstance::showGameplayWidget()
{
	if (GameplayWidgetRef == nullptr) { GameplayWidgetRef = CreateWidget(GetWorld()->GetFirstPlayerController(),GameplayWidgetTemplate); }
	GameplayWidgetRef->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	//set input mode game only
	FInputModeUIOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::showInGameMenu()
{
	if (InGameMenuRef == nullptr) { InGameMenuRef = CreateWidget(GetWorld()->GetFirstPlayerController(), InGameMenuTemplate); }
	if (!InGameMenuRef->IsInViewport()) { InGameMenuRef->AddToViewport(999); }
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(InGameMenuRef->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	InGameMenuRef->SetVisibility(ESlateVisibility::Visible);
}
void UMyGameInstance::showEndGameMenu(bool bWinnerTeam, float timeForNextMatch)
{
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	EndGameScoreRef = CreateWidget<UEndGameMenu>(GetWorld()->GetFirstPlayerController(), EndgameTemplate);
	EndGameScoreRef->bWin = bWinnerTeam;
	EndGameScoreRef->timeForNextMatch = timeForNextMatch;
	EndGameScoreRef->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::showSearchMenu()
{
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	SearchGameWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), SearchGameTemplate);
	SearchGameWidget->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(SearchGameWidget->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::showCantFindGameMenu()
{
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	CantFindGameRef = CreateWidget(GetWorld()->GetFirstPlayerController(), CantFindgameWidgetTemplate);
	CantFindGameRef->AddToViewport(999);
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(CantFindGameRef->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::showPlayerScores()
{
	if (PlayerScoreWidget == nullptr) { PlayerScoreWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), playerScoreTemplate); }
	if (!PlayerScoreWidget->IsInViewport()) { PlayerScoreWidget->AddToViewport(); }
	PlayerScoreWidget->SetVisibility(ESlateVisibility::Visible);
}
void UMyGameInstance::showErrorMessageMenu(FString &Msg)
{
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	ErrorMsg = CreateWidget<UErrorMessageMenu>(GetWorld()->GetFirstPlayerController(), ErrorMessageTemplate);
	ErrorMsg->errorMsg = Msg;
	ErrorMsg->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(ErrorMsg->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::showDeathmatchEndGame(float timeForNextMatch)
{
	GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
	DeathMatchEnd = CreateWidget(GetWorld()->GetFirstPlayerController(), DeathMatchEndTemplete);
	DeathMatchEnd->AddToViewport(999);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	//set input mode ui only
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(ErrorMsg->TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
}
void UMyGameInstance::CloseInGameMenu()
{
	MainMenu->SetVisibility(ESlateVisibility::Visible);
}

void UMyGameInstance::HostServer(int maxP, FName mName, bool lan)
{
	maxPlayers = maxP;
	levelName = mName;
	showLoadingScreen();
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bIsLANMatch = lan;
	SessionSettings.bIsDedicated = false;
	SessionSettings.NumPublicConnections = maxPlayers;
	SessionInterface->CreateSession(0, mName, SessionSettings);
}
void UMyGameInstance::CreateSinglePlayerGame()
{
	GameWithBots = true;
	TeamAbots = 10;
	TeamBbots = 10;
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("conquest")), true);
}
void UMyGameInstance::JoinServer(FOnlineSessionSearchResult& searchResult)
{
	if (searchResult.Session.SessionSettings.NumPublicConnections - searchResult.Session.NumOpenPublicConnections > 0)
	{
		showLoadingScreen();
		JoinSession(GetLocalPlayerByIndex(0), searchResult);
	}
}

void UMyGameInstance::ResetGlobalScores()
{
	TeamBmatchesWon = 0;
	TeamAmatchesWon = 0;
}
void UMyGameInstance::AddPoints(bool isB)
{
	isB ? TeamBmatchesWon++ : TeamAmatchesWon++;
}
void UMyGameInstance::CheckScores()
{

}

