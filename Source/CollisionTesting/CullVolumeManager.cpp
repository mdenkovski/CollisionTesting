// Fill out your copyright notice in the Description page of Project Settings.


#include "CullVolumeManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ACullVolumeManager::ACullVolumeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	b_cullVolumeModified = false;

	
}

// Called when the game starts or when spawned
void ACullVolumeManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACullVolumeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACullVolumeManager::LowerCullDistances()
{
	if (!b_cullVolumeModified)
	{
		for (int i = 0; i < cullVolumes.Num(); i++) //iterate through each cull volume in our array
		{
			for (int j = 0; j < cullVolumes[i]->CullDistances.Num(); j++) //iterate through each cull distance pair
			{
				if (cullVolumes[i]->CullDistances[j].CullDistance != 0.0f) //check that the distance is one we want to halve
				{
					cullVolumes[i]->CullDistances[j].CullDistance = cullVolumes[i]->CullDistances[j].CullDistance / cullChangeFactor; //halve the value of each cull distance
					float distance = cullVolumes[i]->CullDistances[j].CullDistance;
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Cull Distance changed to: %f"), distance));
				}
			}
		}
		b_cullVolumeModified = true;
		GWorld->UpdateCullDistanceVolumes();
	}
}

void ACullVolumeManager::RaiseCullDistances()
{
	if (b_cullVolumeModified)
	{
		for (int i = 0; i < cullVolumes.Num(); i++) //iterate through each cull volume in our array
		{
			for (int j = 0; j < cullVolumes[i]->CullDistances.Num(); j++) //iterate through each cull distance pair
			{
				if (cullVolumes[i]->CullDistances[j].CullDistance != 0.0f) //check that the distance is one we want to halve
				{
					cullVolumes[i]->CullDistances[j].CullDistance = cullVolumes[i]->CullDistances[j].CullDistance * cullChangeFactor; // double the value of each cull distance back to normal
					//float distance = cullVolumes[i]->CullDistances[j].CullDistance;
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf( TEXT("Cull Distance changed to: %f"), distance));
				}
			}
		}
		FString name = "empty"; 
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetDebugName(Cast<AActor>( UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, (TEXT("Cull Distance changed by: %s"), name));
		b_cullVolumeModified = false;
		GWorld->UpdateCullDistanceVolumes();
	}
}


//modify cull distance values that do not have a 0 value for distance
void ACullVolumeManager::SetCullDistances(float newDistance)
{
	if (newDistance <= 0.0f)
	{
		return; //make sure there is a valid distance being applied
	}

	for (int i = 0; i < cullVolumes.Num(); i++) //iterate through each cull volume in our array
	{
		for (int j = 0; j < cullVolumes[i]->CullDistances.Num(); j++) //iterate through each cull distance pair
		{
			if (cullVolumes[i]->CullDistances[j].CullDistance != 0.0f) //check that the distance is one we want to modify
			{
				cullVolumes[i]->CullDistances[j].CullDistance = newDistance; // set the cull distance to the new distance
				//float distance = cullVolumes[i]->CullDistances[j].CullDistance;
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Cull Distance changed to: %f"), distance));
			}
		}
	}
	b_cullVolumeModified = false; // we are setting the new base value for the cull distance so this is an unmodified value
	GWorld->UpdateCullDistanceVolumes();
}

