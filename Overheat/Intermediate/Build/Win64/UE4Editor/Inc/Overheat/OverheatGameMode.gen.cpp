// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Overheat/OverheatGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOverheatGameMode() {}
// Cross Module References
	OVERHEAT_API UClass* Z_Construct_UClass_AOverheatGameMode_NoRegister();
	OVERHEAT_API UClass* Z_Construct_UClass_AOverheatGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Overheat();
// End Cross Module References
	void AOverheatGameMode::StaticRegisterNativesAOverheatGameMode()
	{
	}
	UClass* Z_Construct_UClass_AOverheatGameMode_NoRegister()
	{
		return AOverheatGameMode::StaticClass();
	}
	struct Z_Construct_UClass_AOverheatGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AOverheatGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Overheat,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AOverheatGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "OverheatGameMode.h" },
		{ "ModuleRelativePath", "OverheatGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AOverheatGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AOverheatGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AOverheatGameMode_Statics::ClassParams = {
		&AOverheatGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_AOverheatGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AOverheatGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AOverheatGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AOverheatGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AOverheatGameMode, 1323078547);
	template<> OVERHEAT_API UClass* StaticClass<AOverheatGameMode>()
	{
		return AOverheatGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AOverheatGameMode(Z_Construct_UClass_AOverheatGameMode, &AOverheatGameMode::StaticClass, TEXT("/Script/Overheat"), TEXT("AOverheatGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AOverheatGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
