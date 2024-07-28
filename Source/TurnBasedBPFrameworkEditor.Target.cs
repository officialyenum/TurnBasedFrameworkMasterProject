// Copyright Chukwuyenum Opone @officialyenum

using UnrealBuildTool;
using System.Collections.Generic;

public class TurnBasedBPFrameworkEditorTarget : TargetRules
{
	public TurnBasedBPFrameworkEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "TurnBasedBPFramework" } );
	}
}
