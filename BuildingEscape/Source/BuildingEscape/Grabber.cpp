// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    // Look for attached physics handle
    m_physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (m_physicsHandle)
    {
        
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Physics Handle found on %s."), *GetOwner()->GetName());
    }

    // Look for attached input component
    m_inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (m_physicsHandle)
    {
        m_inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        m_inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Input Component found on %s."), *GetOwner()->GetName());
    }
	
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Error, TEXT("Grab Pressed!"));
}

void UGrabber::Drop()
{
    UE_LOG(LogTemp, Error, TEXT("Grab Released!"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector playerVec;
    FRotator playerRot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVec, playerRot);

    //UE_LOG(LogTemp, Warning, TEXT("vec: %s, rot: %s"), *playerVec.ToString(), *playerRot.ToString());
    
    FVector lineTraceEnd = playerVec + playerRot.Vector() * m_reach;
    DrawDebugLine(
        GetWorld(),
        playerVec,
        lineTraceEnd,
        FColor::Red,
        false,
        0.f,
        0.f,
        10.f
    );

	// Setup query parameters
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast out to reach distance
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		hit,
		playerVec,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParameters
	);

	AActor* pActorHit = hit.GetActor();
	if(pActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s hit."), *(pActorHit->GetName()));
	}
}

