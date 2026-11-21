using UnrealBuildTool;
using System.Collections.Generic;

public class PriceOnYourHeadTarget : TargetRules
{
	public PriceOnYourHeadTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("PriceOnYourHead");
	}
}
