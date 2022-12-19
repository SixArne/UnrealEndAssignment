// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTerrainBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADynamicTerrainBase::ADynamicTerrainBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make component tree
	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Detector"));
	m_DebugBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("DebugBillboard"));

	// Set root component
	RootComponent = m_StaticMeshComponent;

	// Attach
	m_BoxComponent->SetupAttachment(m_StaticMeshComponent);
	m_BoxComponent->SetupAttachment(m_DebugBillboard);
}

void ADynamicTerrainBase::OnConstruction(const FTransform& Transform)
{
	// Set variable default to world location
	m_StaticMeshComponent->SetStaticMesh(Mesh);
	m_BoxComponent->SetBoxExtent(DetectorBounds);
	m_BoxComponent->SetWorldLocation(this->GetActorLocation() + DetectorOffset);

	m_DebugBillboard->SetSprite(DebugSprite);

	m_DebugBillboard->SetWorldLocation(this->GetActorLocation() + DetectorOffset);
}

// Called when the game starts or when spawned
void ADynamicTerrainBase::BeginPlay()
{
	Super::BeginPlay();

	// Bind delegate
	//OnPlayerDelegate.AddDynamic(this, &ADynamicTerrainBase::OnPlayerDetectedCallback);

	// Cache begin position
	m_BeginPosition = this->GetActorLocation();

	m_BoxComponent->SetHiddenInGame(false);
	m_BoxComponent->SetVisibility(true);
	m_BoxComponent->SetBoxExtent(DetectorBounds);
	m_BoxComponent->SetWorldLocation(this->GetActorLocation() + DetectorOffset);

	// Setup collision detection
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADynamicTerrainBase::OnOverlapBegin);
}

// Called every frame
void ADynamicTerrainBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_ListeningForPlayer)
	{
		return;
	}

	if (m_IsFalling)
	{
		FHitResult hitResult{};
		FVector start = this->GetActorLocation();
		FVector end = start + (FVector::DownVector * RayLength);

		FCollisionQueryParams collisionParams{};
		collisionParams.AddIgnoredActor(this->GetOwner());

		UE_LOG(LogTemp, Warning, TEXT("Raycasting"));
		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 10.f, 0, 1);
		bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);

		if (isHit)
		{
			OnReachedBottomDelegate.Broadcast();

			m_IsFalling = false;
			m_ListeningForPlayer = false;
			m_StaticMeshComponent->SetSimulatePhysics(false);
		}
	}
}

void ADynamicTerrainBase::StartListeningForPlayer(AActor* player)
{
	m_ListeningForPlayer = true;
	m_PlayerCharacter = player;
}


void ADynamicTerrainBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (m_ListeningForPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player detected"));
		m_StaticMeshComponent->SetSimulatePhysics(true);

		m_IsFalling = true;

		OnPlayerDelegate.Broadcast();
	}
}
