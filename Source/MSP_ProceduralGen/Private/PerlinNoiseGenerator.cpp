// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseGenerator.h"


float UPerlinNoiseGenerator::Perlin2D(float X, float Y, int32 Seed, int32 Octaves) 
{
    float total = 0.0f;    
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f; 
    float persistence = 0.5f; 

    for (int i = 0; i < Octaves; i++)
    {
        Seed = (Seed + i * 263) & ((1 << 16) - 1);

        float seedX = X * frequency + (Seed * 499 + i * 727);
        float seedY = Y * frequency + (Seed * 137 + i * 521);

        float noise = FMath::PerlinNoise2D(FVector2D(seedX, seedY)) * amplitude;

        total += noise;
        maxValue += amplitude;
        frequency *= 2.0f;
        amplitude *= persistence;
    }
    return (total / maxValue  + 1)  * 0.5f;
}

float UPerlinNoiseGenerator::Perlin2DShaped(float X, float Y, int32 Seed, float Scale, float Cover, float Contrast, int32 Octaves) 
{
    X = X / Scale;
    Y = Y / Scale;

    float value = Perlin2D(X, Y, Seed, Octaves);

    Cover = Cover * 2 - 1;

    float CoverA = 1 / (1 - FMath::Clamp(Cover, 0, 0.999));
    float CoverB = 1 - 1 / (1 + FMath::Clamp(Cover, -.999, 0));

    float covered = FMath::Lerp(CoverA, CoverB, value);

    float contrasted = FMath::Clamp((covered - 0.5) * Contrast + 0.5, 0, 1);

    return contrasted;
}

