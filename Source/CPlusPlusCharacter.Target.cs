// © Coyrights Nilapatri, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class CPlusPlusCharacterTarget : TargetRules
{
	public CPlusPlusCharacterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "CPlusPlusCharacter" } );
	}
}
