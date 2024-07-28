// Copyright Chukwuyenum Opone @officialyenum

using UnrealBuildTool;
using System.Collections.Generic;

public class TurnBasedBPFrameworkTarget : TargetRules
{
	public TurnBasedBPFrameworkTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "TurnBasedBPFramework" } );
	}
}
