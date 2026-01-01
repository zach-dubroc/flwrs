
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "input_player.generated.h"

UCLASS()
class FLWR_BASE_API Ainput_player : public ACharacter
{
	GENERATED_BODY()



protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class Uplayer_camera_component* camera_component;


	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputMappingContext* input_mapping;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* move_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* look_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* jump_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* sprint_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* toggle_cam_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* zoom_in_action;
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* zoom_out_action;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool is_sprinting = false;
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float walk_speed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float sprint_speed;

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
	void SprintStop(const FInputActionValue& input_value);
	void SprintStart(const FInputActionValue& input_value);
	void ZoomIn(const FInputActionValue& input_value);
	void ZoomOut(const FInputActionValue& input_value);
}; 



