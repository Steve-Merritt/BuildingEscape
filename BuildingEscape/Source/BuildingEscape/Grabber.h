// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    void SetupInputComponent();

    void FindPhysicsHandleComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    float m_reach = 100.f;

    UPhysicsHandleComponent* m_physicsHandle = nullptr;

    UInputComponent* m_inputComponent = nullptr;

    void Grab();
    void Drop();

    // Return hit for first physics body in reach
    FHitResult GetFirstPhysicsBodyInReach() const;

    FVector GetReachLineStart() const;
    FVector GetReachLineEnd() const;
};
