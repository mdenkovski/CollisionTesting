// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SignificanceManager.h"
#include "SignificanceManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USignificanceManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COLLISIONTESTING_API ISignificanceManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual float SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint) = 0;
	virtual void PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal) = 0;

	struct FSignificanceSettings
	{
		struct FSignificanceThresholds
		{
			FSignificanceThresholds(float InSignificance, float InMaxDistance) : Significance(InSignificance), MaxDistance(InMaxDistance) {}
			float Significance;
			float MaxDistance;
		};

		//currenly have it set to have 3 different significance thresholds. increase the allocator to add more
		TArray<FSignificanceThresholds, TInlineAllocator<3>> SignificanceThresholds;
	};

	FSignificanceSettings SignificanceSettings;

	virtual float GetSignificanceByDistance(float distance);
	virtual void AddSignificanceThreshold(float significance, float maxDistance);

	virtual void RegeisterWithSignificancemanager(UObject* ObjectToRegister, const UWorld* World);

};
