
#include "input_player.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

Ainput_player::Ainput_player()
{
	PrimaryActorTick.bCanEverTick = true;

	spring_arm = CreateDefaultSubobject<USpringArmComponent>("spring_arm");
	spring_arm->SetupAttachment(GetMesh(),fp_camera_socket);
	spring_arm->bUsePawnControlRotation = true;
	fp_camera = CreateDefaultSubobject<UCameraComponent>("fp_Camera");
	fp_camera->SetupAttachment(spring_arm);
	fp_camera->bUsePawnControlRotation = true;

	fp_arm_length = 0.0f;
	tp_arm_length = 300.f;
	min_arm_length = 0.f;
	max_arm_length = 300.f;
	zoom_step = 80.f;

	target_arm_length = max_arm_length;
	current_arm_length = max_arm_length;

	current_cam_offset = tp_cam_offset;
	target_cam_offset = tp_cam_offset;

	
	
	walk_speed = FMath::Clamp(600.f, 150.f, 1200.f);
	sprint_speed = FMath::Clamp(1200.f, walk_speed + 1.f, 2400.f);
}

void Ainput_player::BeginPlay()
{
	Super::BeginPlay();
}

void Ainput_player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	current_arm_length = FMath::FInterpTo(current_arm_length, target_arm_length, DeltaTime, zoom_interp_speed);
	spring_arm->TargetArmLength = current_arm_length;

	current_cam_offset = FMath::VInterpTo(current_cam_offset, target_cam_offset, DeltaTime, zoom_interp_speed);
	fp_camera->SetRelativeLocation(current_cam_offset);

}

void Ainput_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(input_mapping, 0);
		}
	}
	if (UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		input->BindAction(move_action, ETriggerEvent::Triggered, this, &Ainput_player::Move);
		input->BindAction(look_action, ETriggerEvent::Triggered, this, &Ainput_player::Look);
		input->BindAction(jump_action, ETriggerEvent::Triggered, this, &Ainput_player::Jump);
		input->BindAction(toggle_cam_action, ETriggerEvent::Triggered, this, &Ainput_player::ToggleCam);
		input->BindAction(sprint_action, ETriggerEvent::Started, this, &Ainput_player::SprintStart);
		input->BindAction(sprint_action, ETriggerEvent::Completed, this, &Ainput_player::SprintStop);
		input->BindAction(zoom_in_action, ETriggerEvent::Completed, this, &Ainput_player::ZoomIn);
		input->BindAction(zoom_out_action, ETriggerEvent::Completed, this, &Ainput_player::ZoomOut);
	}
}

void Ainput_player::Move(const FInputActionValue& input_value)
{
	FVector2D input_vector = input_value.Get<FVector2D>(); 
	if (IsValid(Controller)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, input_vector.Y);
		AddMovementInput(RightDirection, input_vector.X);

	}
}

void Ainput_player::Look(const FInputActionValue& input_value)
{
	FVector2D input_vector = input_value.Get<FVector2D>();
	if (IsValid(Controller)) {

		AddControllerYawInput(input_vector.X);
		AddControllerPitchInput(input_vector.Y);
	}
}

void Ainput_player::ToggleCam()
{
	if (IsValid(Controller)) {
		is_first_person = !is_first_person;
		if (is_first_person) {
			//todo: do this work
			target_arm_length = fp_arm_length;
			target_cam_offset = fp_cam_offset;
			fp_camera->SetRelativeLocation(fp_cam_offset);
			UE_LOG(LogTemp, Warning, TEXT("first person length: %.2f, socket: %s"),target_arm_length, *target_cam_offset.ToString());
		}
		else {
			target_arm_length = tp_arm_length;
			target_cam_offset = tp_cam_offset;
			fp_camera->SetRelativeLocation(tp_cam_offset);
			UE_LOG(LogTemp, Warning, TEXT("third person length: %.2f, socket: %s"),target_arm_length, *target_cam_offset.ToString());
		}
	}
}

void Ainput_player::Jump()
{
	ACharacter::Jump();
}

void Ainput_player::SprintStart(const FInputActionValue& input_value)
{
	GetCharacterMovement()->MaxWalkSpeed = sprint_speed;
}
void Ainput_player::SprintStop(const FInputActionValue& input_value)
{
	GetCharacterMovement()->MaxWalkSpeed = walk_speed;
}

//mousewheel up
void Ainput_player::ZoomIn(const FInputActionValue& input_value)
{
	target_arm_length = FMath::Clamp(target_arm_length - zoom_step, min_arm_length, max_arm_length);

	float alpha = (target_arm_length - fp_arm_length) / (tp_arm_length - fp_arm_length);
	target_cam_offset = FMath::Lerp(fp_cam_offset, tp_cam_offset, alpha);
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("ArmLength | Current: %.2f | Target: %.2f | offset:\n %s"),
		current_arm_length,
		target_arm_length,
		*target_cam_offset.ToString()
	);

	//target_cam_offset.Y < 18.f ? target_cam_offset.Y = 0.f : return;
	if (target_cam_offset.Y >= 36.f) {
		return;
	}
	else {

		target_cam_offset.Y = 0.f;
		target_arm_length = 0.f;
	}



}
void Ainput_player::ZoomOut(const FInputActionValue& input_value)
{
	//inverse add socket offset in for third person
	target_arm_length = FMath::Clamp(target_arm_length + zoom_step, min_arm_length, max_arm_length);

	float alpha = (target_arm_length - fp_arm_length) / (tp_arm_length - fp_arm_length);
	target_cam_offset = FMath::Lerp(fp_cam_offset, tp_cam_offset, alpha);
	if (target_cam_offset.Y < 36.f) {
		target_cam_offset.Y = 36.f;
		target_arm_length = 80.f;
	}
	else {
		return;
	}
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("ArmLength | Current: %.2f | Target: %.2f | offset:\n %s"),
		current_arm_length,
		target_arm_length,
		*target_cam_offset.ToString()
	);

}



