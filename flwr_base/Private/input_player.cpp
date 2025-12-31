
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

	min_arm_length = 0.f;
	max_arm_length = 400.f;
	zoom_step = 40.f;
	zoom_interp_speed = 5.f;
	target_arm_length = max_arm_length;
	current_arm_length = max_arm_length;
	
	
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
		//get direction
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
//todo: set spring arm length instead of camera swap
			UE_LOG(LogTemp, Warning, TEXT("first person"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("third person"));
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
}
void Ainput_player::ZoomOut(const FInputActionValue& input_value)
{
	target_arm_length = FMath::Clamp(target_arm_length + zoom_step, min_arm_length, max_arm_length);
}



