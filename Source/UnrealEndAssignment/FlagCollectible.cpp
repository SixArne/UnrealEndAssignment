// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagCollectible.h"
#include "DynamicTerrainBase.h"
#include "EngineUtils.h"

// Sets default values
AFlagCollectible::AFlagCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create box collider
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Detector"));
	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATIC MESH"));

	SetRootComponent(m_StaticMeshComponent);
	
	m_BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AFlagCollectible::OnConstruction(const FTransform& transform)
{
	// Set extent to blueprint values
	m_BoxComponent->SetBoxExtent(BoxSize);
	m_StaticMeshComponent->SetStaticMesh(m_StaticMesh);

	m_BoxComponent->SetVisibility(true);
	m_BoxComponent->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AFlagCollectible::BeginPlay()
{
	Super::BeginPlay();

	m_BoxComponent->SetBoxExtent(BoxSize);

	m_BoxComponent->SetVisibility(true);
	m_BoxComponent->SetHiddenInGame(false);

	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlagCollectible::OnOverlapBegin);

	// Find all meshes and save them locally.
	FindAllDynamicMeshes();
}

// Called every frame
void AFlagCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlagCollectible::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Flag got taken"));

	// Inform meshes that flag was taken
	ArmAllDynamicMeshes(OtherActor);

	OnFlagCollectedDelegate.Broadcast();

	// D
	this->SetActorEnableCollision(false);
	m_StaticMeshComponent->SetVisibility(false);
}

/************************************************************************/
/* Tells all meshes to start watching out for the player                */
/************************************************************************/
void AFlagCollectible::ArmAllDynamicMeshes(AActor* actor)
{
	for (auto& mesh : m_DynamicMeshes)
	{
		mesh->StartListeningForPlayer(actor);
	}
}

void AFlagCollectible::FindAllDynamicMeshes()
{
	for (TActorIterator<ADynamicTerrainBase> it{ GetWorld() }; it; ++it)
	{
		auto dynamicTerrainMesh = *it;
		m_DynamicMeshes.Add(dynamicTerrainMesh);
	}
}