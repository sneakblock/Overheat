// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef OVERHEAT_OverheatCharacter_generated_h
#error "OverheatCharacter.generated.h already included, missing '#pragma once' in OverheatCharacter.h"
#endif
#define OVERHEAT_OverheatCharacter_generated_h

#define Overheat_Source_Overheat_OverheatCharacter_h_12_SPARSE_DATA
#define Overheat_Source_Overheat_OverheatCharacter_h_12_RPC_WRAPPERS
#define Overheat_Source_Overheat_OverheatCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define Overheat_Source_Overheat_OverheatCharacter_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAOverheatCharacter(); \
	friend struct Z_Construct_UClass_AOverheatCharacter_Statics; \
public: \
	DECLARE_CLASS(AOverheatCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Overheat"), NO_API) \
	DECLARE_SERIALIZER(AOverheatCharacter)


#define Overheat_Source_Overheat_OverheatCharacter_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAOverheatCharacter(); \
	friend struct Z_Construct_UClass_AOverheatCharacter_Statics; \
public: \
	DECLARE_CLASS(AOverheatCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Overheat"), NO_API) \
	DECLARE_SERIALIZER(AOverheatCharacter)


#define Overheat_Source_Overheat_OverheatCharacter_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AOverheatCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AOverheatCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AOverheatCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AOverheatCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AOverheatCharacter(AOverheatCharacter&&); \
	NO_API AOverheatCharacter(const AOverheatCharacter&); \
public:


#define Overheat_Source_Overheat_OverheatCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AOverheatCharacter(AOverheatCharacter&&); \
	NO_API AOverheatCharacter(const AOverheatCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AOverheatCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AOverheatCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AOverheatCharacter)


#define Overheat_Source_Overheat_OverheatCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(AOverheatCharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__FollowCamera() { return STRUCT_OFFSET(AOverheatCharacter, FollowCamera); } \
	FORCEINLINE static uint32 __PPO__playerHealth() { return STRUCT_OFFSET(AOverheatCharacter, playerHealth); } \
	FORCEINLINE static uint32 __PPO__wasFirstAttackUsed() { return STRUCT_OFFSET(AOverheatCharacter, wasFirstAttackUsed); }


#define Overheat_Source_Overheat_OverheatCharacter_h_9_PROLOG
#define Overheat_Source_Overheat_OverheatCharacter_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Overheat_Source_Overheat_OverheatCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	Overheat_Source_Overheat_OverheatCharacter_h_12_SPARSE_DATA \
	Overheat_Source_Overheat_OverheatCharacter_h_12_RPC_WRAPPERS \
	Overheat_Source_Overheat_OverheatCharacter_h_12_INCLASS \
	Overheat_Source_Overheat_OverheatCharacter_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Overheat_Source_Overheat_OverheatCharacter_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Overheat_Source_Overheat_OverheatCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	Overheat_Source_Overheat_OverheatCharacter_h_12_SPARSE_DATA \
	Overheat_Source_Overheat_OverheatCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	Overheat_Source_Overheat_OverheatCharacter_h_12_INCLASS_NO_PURE_DECLS \
	Overheat_Source_Overheat_OverheatCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> OVERHEAT_API UClass* StaticClass<class AOverheatCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Overheat_Source_Overheat_OverheatCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
