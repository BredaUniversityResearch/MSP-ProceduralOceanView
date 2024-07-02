// Fill out your copyright notice in the Description page of Project Settings.

#include "PRPMinimapGeometryDisplayMode.h"

#include "DataPool/DataPool.h"
#include "Engine/Canvas.h"
#include "Generators/SweepGenerator.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

namespace
{
	FVector2D WorldToMinimapCanvasTransform(FVector Position, const FBox2D& ProjectionArea, FVector2D CanvasSize)
	{
		FVector2D result = FVector2D(Position.X, Position.Y) - ProjectionArea.Min;
		result = result / (ProjectionArea.Max - ProjectionArea.Min);
		result = result * CanvasSize;
		return result;
	}
}

void UPRPMinimapGeometryDisplayMode::EnableDisplayMode_Implementation(UDataPool *a_dataPool)
{
	Super::EnableDisplayMode_Implementation(a_dataPool);

	if (DisplayTextureRenderTarget == nullptr)
	{
		DisplayTextureRenderTarget = NewObject<UTextureRenderTarget2D>(this);
		DisplayTextureRenderTarget->ClearColor = BackgroundColor;
		DisplayTextureRenderTarget->InitAutoFormat(TextureResolution, TextureResolution);
	}
	FVector2D targetCanvasSize = FVector2D(DisplayTextureRenderTarget->GetSurfaceWidth(), DisplayTextureRenderTarget->GetSurfaceHeight());

	FBox2D projectionArea = a_dataPool->GetSimulationBounds();

	UCanvas* targetCanvas = nullptr;
	FVector2D size{};
	FDrawToRenderTargetContext context{};
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), DisplayTextureRenderTarget, targetCanvas, size, context);
		
	for (const auto& polygonLayer : PolygonLayers)
	{
		FDataPoolPolygons poly;
		if (a_dataPool->TryGetData(polygonLayer, poly))
		{
			RenderPolygonData(*targetCanvas, poly, projectionArea, targetCanvasSize);
		}
		else
		{
			UE_LOG(LogMspProceduralGen, Warning, TEXT("Display mode %s failed to fetch data source %s as polygons"), *GetName(), *polygonLayer.ToString());
		}
	}

	for (const auto& lineLayer : LineLayers)
	{
		FDataPoolLines lines;
		if (a_dataPool->TryGetData(lineLayer, lines))
		{
			RenderLineData(*targetCanvas, lines, projectionArea, targetCanvasSize);
		}
		else
		{
			UE_LOG(LogMspProceduralGen, Warning, TEXT("Display mode %s failed to fetch data source %s as lines"), *GetName(), *lineLayer.ToString());
		}
	}

	for (const auto& pointLayer : PointLayers)
	{
		FDataPoolPoints points;
		if (a_dataPool->TryGetData(pointLayer, points))
		{
			RenderPointData(*targetCanvas, points, projectionArea, targetCanvasSize);
		}
		else
		{
			UE_LOG(LogMspProceduralGen, Warning, TEXT("Display mode %s failed to fetch data source %s as points"), *GetName(), *pointLayer.ToString());
		}
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);
}

void UPRPMinimapGeometryDisplayMode::DisableDisplayMode_Implementation(UDataPool *a_dataPool)
{
	Super::DisableDisplayMode_Implementation(a_dataPool);
}

void UPRPMinimapGeometryDisplayMode::RenderPolygonData(UCanvas& TargetCanvas, const FDataPoolPolygons& TargetData, const FBox2D& ProjectedWorldArea, const FVector2D& TargetCanvasSize) const
{
	TArray<FCanvasUVTri> triangles;

	for(const FDataPoolPolygon& polygon: TargetData.m_polygons)
	{
		UE::Geometry::FGeneralizedCylinderGenerator ExtrudeGen;
		for (const FVector& Point : polygon.m_points.m_vectorArray)
		{
			ExtrudeGen.CrossSection.AppendVertex(FVector2D(Point.X, Point.Y));
		}
		ExtrudeGen.Path.Add(FVector3d(0.0f, 0.0f, 0.0f));
		ExtrudeGen.Path.Add(FVector3d(0.0f, 0.0f, 1.0f));
		ExtrudeGen.InitialFrame = UE::Geometry::FFrame3d();
		ExtrudeGen.bCapped = true;
		ExtrudeGen.Generate();

		for(const UE::Geometry::FIndex3i triangleIndex : ExtrudeGen.Triangles)
		{
			FCanvasUVTri tri;
			FVector p0 = ExtrudeGen.Vertices[triangleIndex.A];
			tri.V0_Pos = WorldToMinimapCanvasTransform(p0, ProjectedWorldArea, TargetCanvasSize);
			tri.V0_Color = ForegroundColor;

			FVector p1 = ExtrudeGen.Vertices[triangleIndex.B];
			tri.V1_Pos = WorldToMinimapCanvasTransform(p1, ProjectedWorldArea, TargetCanvasSize);
			tri.V1_Color = ForegroundColor;

			FVector p2 = ExtrudeGen.Vertices[triangleIndex.C];
			tri.V2_Pos = WorldToMinimapCanvasTransform(p2, ProjectedWorldArea, TargetCanvasSize);
			tri.V2_Color = ForegroundColor;

			triangles.Add(tri);
		}
	}

	
	TargetCanvas.K2_DrawMaterialTriangle(PolygonMaterial, triangles);
}

void UPRPMinimapGeometryDisplayMode::RenderLineData(UCanvas& TargetCanvas, const FDataPoolLines& TargetData, const FBox2D& ProjectedWorldArea, const FVector2D& TargetCanvasSize) const
{
	for (const auto& line : TargetData.m_lines)
	{
		FVector2D from = WorldToMinimapCanvasTransform(line.m_points.m_vectorArray[0], ProjectedWorldArea, TargetCanvasSize);
		for (int i = 1; i < line.m_points.m_vectorArray.Num(); ++i)
		{
			FVector2D to = WorldToMinimapCanvasTransform(line.m_points.m_vectorArray[i], ProjectedWorldArea, TargetCanvasSize);
			TargetCanvas.K2_DrawLine(from, to, LineThickness, ForegroundColor);
			from = to;
		}
	}
}

void UPRPMinimapGeometryDisplayMode::RenderPointData(UCanvas& TargetCanvas, const FDataPoolPoints& TargetData, const FBox2D& ProjectedWorldArea, const FVector2D& TargetCanvasSize) const
{
	for (const auto& point : TargetData.m_points)
	{
		FVector2D from = WorldToMinimapCanvasTransform(point.m_vector, ProjectedWorldArea, TargetCanvasSize);
		TargetCanvas.K2_DrawBox(from, FVector2D(LineThickness, LineThickness), LineThickness, ForegroundColor);
	}
}
