// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignificanceManagerInterface.h"
#include "SignificanceActor.generated.h"

UCLASS()
class COLLISIONTESTING_API ASignificanceActor : public AActor, public ISignificanceManagerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASignificanceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


	// Inherited via ISignificanceManagerInterface
	virtual void PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal) override;

	virtual float SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint);


	//UPROPERTY(EditAnywhere)
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditAnywhere, Category = Dynamic_Materials)
	TArray<int> MaterialIndicesToChange;

};
