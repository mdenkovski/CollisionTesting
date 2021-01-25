// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "CustomGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class COLLISIONTESTING_API UCustomGameViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()
	

public: 
	virtual void Tick(float DeltaTime) override;


protected:
	//cached pointer to our player controller
	TWeakObjectPtr<class APlayerController> CachedPlayerController;
};
