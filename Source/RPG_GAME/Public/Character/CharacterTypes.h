#pragma once
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquipOneHand UMETA(DisplayName = "EquipOneHand"),
	ECS_EquipTwoHand UMETA(DisplayName = "EquipTwoHand")
};

UENUM(BlueprintType)
enum class ECharacterAnimationState : uint8
{
	EAS_None UMETA(DisplayName = "None"),
	EAS_Attacking UMETA(DisplayName = "Attack")

};