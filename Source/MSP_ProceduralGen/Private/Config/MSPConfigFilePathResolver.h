#pragma once

class FMSPConfigFilePathResolver
{
public:
	FMSPConfigFilePathResolver(const FString& a_basePath);

	FString ResolvePath(const FString& InPath) const;

private:
	FString BasePath;
};