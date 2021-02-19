// Fill out your copyright notice in the Description page of Project Settings.


#include "CullVolumeManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ACullVolumeManager::ACullVolumeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//b_cullVolumeModified = false;

}

// Called when the game starts or when spawned
void ACullVolumeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

//// Called every frame
//void ACullVolumeManager::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

//void ACullVolumeManager::LowerCullDistances()
//{
//	if (cullVolume == nullptr) return; //only proceed if we have a valid cull volume
//
//	if (!b_cullVolumeModified)
//	{
//		
//			for (int i = 0; i < cullVolume->CullDistances.Num(); i++) //iterate through each cull distance pair
//			{
//				if (cullVolume->CullDistances[i].CullDistance != 0.0f) //check that the distance is one we want to halve
//				{
//					cullVolume->CullDistances[i].CullDistance = cullVolume->CullDistances[i].CullDistance / cullChangeFactor; //halve the value of each cull distance
//					float distance = cullVolume->CullDistances[i].CullDistance;
//					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Cull Distance changed to: %f"), distance));
//				}
//			}
//		
//		b_cullVolumeModified = true;
//		GWorld->UpdateCullDistanceVolumes();
//	}
//}
//
//void ACullVolumeManager::RaiseCullDistances()
//{
//	if (cullVolume == nullptr) return; //only proceed if we have a valid cull volume
//
//	if (b_cullVolumeModified)
//	{
//		
//			for (int i = 0; i < cullVolume->CullDistances.Num(); i++) //iterate through each cull distance pair
//			{
//				if (cullVolume->CullDistances[i].CullDistance != 0.0f) //check that the distance is one we want to halve
//				{
//					cullVolume->CullDistances[i].CullDistance = cullVolume->CullDistances[i].CullDistance * cullChangeFactor; // double the value of each cull distance back to normal
//				}
//			}
//		
//		b_cullVolumeModified = false;
//		GWorld->UpdateCullDistanceVolumes();
//	}
//}
//
//
////modify cull distance values that do not have a 0 value for distance
//void ACullVolumeManager::SetCullDistances(float newDistance)
//{
//	if (cullVolume == nullptr) return; //only proceed if we have a valid cull volume
//
//	if (newDistance <= 0.0f)
//	{
//		return; //make sure there is a valid distance being applied
//	}
//
//	
//	for (int i = 0; i < cullVolume->CullDistances.Num(); i++) //iterate through each cull distance pair
//	{
//		if (cullVolume->CullDistances[i].CullDistance != 0.0f) //check that the distance is one we want to modify
//		{
//			cullVolume->CullDistances[i].CullDistance = newDistance; // set the cull distance to the new distance
//		}
//	}
//	
//	b_cullVolumeModified = false; // we are setting the new base value for the cull distance so this is an unmodified value
//	GWorld->UpdateCullDistanceVolumes();
//}

void ACullVolumeManager::UpdateViewDistance(int QualityLevel)
{
	if (QualityLevel < 0 || QualityLevel >= 4) return; //make sure the Quality level is within applicable bounds
	
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();

	GameSettings->SetViewDistanceQuality(QualityLevel);
	GameSettings->ApplySettings(false);


}

