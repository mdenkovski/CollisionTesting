// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshActor.h"
#include "SignificanceManagerInterface.h"
#include "SignificanceMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class COLLISIONTESTING_API ASignificanceMeshActor : public AMeshActor, public ISignificanceManagerInterface
{
	GENERATED_BODY()
	
public:
	//constructor
	ASignificanceMeshActor();

	// Inherited via ISignificanceManagerInterface
	virtual void PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal) override;

	virtual float SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint);



	void BeginPlay() override;

	//UPROPERTY(EditAnywhere)
		TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditAnywhere)
		TArray<int> MaterialIndicesToChange;
};
