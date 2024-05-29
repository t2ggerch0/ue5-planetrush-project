#include "Subsystem/DataSubsystem.h"

UDataSubsystem::UDataSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Asset
	{ TEXT("/Script/Engine.DataTable'/Game/PlanetRush"
		"/Blueprint/DT_Projectile.DT_Projectile'") };
	ensure(Asset.Object);

	TArray<FProjectileDataTableRow*> TempArray;
	Asset.Object->GetAllRows<FProjectileDataTableRow>
		(TEXT("GetAllRows"), TempArray);
	for (auto& Row : TempArray)
	{
		if (Row->ProjectileType == EProjectileType::None)
		{
			return;
		}
		TableRowByProjectileType.Add(Row->ProjectileType, Row);
	}
}

const FProjectileDataTableRow* UDataSubsystem::GetProjectile
(const EProjectileType& ProjectileType)
{
	return TableRowByProjectileType[ProjectileType];
}
