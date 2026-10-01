#!/usr/bin/env python3
"""Dragonbound — M2 phone-side readiness check.

No Unreal Engine is required. This validates the repository contracts that
can be checked before the editor/asset-import session.
"""
from pathlib import Path
import json
import sys

ROOT = Path(__file__).resolve().parents[1]
errors = []

def require(path: str):
    if not (ROOT / path).exists():
        errors.append(f"missing required path: {path}")

def require_text(path: str, needle: str):
    p = ROOT / path
    if not p.exists():
        errors.append(f"missing required path: {path}")
        return
    text = p.read_text(encoding="utf-8", errors="replace")
    if needle not in text:
        errors.append(f"{path}: missing required text: {needle}")

# Repository handoff contracts.
for p in [
    "Dragonbound.uproject",
    ".gitattributes",
    "Docs/M2_PHONE_PREP.md",
    "Docs/TODO.md",
    "Source/Dragonbound/Public/Characters/DBDragonCharacter.h",
    "Source/Dragonbound/Public/Characters/DBDragonVisualComponent.h",
    "Source/Dragonbound/Public/Characters/DBDragonEmotionComponent.h",
    "Source/Dragonbound/Public/Characters/DBBondComponent.h",
    "Source/Dragonbound/Public/Characters/DBMindLinkComponent.h",
    "Source/Dragonbound/Public/Characters/DBDragonInteractionComponent.h",
    "Source/Dragonbound/Public/AI/DBDragonAIController.h",
    "Source/Dragonbound/Public/Characters/DBRiderCharacter.h",
    "Source/Dragonbound/Public/Characters/DBRiderAppearanceDefinition.h",
    "Source/Dragonbound/Private/Tests/DragonboundM2Tests.cpp",
]:
    require(p)

require_text(".gitattributes", "*.glb filter=lfs diff=lfs merge=lfs -text")
require_text("Docs/M2_PHONE_PREP.md", "war_dragon_rigged.glb")
require_text("Docs/M2_PHONE_PREP.md", "adventurer_rigged.glb")
require_text("Docs/M2_PHONE_PREP.md", "57 joints")
require_text("Docs/M2_PHONE_PREP.md", "28-joint")

uproject = ROOT / "Dragonbound.uproject"
if uproject.exists():
    try:
        data = json.loads(uproject.read_text(encoding="utf-8"))
        plugins = {p.get("Name"): p.get("Enabled") for p in data.get("Plugins", [])}
        for plugin in ("Interchange", "InterchangeEditor"):
            if plugins.get(plugin) is not True:
                errors.append(f"Dragonbound.uproject: {plugin} is not enabled")
        if data.get("EngineAssociation") != "5.8":
            errors.append("Dragonbound.uproject: EngineAssociation is not 5.8")
    except json.JSONDecodeError as exc:
        errors.append(f"Dragonbound.uproject: invalid JSON: {exc}")

if errors:
    print("RESULT: FAIL")
    for error in errors:
        print("ERROR:", error)
    sys.exit(1)

print("RESULT: OK")
print("M2 phone-side repository contracts are present.")
print("Unreal asset import and Blueprint validation remain editor-only work.")
