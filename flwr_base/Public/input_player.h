
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "input_player.generated.h"

UCLASS()
class FLWR_BASE_API Ainput_player : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* fp_Camera;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	bool is_first_person = false;

protected:
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputMappingContext* input_mapping;

	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* move_action;

	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* look_action;

	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* jump_action;

	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* toggle_cam_action;
		

public:
	Ainput_player();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	void Move(const FInputActionValue& input_value);
	void Look(const FInputActionValue& input_value);
	void ToggleCam();
	void Jump();
};
