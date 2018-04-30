// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

    //m_pPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();

    // Start with the door closed (180 degrees)
    m_pOwner = GetOwner();
    FRotator rot = m_pOwner->GetActorRotation();
    rot.Yaw = 180.0f;
    m_pOwner->SetActorRotation(rot);
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Poll the trigger volume
    // If Player steps in the volume, open the door.
    static bool bOpenDoor = false; 
    if (GetTotalMassOfActorsOnPlate() > 30.f) // TODO make weight into parameter
    {
        bOpenDoor = true;
    }
    else
    {
        bOpenDoor = false;
    }

    if (bOpenDoor)
        OpenDoor(DeltaTime);
    else
        CloseDoor(DeltaTime);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{ 
    FRotator rot = m_pOwner->GetActorRotation();
    
    if (rot.Yaw > 90.0f || rot.Yaw < 0)
    {
        rot.Yaw -= DeltaTime * m_doorSpeed;
        FMath::Clamp(rot.Yaw, 90.0f, 180.0f);
        m_pOwner->SetActorRotation(rot);
    }

    // UE_LOG(LogTemp, Warning, TEXT("%s Yaw is %f"), *pOwner->GetName(), rot.Yaw);    
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
    FRotator rot = m_pOwner->GetActorRotation();

    if (FMath::Abs(rot.Yaw) < 179.0f)
    {
        rot.Yaw += DeltaTime * m_doorSpeed * 1.5;
        FMath::Clamp(rot.Yaw, 90.0f, 180.0f);
        m_pOwner->SetActorRotation(rot);
    }

    //UE_LOG(LogTemp, Warning, TEXT("%s Yaw is %f"), *pOwner->GetName(), rot.Yaw);  
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float totalMass = 0.f;

    // Find all overlapping actors
    TArray<AActor*> overlappingActors;
    m_pPressurePlate->GetOverlappingActors(overlappingActors);

    // Iterate through them adding their masses
    for (const auto* actor : overlappingActors)
    {
        totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }

    return totalMass;
}

