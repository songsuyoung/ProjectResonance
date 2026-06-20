#include "Actor/RFarmPlotVolume.h"

void ARFarmPlotVolume::BeginPlay()
{
	Super::BeginPlay();
	
	// 현재 센터를 중심으로, Row/Col Location 구분 후 PlotData 를 생성한다.
	FVector Center = GetActorLocation();

	FVector Origin, BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	FVector LeftUpper = FVector(Center.X - BoxExtent.X, Center.Y + BoxExtent.Y, Center.Z);
	
	// BoxExtent 는 절반임
	float Width  = (BoxExtent.X * 2) / Coordinate.Column;
	float Height = (BoxExtent.Y * 2) / Coordinate.Row;
	
	UWorld* World = GetWorld();
	check(World);
	
	// 최상단 좌측 Location값을 구해서, Row * Column만큼 이동
	for (int Row = 0; Row < Coordinate.Row; Row++)
	{
		for (int Col = 0; Col < Coordinate.Column; Col++)
		{
			FVector Location = FVector(
				LeftUpper.X + Width * (Col + 0.5f), // 칸의 "중앙"에 찍고 싶다면 +0.5
				LeftUpper.Y - Height * (Row + 0.5f),
				LeftUpper.Z
			);
			
			DrawDebugBox(World, Location, FVector(FVector(Width, Height, 10.f)), FColor::Red, false, 10.f, 0, 3.f);
			PlotData.Add({Location, false, false, false});
		}
	}
}
