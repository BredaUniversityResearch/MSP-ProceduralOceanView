// Fill out your copyright notice in the Description page of Project Settings.
#include "MSPProcGenGameInstance.h"

#include "MoviePlayer.h"
#include "GenerationSettings.h"
#include "MSPLoadingScreenWidget.h"
#include "DataPool/DataPool.h"
#include "GeoProjection/GeoCoordinateSystemTransformation.h"

UMSPProcGenGameInstance* UMSPProcGenGameInstance::ms_instance = nullptr;

void UMSPProcGenGameInstance::Init()
{
	Super::Init();

	UGeoCoordinateSystemTransformation::SetupProjContext();

	ms_instance = this;
	m_dataPool = NewObject<UDataPool>(this);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMSPProcGenGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMSPProcGenGameInstance::EndLoadingScreen);
}

void UMSPProcGenGameInstance::Shutdown()
{
	ms_instance = nullptr;

	UGeoCoordinateSystemTransformation::DestroyProjContext();

	Super::Shutdown();
}

UMSPProcGenGameInstance* UMSPProcGenGameInstance::GetInstance()
{
	return ms_instance;
}

UDataPool& UMSPProcGenGameInstance::GetDataPool() const
{
	return *m_dataPool;
}

void UMSPProcGenGameInstance::ResetDataPool()
{
	m_dataPool = NewObject<UDataPool>(this);
}

void UMSPProcGenGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (IsRunningDedicatedServer())
	{
		return;
	}

	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.WidgetLoadingScreen = SNew(SMSPLoadingScreenWidget);

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UMSPProcGenGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
}

UGenerationSettings* UMSPProcGenGameInstance::GetGenerationSettings()
{
	if (!m_generationSettings)
	{
		m_generationSettings = NewObject<UGenerationSettings>();
	}
	return m_generationSettings;
}
