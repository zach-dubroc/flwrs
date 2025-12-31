
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

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	//fp_CameraPivot = CreateDefaultSubobject<USceneComponent>("fp_CameraPivot");
	//fp_CameraPivot->SetupAttachment(GetMesh(), fp_CameraSocket);

	fp_Camera = CreateDefaultSubobject<UCameraComponent>("fp_Camera");
	fp_Camera->SetupAttachment(GetMesh(),fp_CameraSocket);
	fp_Camera->bUsePawnControlRotation = true;

	walk_speed = FMath::Clamp(walk_speed, 150.f, 1200.f);
	sprint_speed = FMath::Clamp(sprint_speed, walk_speed + 1.f, 2400.f);
}
void Ainput_player::BeginPlay()
{
	Super::BeginPlay();
}
void Ainput_player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		//if clipping can use GetMesh->SetOwnerNoSee
		//better fix maybe is attaching fp_camera to head socket and using eye offset
		is_first_person = !is_first_person;
		if (is_first_person) {
			fp_Camera->SetActive(true);
			Camera->SetActive(false);
			//GetMesh()->SetOwnerNoSee(true);

		}
		else {
			fp_Camera->SetActive(false);
			Camera->SetActive(true);
			//GetMesh()->SetOwnerNoSee(false);
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



