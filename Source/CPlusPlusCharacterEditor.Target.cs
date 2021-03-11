// © Coyrights Nilapatri, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class CPlusPlusCharacterEditorTarget : TargetRules
{
	public CPlusPlusCharacterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "CPlusPlusCharacter" } );
	}
}
