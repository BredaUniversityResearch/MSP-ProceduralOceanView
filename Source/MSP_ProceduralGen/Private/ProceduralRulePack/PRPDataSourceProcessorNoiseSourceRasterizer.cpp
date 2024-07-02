#include "PRPDataSourceProcessorNoiseSourceRasterizer.h"

#include "DataPool/DataPool.h"
#include "DataPool/DataPoolDataSourceNames.h"
#include "DataPool/DataPoolRaster.h"
#include "Tasks/Task.h"

const FString UPRPDataSourceProcessorNoiseSourceRasterizer::NoiseR1IntensityMetaDataKey = "NoiseR1Intensity";
const FString UPRPDataSourceProcessorNoiseSourceRasterizer::NoiseR1DistanceMetaDataKey = "NoiseR1Distance";

void UPRPDataSourceProcessorNoiseSourceRasterizer::RegisterWithDataPool_Implementation(UDataPool* DataPool)
{
	Super::RegisterWithDataPool_Implementation(DataPool);

	ensure(DataPool != nullptr);

	m_dataPoolChangedDelegate.BindDynamic(this, &UPRPDataSourceProcessorNoiseSourceRasterizer::OnDataSourceChanged);
	const bool result = DataPool->TryRegisterDataUpdatedListener(DataPoolDataSourceNames::NOISE_SOURCE, m_dataPoolChangedDelegate);
	check(result);
}

void UPRPDataSourceProcessorNoiseSourceRasterizer::OnDataSourceChanged(const FName& DataSourceName)
{
	if (DataSourceName != DataPoolDataSourceNames::NOISE_SOURCE)
	{
		return;
	}

	FDataPoolPoints noiseSourceData;
	bool result = TargetDataPool->TryGetData(DataPoolDataSourceNames::NOISE_SOURCE, noiseSourceData);
	ensure(result);

	IDataPoolRasterReadOnly* noiseRaster;
	result = TargetDataPool->TryGetData(DataPoolDataSourceNames::NOISE, noiseRaster);
	ensure(result);

	UDataPoolRaster* noiseRasterOutput = nullptr;
	FIntPoint targetOutput = noiseRaster->GetRasterSizePixels();
	if (targetOutput.X != OutputLayerResolutionXY || targetOutput.Y != OutputLayerResolutionXY)
	{
		noiseRasterOutput = UDataPoolRaster::CreateWithFormatAndBounds(TargetDataPool, DataPoolDataSourceNames::NOISE, OutputLayerResolutionXY, FRasterPixelChannelFormat::Gray8, noiseRaster->GetCoveredWorldArea());
	}
	else
	{
		noiseRasterOutput = noiseRaster->MakeWritableCopy();
	}

	UE::Tasks::Launch(UE_SOURCE_LOCATION, [noiseSourceData, outputRaster = TStrongObjectPtr(noiseRasterOutput), DataPool = TargetDataPool]() mutable
	{
		RasterizeNoise(noiseSourceData, outputRaster.Get());
		DataPool->TrySetData(DataPoolDataSourceNames::NOISE, MoveTemp(outputRaster)); 
	});
}

void UPRPDataSourceProcessorNoiseSourceRasterizer::RasterizeNoise(const FDataPoolPoints& DataPoolPoints, UDataPoolRaster* DataPoolRaster)
{
	DataPoolRaster->SetAllRasterValuesToZero();

	for (const FDataPoolPoint& point : DataPoolPoints.m_points)
	{
		double noiseR1Intensity = 0.0;
		double noiseR1Distance = 0.0;
		if (const FString* intensityMetaData = point.m_metadata.Find(NoiseR1IntensityMetaDataKey))
		{
			noiseR1Intensity = FCString::Atod(*(*intensityMetaData));
		}
		if (const FString* falloutDistanceMetaData = point.m_metadata.Find(NoiseR1DistanceMetaDataKey))
		{
			noiseR1Distance = FCString::Atod(*(*falloutDistanceMetaData));
		}

		if (noiseR1Intensity > NoiseFloorOcean && noiseR1Distance > 0)
		{
			//I1 - 20 * log_10(x / R1)
			double distancesToNoiseFloor = FMath::Pow(2, (noiseR1Intensity - NoiseFloorOcean) / 6.0); //6dB reduction every doubling of distance.
			double falloffDistance = distancesToNoiseFloor * noiseR1Distance;
			DataPoolRaster->PlotValueGaussianDistribution(FVector2D(point.m_vector.X, point.m_vector.Y), falloffDistance, noiseR1Intensity);
		}
	}
}
