// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef OVERHEAT_HitboxActor_generated_h
#error "HitboxActor.generated.h already included, missing '#pragma once' in HitboxActor.h"
#endif
#define OVERHEAT_HitboxActor_generated_h

#define Overheat_Source_Overheat_HitboxActor_h_19_SPARSE_DATA
#define Overheat_Source_Overheat_HitboxActor_h_19_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execTriggerVisualizeHitbox) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->TriggerVisualizeHitbox(); \
		P_NATIVE_END; \
	}


#define Overheat_Source_Overheat_HitboxActor_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execTriggerVisualizeHitbox) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->TriggerVisualizeHitbox(); \
		P_NATIVE_END; \
	}


#define Overheat_Source_Overheat_HitboxActor_h_19_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAHitboxActor(); \
	friend struct Z_Construct_UClass_AHitboxActor_Statics; \
public: \
	DECLARE_CLASS(AHitboxActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Overheat"), NO_API) \
	DECLARE_SERIALIZER(AHitboxActor)


#define Overheat_Source_Overheat_HitboxActor_h_19_INCLASS \
private: \
	static void StaticRegisterNativesAHitboxActor(); \
	friend struct Z_Construct_UClass_AHitboxActor_Statics; \
public: \
	DECLARE_CLASS(AHitboxActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Overheat"), NO_API) \
	DECLARE_SERIALIZER(AHitboxActor)


#define Overheat_Source_Overheat_HitboxActor_h_19_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AHitboxActor(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AHitboxActor) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AHitboxActor); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AHitboxActor); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AHitboxActor(AHitboxActor&&); \
	NO_API AHitboxActor(const AHitboxActor&); \
public:


#define Overheat_Source_Overheat_HitboxActor_h_19_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AHitboxActor(AHitboxActor&&); \
	NO_API AHitboxActor(const AHitboxActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AHitboxActor); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AHitboxActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AHitboxActor)


#define Overheat_Source_Overheat_HitboxActor_h_19_PRIVATE_PROPERTY_OFFSET
#define Overheat_Source_Overheat_HitboxActor_h_16_PROLOG
#define Overheat_Source_Overheat_HitboxActor_h_19_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Overheat_Source_Overheat_HitboxActor_h_19_PRIVATE_PROPERTY_OFFSET \
	Overheat_Source_Overheat_HitboxActor_h_19_SPARSE_DATA \
	Overheat_Source_Overheat_HitboxActor_h_19_RPC_WRAPPERS \
	Overheat_Source_Overheat_HitboxActor_h_19_INCLASS \
	Overheat_Source_Overheat_HitboxActor_h_19_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Overheat_Source_Overheat_HitboxActor_h_19_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Overheat_Source_Overheat_HitboxActor_h_19_PRIVATE_PROPERTY_OFFSET \
	Overheat_Source_Overheat_HitboxActor_h_19_SPARSE_DATA \
	Overheat_Source_Overheat_HitboxActor_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	Overheat_Source_Overheat_HitboxActor_h_19_INCLASS_NO_PURE_DECLS \
	Overheat_Source_Overheat_HitboxActor_h_19_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> OVERHEAT_API UClass* StaticClass<class AHitboxActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Overheat_Source_Overheat_HitboxActor_h


#define FOREACH_ENUM_EHITBOXENUM(op) \
	op(EHitboxEnum::HB_PROXIMITY) \
	op(EHitboxEnum::HB_STRIKE) \
	op(EHitboxEnum::HB_HURTBOX) 

enum class EHitboxEnum : uint8;
template<> OVERHEAT_API UEnum* StaticEnum<EHitboxEnum>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
