#include "Config/VectorLayer.h"

#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "Config/MSPDataLayerDefaultTags.h"
#include "DataPool/DataPoolDataTypes.h"

EDataPoolDataType UVectorLayer::GetDataTypeFromTags() const
{
    if (tags.Contains(FMSPDataLayerDefaultTags::Line))
    {
        return EDataPoolDataType::Lines;
    }
    if (tags.Contains(FMSPDataLayerDefaultTags::Point))
    {
        return EDataPoolDataType::Points;
    }
    if (tags.Contains(FMSPDataLayerDefaultTags::Polygon))
    {
        return EDataPoolDataType::Polygons;
    }

    UE_LOG(LogMspProceduralGen, Error, TEXT("Could not deduce data type for vector layer with name %s"), *name);
    return EDataPoolDataType::Lines;
}
