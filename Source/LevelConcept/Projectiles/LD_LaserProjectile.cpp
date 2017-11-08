// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelConcept.h"
#include "BASE_EnemyCharacter.h"
#include "LD_Wall.h"
#include "LD_Player.h"
#include "LD_LaserProjectile.h"

ALD_LaserProjectile::ALD_LaserProjectile() {
	ProjectileLifeSpan = 4.0f;
}

void ALD_LaserProjectile::ProjectileCollisionDetection(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ProjectileImpactCue) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, "ProjectileImpactCue");
		UGameplayStatics::PlaySound2D(OtherActor->GetWorld(), ProjectileImpactCue);
	}
	if (ProjectileOwner == EProjectileOwner::PO_Player) {
		if (OtherActor->ActorHasTag(FName("Enemy"))) {
			ABASE_EnemyCharacter* enemyCharacter = Cast<ABASE_EnemyCharacter>(OtherActor);
			if (enemyCharacter) {
				enemyCharacter->EnemyRecieveDamage(ProjectileDamage);
			}
			Destroy();
			return;
		}
		else if (OtherActor->ActorHasTag(FName("Wall"))) {
			ALD_Wall* wall = Cast<ALD_Wall>(OtherActor);
			if (wall) {
				if (wall->IsSecretWall) {
					wall->SecretWallShot(ProjectileOwner);
				}
				return;
			}
		}
	}
	else if (ProjectileOwner == EProjectileOwner::PO_Enemey) {
		ALD_Player* player = Cast<ALD_Player>(OtherActor);
		if (player) {
			player->DamagePlayer(ProjectileDamage);
			Destroy();
		}
	}
}