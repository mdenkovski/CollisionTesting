// Fill out your copyright notice in the Description page of Project Settings.


#include "SignificanceMeshActor.h"

ASignificanceMeshActor::ASignificanceMeshActor()
{
	AddSignificanceThreshold(2.0, 500.0f);
	AddSignificanceThreshold(1.0f, 800.0f);
	AddSignificanceThreshold(0.0f, 1200.0f);

	Tags.Add("Mesh");
}

void ASignificanceMeshActor::PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
{
	if (OldSignificance == Significance) return;

	if (ObjectInfo->GetTag() == TEXT("Mesh"))
	{
		ASignificanceMeshActor* Mesh = CastChecked<ASignificanceMeshActor>(ObjectInfo->GetObject());

		if (Significance == 0.0f) //lowest significance
		{
			//change the qulaity of each dynamic material we need to update to 0
			for (int i = 0; i < MaterialIndicesToChange.Num(); i++)
			{
				DynamicMaterials[i]->SetScalarParameterValue("Quality", 0);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Significance 0"));
			}

		}
		else if (Significance == 1.0f)
		{
			//change the qulaity of each dynamic material we need to update to 1
			for (int i = 0; i < MaterialIndicesToChange.Num(); i++)
			{
				DynamicMaterials[i]->SetScalarParameterValue("Quality", 1);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Significance 1"));
			}
		}
		else if (Significance == 2.0f)
		{
			//change the qulaity of each dynamic material we need to update to 2
			for (int i = 0; i < MaterialIndicesToChange.Num(); i++)
			{
				DynamicMaterials[i]->SetScalarParameterValue("Quality", 2);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Significance 2"));
			}
		}
	}
}

float ASignificanceMeshActor::SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewPoint)
{
	if (ObjectInfo->GetTag() == TEXT("Mesh"))
	{
		ASignificanceMeshActor* Mesh = CastChecked<ASignificanceMeshActor>(ObjectInfo->GetObject());

		const float Distance = (Mesh->GetActorLocation() - ViewPoint.GetLocation()).Size();

		return GetSignificanceByDistance(Distance);

	}

	return 0.0f;
}

void ASignificanceMeshActor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < MaterialIndicesToChange.Num(); i++)
	{
		int index = MaterialIndicesToChange[i];
		DynamicMaterials.Add(staticMesh->CreateDynamicMaterialInstance(MaterialIndicesToChange[i], staticMesh->GetMaterial(MaterialIndicesToChange[i])));
	}




	if (!IsNetMode(NM_DedicatedServer))
	{
		RegeisterWithSignificancemanager(this, GetWorld(), TEXT("Mesh"));
	}
}
