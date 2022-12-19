// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"

#include "DynamicTerrainBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedBottomSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDetectedSignature);


UCLASS()
class UNREALENDASSIGNMENT_API ADynamicTerrainBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicTerrainBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//bool m_Armed{ false };
	//float m_DetectionDistance{100.f};

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RayLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DetectorOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DetectorBounds { 50, 50, 50};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DebugSprite;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDetectedSignature OnPlayerDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnReachedBottomSignature OnReachedBottomDelegate;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	void StartListeningForPlayer(AActor* player);

	// Collision
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);	

private:
	UStaticMeshComponent* m_StaticMeshComponent{};
	UBoxComponent* m_BoxComponent{};
	UBillboardComponent* m_DebugBillboard{};

	FVector m_BeginPosition{};
	AActor* m_PlayerCharacter{};
	
	bool m_ListeningForPlayer{ false };
	bool m_IsFalling{ false };
};
