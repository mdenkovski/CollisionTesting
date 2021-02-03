// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameViewportClient.h"
#include "SignificanceManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UCustomGameViewportClient::UCustomGameViewportClient(const FObjectInitializer& Objectinitializer)
	: Super(Objectinitializer)
{

}

void UCustomGameViewportClient::Tick(float DeltaTime)
{
    // Call the superclass' Tick function.
    Super::Tick(DeltaTime);

    // Ensure that we have a valid World and Significance Manager instance.
    if (UWorld* MyWorld = GetWorld())
    {

        

        if (USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(MyWorld))
        {
            // Update once per frame, using only Player 0's world transform.
            if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(MyWorld, 0))
            {
                // The Significance Manager uses an ArrayView. Construct a one-element Array to hold the Transform.
                TArray<FTransform> TransformArray;
                TransformArray.Add(PlayerPawn->GetTransform());
                // Update the Significance Manager with our one-element Array passed in through an ArrayView.
                SignificanceManager->Update(TArrayView<FTransform>(TransformArray));
            }
        }
    }
}













//possible alternative to have
/*const bool bHasValidData = CachedPlayerController.IsValid();
        if (!bHasValidData)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(MyWorld, 0);
            CachedPlayerController = TWeakObjectPtr<class APlayerController>(PC);
        }

        if (bHasValidData)
        {
            USignificanceManager* SignificanceManager = FSignificanceManagerModule::Get(MyWorld);
            if (SignificanceManager)
            {
                FVector ViewLocation;
                FRotator ViewRotation;
                CachedPlayerController.Get()->GetPlayerViewPoint(ViewLocation, ViewRotation);

                TArray<FTransform> ViewPoint;
                ViewPoint.Emplace(ViewRotation, ViewLocation, FVector::OneVector);

                SignificanceManager->Update(ViewPoint);
            }
        }*/