// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/CullDistanceVolume.h"
#include "CullVolumeManager.generated.h"

UCLASS()
class COLLISIONTESTING_API ACullVolumeManager : public AActor
{
	GENERATED_BODY()

		
	bool b_cullVolumeModified;

	
	
public:	
	// Sets default values for this actor's properties
	ACullVolumeManager();

	UPROPERTY(EditAnywhere)
	TArray<ACullDistanceVolume*> cullVolumes;

	UPROPERTY(EditAnywhere)
		float cullChangeFactor = 2.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void LowerCullDistances();
	void LowerCullDistances_Implementation();

	UFUNCTION(BlueprintCallable)
		void RaiseCullDistances();
	void RaiseCullDistances_Implementation();

	UFUNCTION(BlueprintCallable)
		void SetCullDistances(float newDistance);
	void SetCullDistances_Implementation(float newDistance);

};
