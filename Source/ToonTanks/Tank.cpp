// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm -> SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera -> SetupAttachment(SpringArm);

    
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent -> BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent -> BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent -> BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
	
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController -> GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false,
            HitResult);

        RotateTurret(HitResult.ImpactPoint);

    }

}

void ATank::Move(float Value) 
{
    double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value * DeltaTime * Speed ;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * DeltaTime * TurnRate;
    AddActorLocalRotation(DeltaRotation, true);
}


