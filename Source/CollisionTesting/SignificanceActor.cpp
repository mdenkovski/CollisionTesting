// Fill out your copyright notice in the Description page of Project Settings.


#include "SignificanceActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASignificanceActor::ASignificanceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AddSignificanceThreshold(2.0, 500.0f);
	AddSignificanceThreshold(1.0f, 800.0f);
	AddSignificanceThreshold(0.0f, 1200.0f);

	Tags.Add("SignificanceActor");
}

// Called when the game starts or when spawned
void ASignificanceActor::BeginPlay()
{
	Super::BeginPlay();
	

	TArray<UStaticMeshComponent*> Components;
	this->GetComponents<UStaticMeshComponent>(Components); //get the static mesh components
	if (Components.Num() > 0) //make sure that there is a mesh component that was obtained
	{
		UStaticMeshComponent* StaticMeshComponent = Components[0]; //only using the first static mesh component. if there are multiple meshes they will be ignored so only have one on the actor
		//create the dynamic material instances for the materials that need to be dynamic
		for (int i = 0; i < MaterialIndicesToChange.Num(); i++)
		{
			int index = MaterialIndicesToChange[i];
			DynamicMaterials.Add(StaticMeshComponent->CreateDynamicMaterialInstance(MaterialIndicesToChange[i], StaticMeshComponent->GetMaterial(MaterialIndicesToChange[i])));
		}
	}

	if (!IsNetMode(NM_DedicatedServer))
	{
		RegeisterWithSignificancemanager(this, GetWorld(), TEXT("SignificanceActor"));
	}
	
}



void ASignificanceActor::PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
{
	if (OldSignificance == Significance) return;

	if (ObjectInfo->GetTag() == TEXT("SignificanceActor"))
	{
		ASignificanceActor* SignificanceActor = CastChecked<ASignificanceActor>(ObjectInfo->GetObject());

		if (Significance == 0.0f) //lowest significance
		{
			//change the qulaity of each dynamic material we need to update to 0
			for (int i = 0; i < SignificanceActor->MaterialIndicesToChange.Num(); i++)
			{
				SignificanceActor->DynamicMaterials[i]->SetScalarParameterValue("Quality", 0);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Significance 0"));
			}

		}
		else if (Significance == 1.0f)
		{
			//change the qulaity of each dynamic material we need to update to 1
			for (int i = 0; i < SignificanceActor->MaterialIndicesToChange.Num(); i++)
			{
				SignificanceActor->DynamicMaterials[i]->SetScalarParameterValue("Quality", 1);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Significance 1"));
			}
		}
		else if (Significance == 2.0f)
		{
			//change the qulaity of each dynamic material we need to update to 2
			for (int i = 0; i < SignificanceActor->MaterialIndicesToChange.Num(); i++)
			{
				SignificanceActor->DynamicMaterials[i]->SetScalarParameterValue("Quality", 2);
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Significance 2"));
			}
		}
	}

}

float ASignificanceActor::SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint)
{
	if (ObjectInfo->GetTag() == TEXT("SignificanceActor"))
	{
		ASignificanceActor* SignificanceActor = CastChecked<ASignificanceActor>(ObjectInfo->GetObject());

		const float Distance = (SignificanceActor->GetActorLocation() - ViewPoint.GetLocation()).Size();

		return GetSignificanceByDistance(Distance);

	}

	return 0.0f;
}

