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

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"), //순찰
	EES_Chasing UMETA(DisplayName = "Chasing"), //쫓다
	EES_Attacking UMETA(DisplayName = "Attacking") //공격
};