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
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Poll the trigger volume
    // If Player steps in the volume, open the door.
    static bool bOpenDoor = false; 
    if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO make weight into parameter
    {
        OnOpenRequest.Broadcast();
    }
    else
    {
        OnCloseRequest.Broadcast();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float totalMass = 0.f;

    if (PressurePlate)
    {
        // Find all overlapping actors
        TArray<AActor*> overlappingActors;
        PressurePlate->GetOverlappingActors(overlappingActors);

        // Iterate through them adding their masses
        for (const auto* actor : overlappingActors)
        {
            totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        }
    }

    return totalMass;
}

