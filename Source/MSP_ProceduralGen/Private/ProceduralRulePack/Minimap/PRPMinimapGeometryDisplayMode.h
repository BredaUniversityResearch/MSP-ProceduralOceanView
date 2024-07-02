// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralRulePack/PRPMinimapDisplayMode.h"

#include "PRPMinimapGeometryDisplayMode.generated.h"

struct FDataPoolPolygons;
struct FDataPoolPoints;
struct FDataPoolLines;

UENUM(BlueprintType)
enum class EMinimapGeometryMode: uint8
{
	Polygons,
	Lines,
	Points
};

UCLASS(Blueprintable, BlueprintType)
class UPRPMinimapGeometryDisplayMode : public UPRPMinimapDisplayMode
{
	GENERATED_BODY()
public:
	virtual void EnableDisplayMode_Implementation(UDataPool* a_dataPool);
	virtual void DisableDisplayMode_Implementation(UDataPool* a_dataPool);

protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	UTextureRenderTarget2D* DisplayTextureRenderTarget;

private:
	void RenderPolygonData(UCanvas& TargetCanvas, const FDataPoolPolygons& TargetData, const FBox2D& ProjectedWorldArea, const FVector2D& TargetCanvasSize) const;
	void RenderLineData(UCanvas& TargetCanvas, const FDataPoolLines& TargetData, const FBox2D& ProjectedWorldArea, const FVector2D& TargetCanvasSize) const;
	void RenderPointData(UCanvas& TargetCanvas, const FDataPoolPoints& TargetData, const FBox2D& ProjectedWorldArea, const FVector2D& TargetCanvasSize) const;

	UPROPERTY(EditAnywhere)
	int TextureResolution = 256;
	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundColor = FLinearColor(0.02f, 0.05f, 0.1f, 0.7f);
	UPROPERTY(EditAnywhere)
	FLinearColor ForegroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	TArray<FName> PolygonLayers;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* PolygonMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<FName> LineLayers;
	UPROPERTY(EditAnywhere)
	float LineThickness = 2.0f;

	UPROPERTY(EditAnywhere)
	TArray<FName> PointLayers;
};
