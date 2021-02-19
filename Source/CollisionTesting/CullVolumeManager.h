// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Engine/CullDistanceVolume.h"
#include "GameFramework/GameUserSettings.h"
#include "CullVolumeManager.generated.h"

UCLASS()
class COLLISIONTESTING_API ACullVolumeManager : public AActor
{
	GENERATED_BODY()

	//bool b_cullVolumeModified;

public:	
	// Sets default values for this actor's properties
	ACullVolumeManager();

	/*UPROPERTY(EditAnywhere)
	ACullDistanceVolume* cullVolume;

	UPROPERTY(EditAnywhere)
		float cullChangeFactor = 2.0f;*/


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/*UFUNCTION(BlueprintCallable)
		void LowerCullDistances();

	UFUNCTION(BlueprintCallable)
		void RaiseCullDistances();

	UFUNCTION(BlueprintCallable)
		void SetCullDistances(float newDistance);*/

	UFUNCTION(BlueprintCallable)
		void UpdateViewDistance(int QualityLevel);

};
