// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
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
    FindPhysicsHandleComponent();

    // Look for attached input component
    SetupInputComponent();
	
}

void UGrabber::SetupInputComponent()
{
    m_inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (m_inputComponent)
    {
        m_inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        m_inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Input Component found on %s."), *GetOwner()->GetName());
    }
}

void UGrabber::FindPhysicsHandleComponent()
{
    m_physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (nullptr == m_physicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("No Physics Handle found on %s."), *GetOwner()->GetName());
    }
}

void UGrabber::Grab()
{
    // LINE TRACE and see if we reach any actors with physics body collision channel set
    auto hitResult = GetFirstPhysicsBodyInReach();
    auto componentToGrab = hitResult.GetComponent();
    auto actorHit = hitResult.GetActor();

    // attach physics handle
    if (actorHit)
    {
        m_physicsHandle->GrabComponent(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation(), true);
    }
}

void UGrabber::Drop()
{
    // release physics handle
    m_physicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Setup query parameters
    FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

    /// Ray-cast out to reach distance
    FHitResult hitResult;
    GetWorld()->LineTraceSingleByObjectType(
        hitResult,
        GetReachLineStart(),
        GetReachLineEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        traceParameters
    );

    return hitResult;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 

    // If physics handle is attached move the object we're holding
    if (m_physicsHandle->GrabbedComponent)
    {
        // move the object that we're holding
        m_physicsHandle->SetTargetLocation(GetReachLineEnd());
    }   
}

FVector UGrabber::GetReachLineStart() const
{
    FVector playerVec;
    FRotator playerRot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVec, playerRot);
    
    return playerVec;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector playerVec;
    FRotator playerRot;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerVec, playerRot);

    return playerVec + playerRot.Vector() * m_reach;
}

