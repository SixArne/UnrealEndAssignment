// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicTerrainBase.h"
#include "Components/BoxComponent.h"

#include "FlagCollectible.generated.h"

UCLASS()
class UNREALENDASSIGNMENT_API AFlagCollectible : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFlagCollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 
	virtual void OnConstruction(const FTransform& transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoxSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* m_StaticMesh;
	
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
	
	void ArmAllDynamicMeshes(AActor* player);
	void FindAllDynamicMeshes();
	
	UStaticMeshComponent* m_StaticMeshComponent;
	UBoxComponent* m_BoxComponent;

	// holds all dynamic meshes
	TArray<ADynamicTerrainBase*> m_DynamicMeshes{};
};
