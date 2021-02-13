// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshActor.h"

// Sets default values
AMeshActor::AMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	staticMesh->AttachTo(Root);
	staticMesh->SetMobility(EComponentMobility::Static);

	sphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollisionCheck");
	sphereCollision->SetSphereRadius(200.0f);
	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeshActor::OnSphereBeginOverlap);
	sphereCollision->OnComponentEndOverlap.AddDynamic(this, &AMeshActor::OnSphereEndOverlap);
	sphereCollision->AttachTo(Root);
	sphereCollision->ComponentTags.Add("CustomCollisionMesh");
	sphereCollision->SetMobility(EComponentMobility::Static);
	

	RootComponent->SetMobility(EComponentMobility::Static);

	//Tags.Add("TestTag");
}

// Called when the game starts or when spawned
void AMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshActor::DisableCollision()
{
	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeshActor::EnableCollision()
{
	staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeshActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		staticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Collision Enabled");
	}
}

void AMeshActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Collision Disabled");
	}
}

