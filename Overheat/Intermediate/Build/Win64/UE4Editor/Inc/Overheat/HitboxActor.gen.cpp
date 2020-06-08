// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Overheat/HitboxActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHitboxActor() {}
// Cross Module References
	OVERHEAT_API UEnum* Z_Construct_UEnum_Overheat_EHitboxEnum();
	UPackage* Z_Construct_UPackage__Script_Overheat();
	OVERHEAT_API UClass* Z_Construct_UClass_AHitboxActor_NoRegister();
	OVERHEAT_API UClass* Z_Construct_UClass_AHitboxActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	OVERHEAT_API UFunction* Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
// End Cross Module References
	static UEnum* EHitboxEnum_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_Overheat_EHitboxEnum, Z_Construct_UPackage__Script_Overheat(), TEXT("EHitboxEnum"));
		}
		return Singleton;
	}
	template<> OVERHEAT_API UEnum* StaticEnum<EHitboxEnum>()
	{
		return EHitboxEnum_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EHitboxEnum(EHitboxEnum_StaticEnum, TEXT("/Script/Overheat"), TEXT("EHitboxEnum"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_Overheat_EHitboxEnum_Hash() { return 3223369383U; }
	UEnum* Z_Construct_UEnum_Overheat_EHitboxEnum()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_Overheat();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EHitboxEnum"), 0, Get_Z_Construct_UEnum_Overheat_EHitboxEnum_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EHitboxEnum::HB_PROXIMITY", (int64)EHitboxEnum::HB_PROXIMITY },
				{ "EHitboxEnum::HB_STRIKE", (int64)EHitboxEnum::HB_STRIKE },
				{ "EHitboxEnum::HB_HURTBOX", (int64)EHitboxEnum::HB_HURTBOX },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "HB_HURTBOX.DisplayName", "Hurtbox" },
				{ "HB_HURTBOX.Name", "EHitboxEnum::HB_HURTBOX" },
				{ "HB_PROXIMITY.DisplayName", "Proximity" },
				{ "HB_PROXIMITY.Name", "EHitboxEnum::HB_PROXIMITY" },
				{ "HB_STRIKE.DisplayName", "Strike" },
				{ "HB_STRIKE.Name", "EHitboxEnum::HB_STRIKE" },
				{ "ModuleRelativePath", "HitboxActor.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_Overheat,
				nullptr,
				"EHitboxEnum",
				"EHitboxEnum",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void AHitboxActor::StaticRegisterNativesAHitboxActor()
	{
		UClass* Class = AHitboxActor::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "TriggerVisualizeHitbox", &AHitboxActor::execTriggerVisualizeHitbox },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox_Statics::Function_MetaDataParams[] = {
		{ "Comment", "//Notify the HitboxActorBP class instance that the hitbox is ready to be drawn.\n" },
		{ "ModuleRelativePath", "HitboxActor.h" },
		{ "ToolTip", "Notify the HitboxActorBP class instance that the hitbox is ready to be drawn." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AHitboxActor, nullptr, "TriggerVisualizeHitbox", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_AHitboxActor_NoRegister()
	{
		return AHitboxActor::StaticClass();
	}
	struct Z_Construct_UClass_AHitboxActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_hitboxDamage_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_hitboxDamage;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_hitboxLocation_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_hitboxLocation;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_hitboxType_MetaData[];
#endif
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_hitboxType;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_hitboxType_Underlying;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AHitboxActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Overheat,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_AHitboxActor_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AHitboxActor_TriggerVisualizeHitbox, "TriggerVisualizeHitbox" }, // 884739254
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHitboxActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "HitboxActor.h" },
		{ "ModuleRelativePath", "HitboxActor.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxDamage_MetaData[] = {
		{ "Category", "Hitbox" },
		{ "Comment", "//The damage a hitbox will do\n" },
		{ "ModuleRelativePath", "HitboxActor.h" },
		{ "ToolTip", "The damage a hitbox will do" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxDamage = { "hitboxDamage", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AHitboxActor, hitboxDamage), METADATA_PARAMS(Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxDamage_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxDamage_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxLocation_MetaData[] = {
		{ "Category", "Hitbox" },
		{ "Comment", "//The locationto spawn the hitbox.\n" },
		{ "ModuleRelativePath", "HitboxActor.h" },
		{ "ToolTip", "The locationto spawn the hitbox." },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxLocation = { "hitboxLocation", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AHitboxActor, hitboxLocation), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxLocation_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxLocation_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType_MetaData[] = {
		{ "Category", "Hitbox" },
		{ "Comment", "//The hitbox enum instance.\n" },
		{ "ModuleRelativePath", "HitboxActor.h" },
		{ "ToolTip", "The hitbox enum instance." },
	};
#endif
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType = { "hitboxType", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AHitboxActor, hitboxType), Z_Construct_UEnum_Overheat_EHitboxEnum, METADATA_PARAMS(Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType_MetaData)) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AHitboxActor_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxDamage,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxLocation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AHitboxActor_Statics::NewProp_hitboxType_Underlying,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AHitboxActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AHitboxActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AHitboxActor_Statics::ClassParams = {
		&AHitboxActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AHitboxActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AHitboxActor_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AHitboxActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AHitboxActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AHitboxActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AHitboxActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AHitboxActor, 976396478);
	template<> OVERHEAT_API UClass* StaticClass<AHitboxActor>()
	{
		return AHitboxActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AHitboxActor(Z_Construct_UClass_AHitboxActor, &AHitboxActor::StaticClass, TEXT("/Script/Overheat"), TEXT("AHitboxActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AHitboxActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
