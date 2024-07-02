#include "Config/DisplayMethods/DMAreaModelScatter.h"

#include "Config/Config.h"

namespace
{
	bool GetParentVectorLayer(const UDMAreaModelScatter* a_targetDisplayMethod, UVectorLayer*& a_vectorLayer)
	{
		check(false);
		return false;

		/*a_vectorLayer = nullptr;
		const UConfig* config = UConfig::GetInstance();
		if (config == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("%hs: Config has not been loaded."), __FUNCTION__);
			return false;
		}
		a_vectorLayer = *config->datamodel->vector_layers.FindByPredicate([a_targetDisplayMethod](const UVectorLayer* a_vl)
		{
			return a_vl->display_methods.FindByPredicate([a_targetDisplayMethod](const UADisplayMethod* a_dm)
			{
				return a_targetDisplayMethod->GetUniqueID() == a_dm->GetUniqueID();
			});
		});
		return a_vectorLayer != nullptr;*/
	}
}

bool UDMAreaModelScatter::GetParentVectorLayer(UVectorLayer*& a_vectorLayer) const
{
	return ::GetParentVectorLayer(this, a_vectorLayer);
}

bool UDMAreaModelScatter::GetModelByDataIndex(int32 a_dataIndex, FString& a_model) const
{
	return GetFieldValueByDataIndex(a_dataIndex, meta_model, type_model, model, a_model);
}

bool UDMAreaModelScatter::GetWorldSpacingByDataIndex(int32 a_dataIndex, float& a_spacing) const
{
	return GetFieldValueByDataIndex(a_dataIndex, meta_spacing, type_spacing, spacing, a_spacing);
}

bool UDMAreaModelScatter::GetSeaBottomByDataIndex(int32 a_dataIndex, bool& a_seaBottom) const
{
	return GetFieldValueByDataIndex(a_dataIndex, meta_seabottom, type_seabottom, seabottom, a_seaBottom);
}

bool UDMAreaModelScatter::GetMaterialByDataIndex(int32 a_dataIndex, FString& a_material) const
{
	return GetFieldValueByDataIndex(a_dataIndex, meta_material, type_material, material, a_material);
}

bool UDMAreaModelScatter::GetAmountByDataIndex(int32 a_dataIndex, int32& a_amount) const
{
	return GetFieldValueByDataIndex(a_dataIndex, meta_amount, type_amount, amount, a_amount);
}
