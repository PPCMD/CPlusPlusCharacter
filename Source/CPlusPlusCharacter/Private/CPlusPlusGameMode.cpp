// © Copyrights Nilapatri, All Rights Reserved

#include "CPlusPlusGameMode.h"
#include "CPlusPlusCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPlusPlusGameMode::ACPlusPlusGameMode()
{
	// Set the default pawn to the Blueprint class
	/* static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Blueprtints/CharacterBlueprint"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	} */
}
