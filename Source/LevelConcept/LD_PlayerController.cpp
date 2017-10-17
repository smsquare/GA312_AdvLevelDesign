#include "LevelConcept.h"
#include "LD_PlayerController.h"

ALD_PlayerController::ALD_PlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	HitResultTraceDistance = 10000.0f;
}

FVector ALD_PlayerController::GetPlayerAimingDirection() {
	// Get the player location
	FVector actorLocation = GetControlledPawn()->GetActorLocation();
	// Get the screen space location of the actor
	FVector2D screenLocation;
	ProjectWorldLocationToScreen(actorLocation, screenLocation);
	// Get the screen space location of the mouse
	float mouseX, mouseY;
	GetMousePosition(mouseX, mouseY);
	//FVector lineEnd, lineDir;
	//DeprojectMousePositionToWorld(lineEnd, lineDir);
	//
	//DrawDebugLine(GetWorld(),
	//	FVector(actorLocation.X, actorLocation.Y, actorLocation.Z),
	//	FVector(lineEnd.X, actorLocation.Y, lineEnd.Z),
	//	FColor::Green, false, 2.0f, 0, 1.0f);
	//
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "TEST: GetPlayerAimingDirection()");
	FVector result;
	result.X = mouseX - screenLocation.X;
	result.Y = 0.0f;
	result.Z = mouseY - screenLocation.Y;
	return result;
}
