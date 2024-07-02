#include "SeaBedMeshActor.h"

#include "MSPProcGenGameInstance.h"
#include "Config/Config.h"
#include "DataPool/DataPool.h"
#include "DataPool/DataPoolDataSourceNames.h"
#include "DataPool/DataPoolRaster.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

namespace
{
	void GenerateTangentsAndNormals(int32 a_width, int32 a_height, const TArray<FVector>& a_outHeightPixels, TArray<FVector>& a_outNormals, TArray<FProcMeshTangent>& a_outTangents)
	{
		for (int32 i = 0; i < a_height; i++)
		{
			for (int32 j = 0; j < a_width; j++)
			{
				FVector tangent_x = FVector::ZeroVector;
				FVector tangent_y = FVector::ZeroVector;

				// Check for left edge
				if (j > 0) {
					tangent_x += a_outHeightPixels[i * a_width + j] - a_outHeightPixels[i * a_width + j - 1];
				}
				// Check for right edge
				if (j < a_width - 1) {
					tangent_x += a_outHeightPixels[i * a_width + j + 1] - a_outHeightPixels[i * a_width + j];
				}
				// Check for top edge
				if (i > 0) {
					tangent_y += a_outHeightPixels[i * a_width + j] - a_outHeightPixels[(i - 1) * a_width + j];
				}
				// Check for bottom edge
				if (i < a_height - 1) {
					tangent_y += a_outHeightPixels[(i + 1) * a_width + j] - a_outHeightPixels[i * a_width + j];
				}

				// Calculate the normal
				FVector normal = FVector::CrossProduct(tangent_x, tangent_y);
				normal.Normalize();

				// Add the normal to the array
				a_outNormals.Add(normal);
				FProcMeshTangent Tangent;
				Tangent.TangentX = tangent_y;
				a_outTangents.Add(Tangent);
			}
		}
	}

	// This smooths out mesh points to remove jaggies.
	void GaussianBlurVectors(TArray<FVector>& a_vectors, const int32& a_width, const int32& a_height, const int32 a_blurRadius)
	{
		TArray<FVector> blurredPixels = a_vectors;
		for (int32 y = 0; y < a_height; y++)
		{
			for (int32 x = 0; x < a_width; x++)
			{
				FVector sum = FVector::ZeroVector;
				float weightSum = 0.0f;

				for (int32 boxY = -a_blurRadius; boxY <= a_blurRadius; boxY++)
				{
					for (int32 boxX = -a_blurRadius; boxX <= a_blurRadius; boxX++)
					{
						const int32 targetX = x + boxX;
						const int32 targetY = y + boxY;

						if (targetX >= 0 && targetX < a_width && targetY >= 0 && targetY < a_height)
						{
							float distance = FMath::Sqrt(static_cast<float>(boxX * boxX + boxY * boxY));
							float weight = FMath::Exp(-0.5f * distance * distance / (a_blurRadius * a_blurRadius)) / (2.0f * PI * a_blurRadius * a_blurRadius);
							sum += a_vectors[targetY * a_width + targetX] * weight;
							weightSum += weight;
						}
					}
				}

				// Avoid divide by zero if the weight sum is zero
				if (weightSum > 0.0f)
				{
					blurredPixels[y * a_width + x] = sum / weightSum;
				}
				else
				{
					blurredPixels[y * a_width + x] = FVector::ZeroVector;;
				}
			}
		}

		a_vectors = blurredPixels;
	}

	void GenerateTriangleIndicesArray(TArray<int32>& a_outTriangleIndices, int32 a_width, int32 a_height)
	{
		for (int32 y = 0; y < a_height - 1; ++y)
		{
			for (int32 x = 0; x < a_width - 1; ++x)
			{
				int32 topLeft = y * a_width + x;
				int32 topRight = y * a_width + x + 1;
				int32 bottomLeft = (y + 1) * a_width + x;
				int32 bottomRight = (y + 1) * a_width + x + 1;

				// Triangle 1 (top-left, top-right, bottom-left)
				a_outTriangleIndices.Add(topLeft);
				a_outTriangleIndices.Add(bottomLeft);
				a_outTriangleIndices.Add(topRight);

				// Triangle 2 (top-right, bottom-right, bottom-left)
				a_outTriangleIndices.Add(topRight);
				a_outTriangleIndices.Add(bottomLeft);
				a_outTriangleIndices.Add(bottomRight);
			}
		}
	}
}

ASeaBedMeshActor::ASeaBedMeshActor(const FObjectInitializer& ObjectInitializer)
{
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(FName("MeshComponent"));
}

void ASeaBedMeshActor::BeginPlay()
{
	Super::BeginPlay();

	UDataPool& dataPool = UMSPProcGenGameInstance::GetInstance()->GetDataPool();

	m_bathymetryDataUpdatedDelegate.BindDynamic(this, &ASeaBedMeshActor::OnBathymetryDataChanged);
	dataPool.TryRegisterDataUpdatedListener(DataPoolDataSourceNames::BATHYMETRY, m_bathymetryDataUpdatedDelegate);

	OnBathymetryDataChanged(DataPoolDataSourceNames::BATHYMETRY);
}

void ASeaBedMeshActor::OnBathymetryDataChanged(const FName& UpdatedDataSourceName)
{
	FVector2D worldBoundsMin = UConfig::GetInstance()->datamodel->GetCoordinate0AsWorldVector2D();
	FVector2D worldBoundsMax = UConfig::GetInstance()->datamodel->GetCoordinate1AsWorldVector2D();
	GenerateProceduralMeshData(FIntVector2(VertexResolutionXY, VertexResolutionXY), MeshSmoothing, FBox2D(worldBoundsMin, worldBoundsMax), MeshComponent);
}

bool ASeaBedMeshActor::GenerateProceduralMeshData(FIntVector2 VertexResolution, const int32 MeshSmoothingAmount, FBox2D TargetWorldArea, UProceduralMeshComponent* Target)
{
	UMSPProcGenGameInstance* mspGameMode = UMSPProcGenGameInstance::GetInstance();
	UDataPool& dataPool = mspGameMode->GetDataPool();
	IDataPoolRasterReadOnly* bathymetryData = nullptr;
	if (!dataPool.TryGetData(DataPoolDataSourceNames::BATHYMETRY, bathymetryData))
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Failed to find bathymetry raster data source, cannot generate procedural mesh"));
		return false;
	}

	TArray<FProcMeshTangent> outTangents;
	TArray<FVector2D>        outUVs;
	TArray<int32>            outTriangleIndices;
	TArray<FVector>          outHeightPixels;
	TArray<FVector>          outNormals;
	/*int32 width = m_rasterWidth;
	int32 height = m_rasterHeight;
	const TArray<uint8>& byteArray = m_rasterByteArray;*/

	int32 numVertices = VertexResolution.X * VertexResolution.Y;

	//~~~~~~~~~~~~~ Generate Linear Color Array and reserve memory for other arrays ~~~~~~~~~~~~~~~~~~~~~~
	outHeightPixels.Reserve(numVertices);
	outUVs.Reserve(numVertices);
	outNormals.Reserve(numVertices);
	outTangents.Reserve(numVertices);

	const FVector2D dataModelAreaDistance = TargetWorldArea.Max - TargetWorldArea.Min;
	for (int y = 0; y < VertexResolution.Y; ++y)
	{
		for (int x = 0; x < VertexResolution.X; ++x)
		{
			FVector2D vertexPosPercentage = FVector2D(static_cast<double>(x) / static_cast<double>(VertexResolution.X - 1),
				static_cast<double>(y) / static_cast<double>(VertexResolution.Y - 1));

			double vertexX = TargetWorldArea.Min.X + (vertexPosPercentage.X * dataModelAreaDistance.X);
			double vertexY = TargetWorldArea.Min.Y + (vertexPosPercentage.Y * dataModelAreaDistance.Y);
			double vertexZ = bathymetryData->GetMappedValueAtWorldPosition(FVector2D(vertexX, vertexY));
			//POV-???: Land should be at 5m. Bathymetry is clipped at 0.
			if (vertexZ > -1.0)
			{
				vertexZ = 500.0; //Unreal is in cm
			}
			outHeightPixels.Add(FVector(vertexX, vertexY, vertexZ));

			outUVs.Add(vertexPosPercentage);
		}
	}

	// smooth out the mesh by an input amount to get rid of jaggies

	//GaussianBlurVectors(outHeightPixels, VertexResolution.X, VertexResolution.Y, MeshSmoothingAmount);

	//----- GENERATE TRIANGLE INDICES ARRAY -----

	GenerateTriangleIndicesArray(outTriangleIndices, VertexResolution.X, VertexResolution.Y);

	//--------GENERATE NORMALS AND TANGENTS ARRAY----------

	GenerateTangentsAndNormals(VertexResolution.X, VertexResolution.Y, outHeightPixels, outNormals, outTangents);

	//-----------GENERATE VCOLORS AND FILL WITH WHITE ------

	TArray<FLinearColor> outVcols;
	outVcols.Init(FLinearColor(1, 1, 1, 1), outHeightPixels.Num());

	Target->ClearMeshSection(0);
	Target->CreateMeshSection_LinearColor(0, outHeightPixels, outTriangleIndices, outNormals, outUVs, outVcols, outTangents, true);
	Target->AddCollisionConvexMesh(outHeightPixels);
	// add the collision to the mesh
	Target->SetCollisionProfileName("BlockAllStatic");
	Target->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Target->SetCollisionObjectType(ECC_GameTraceChannel2);

	Target->SetLightingChannels(true, true, false);

	Target->SetMaterial(0, MeshMaterial);

	// Success!
	return true;
}

