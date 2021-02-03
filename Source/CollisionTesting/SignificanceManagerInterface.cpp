// Fill out your copyright notice in the Description page of Project Settings.


#include "SignificanceManagerInterface.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Add default functionality here for any ISignificanceManagerInterface functions that are not pure virtual.

float ISignificanceManagerInterface::GetSignificanceByDistance(float distance)
{
	const int32 NumThresholds = SignificanceSettings.SignificanceThresholds.Num();
	if (distance >= SignificanceSettings.SignificanceThresholds[NumThresholds - 1].MaxDistance)
	{
		return SignificanceSettings.SignificanceThresholds[NumThresholds - 1].Significance;
	}
	else
	{
		for (int32 i = 0; i < NumThresholds; i++)
		{
			FSignificanceSettings::FSignificanceThresholds& Thresholds = SignificanceSettings.SignificanceThresholds[i];
			if (distance <= Thresholds.MaxDistance)
			{
				return Thresholds.Significance;
			}
		}
	}

	return 0.0f;
}

void ISignificanceManagerInterface::AddSignificanceThreshold(float significance, float maxDistance)
{
	SignificanceSettings.SignificanceThresholds.Emplace(significance, maxDistance);
}

void ISignificanceManagerInterface::RegeisterWithSignificancemanager(UObject* ObjectToRegister, const UWorld* World, FName Tag)
{

	USignificanceManager* SignificanceManager = USignificanceManager::Get(World);
	if (SignificanceManager)
	{
		auto Significance = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint) -> float
		{
			return SignificanceFunction(ObjectInfo, ViewPoint);
		};

		auto PostSignificance = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
		{
			return PostSignificanceFunction(ObjectInfo, OldSignificance, Significance, bFinal);
		};

		SignificanceManager->RegisterObject(ObjectToRegister, Tag, Significance, USignificanceManager::EPostSignificanceType::Sequential, PostSignificance);
	}
}
