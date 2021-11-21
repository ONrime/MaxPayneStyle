// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Public/Stage/MaxStageGameMode.h"
#include "Player/Public/PlayerCharacter.h"
#include "Game/Public/Stage/Max_PlayerController.h"

AMaxStageGameMode::AMaxStageGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMax_PlayerController::StaticClass();
	//GameStateClass = ATwoVersus_GameState::StaticClass();
	//PlayerStateClass = ATwoVersus_PlayerState::StaticClass();
	//HUDClass = AMultiPlayer_HUD::StaticClass();
}
