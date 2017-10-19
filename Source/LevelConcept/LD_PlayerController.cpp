#include "LevelConcept.h"
#include "LD_PlayerController.h"

ALD_PlayerController::ALD_PlayerController() {
	playerGunLocation = FVector(0.0f, 0.0f, 75.0f);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	HitResultTraceDistance = 10000.0f;
}

FVector ALD_PlayerController::GetPlayerAimingDirection() {
	// Get the player location
	FVector actorLocation = GetPawn()->GetActorLocation();
	actorLocation += playerGunLocation;
	//actorLocation += actorLocation.ForwardVector * (25);
	// Get the screen space location of the actor
	FVector2D screenLocation;
	ProjectWorldLocationToScreen(actorLocation, screenLocation);
	// Get the screen space location of the mouse
	float mouseX, mouseY;
	GetMousePosition(mouseX, mouseY);
	FVector result;
	result.X = mouseX - screenLocation.X;
	result.Y = 0.0f;
	result.Z = screenLocation.Y - mouseY;

	return result;
}
