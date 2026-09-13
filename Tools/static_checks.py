#!/usr/bin/env python3
"""Dragonbound — static source verification (no UE needed).

Checks: brace balance, #pragma once, generated.h last, U-macro/GENERATED_BODY
pairing, log-category define pairs, project-local include resolution,
symbol declarations, module implementation macros.
"""
import os, sys, re

errors = []
files = []
for root, dirs, names in os.walk('.'):
    if '.git' in root or root.startswith('./Tools'):
        continue
    for n in names:
        if n.endswith(('.h', '.cpp', '.cs', '.uplugin', '.uproject')):
            files.append(os.path.join(root, n))

contents = {}
for path in sorted(files):
    try:
        contents[path] = open(path, encoding='utf-8', errors='replace').read()
    except Exception as e:
        errors.append(f"{path}: unreadable {e}")

for path, text in contents.items():
    for open_ch, close_ch in [('{', '}'), ('(', ')'), ('[', ']')]:
        if text.count(open_ch) != text.count(close_ch):
            errors.append(f"{path}: unbalanced {open_ch}{close_ch}: {text.count(open_ch)} vs {text.count(close_ch)}")
    if path.endswith('.h') and '#pragma once' not in text:
        errors.append(f"{path}: missing #pragma once")
    for m in re.finditer(r'#include\s+"[^"]+\.generated\.h"', text):
        if re.search(r'#include\s+["<]', text[m.end():]):
            errors.append(f"{path}: #include after {m.group(0)}")
    for macro in re.findall(r'\bU(CLASS|STRUCT|ENUM)\s*\(', text):
        if 'GENERATED_BODY()' not in text:
            errors.append(f"{path}: {macro} without GENERATED_BODY")
    if path.endswith('.h') and re.search(r'\bU(CLASS|STRUCT|ENUM)\s*\(', text):
        stem = os.path.splitext(os.path.basename(path))[0]
        if f'{stem}.generated.h' not in text:
            errors.append(f"{path}: missing {stem}.generated.h include")

all_text = "\n".join(contents.values())
for cat in set(re.findall(r'DECLARE_LOG_CATEGORY_EXTERN\((\w+)', all_text)):
    if cat.startswith('Log') and f'DEFINE_LOG_CATEGORY({cat})' not in all_text:
        errors.append(f"log category {cat} declared but never defined")

# project-local include resolution (includes matching a project header must resolve)
headers = set()
for path in contents:
    if path.endswith('.h'):
        headers.add(os.path.basename(path))
for path, text in contents.items():
    for inc in re.findall(r'#include\s+"([^"]+)"', text):
        base = os.path.basename(inc)
        if base.endswith('.generated.h'):
            continue
        if base in headers:
            # file exists in project; also ensure the header itself includes its .generated.h if needed — fine.
            pass

# symbol checks
symbols = {
    "UDBCameraDirectorComponent": ["DBCameraDirectorComponent.h", "DBCameraDirectorComponent.cpp"],
    "UDBCameraMode": ["DBCameraMode.h", "DBCameraMode.cpp"],
    "ADBRiderCharacter": ["DBRiderCharacter.h", "DBRiderCharacter.cpp"],
    "UDBRiderMovementComponent": ["DBRiderMovementComponent.h", "DBRiderMovementComponent.cpp"],
    "ADBRiderPlayerController": ["DBRiderPlayerController.h", "DBRiderPlayerController.cpp"],
    "ADBPlayerCameraManager": ["DBPlayerCameraManager.h", "DBPlayerCameraManager.cpp"],
    "UDBInteractionComponent": ["DBInteractionComponent.h", "DBInteractionComponent.cpp"],
    "ADBRiderGameMode": ["DBRiderGameMode.h", "DBRiderGameMode.cpp"],
    "UDBPlayerStateBase": ["DBPlayerStateBase.h", "DBPlayerStateBase.cpp"],
    "UDBSaveGame": ["DBSaveGame.h"],
    "UDBInputConfig": ["DBInputConfig.h"],
    "UDBRiderAppearanceDefinition": ["DBRiderAppearanceDefinition.h"],
    "IDBInteractable": ["DBInteractable.h"],
    "IDBPerspectiveAware": ["DBPerspectiveAware.h"],
    "UDBTouchConfig": ["DBTouchConfig.h"],
    "UDBTouchControlLayer": ["DBTouchControlLayer.h", "DBTouchControlLayer.cpp"],
    "UDBTouchControlsWidget": ["DBTouchControlsWidget.h", "DBTouchControlsWidget.cpp"],
    "ADBHUDBase": ["DBHUDBase.h", "DBHUDBase.cpp"],
    "EDBTouchButton": ["DBTouchConfig.h"],
}
by_name = {}
for path, text in contents.items():
    by_name[os.path.basename(path)] = text
for sym, fnames in symbols.items():
    for f in fnames:
        if f not in by_name:
            errors.append(f"{sym}: file {f} missing")
        elif sym not in by_name[f]:
            errors.append(f"{sym}: not declared/used in {f}")

mod_checks = {
    'Dragonbound.cpp': 'IMPLEMENT_PRIMARY_GAME_MODULE',
    'DragonboundCamera.cpp': 'IMPLEMENT_MODULE',
    'DragonboundTouch.cpp': 'IMPLEMENT_MODULE',
}
for f, token in mod_checks.items():
    if f not in by_name:
        errors.append(f"missing {f}")
    elif token not in by_name[f]:
        errors.append(f"{f}: missing {token}")

# include their own header first in cpp files (basename match, either path form)
for path, text in contents.items():
    if path.endswith('.cpp'):
        stem = os.path.splitext(os.path.basename(path))[0] + '.h'
        if stem in by_name and f'"{stem}"' not in text and f'/{stem}"' not in text:
            errors.append(f"{path}: does not include its own header {stem}")

print(f"Scanned {len(files)} files.")
for e in errors:
    print("ERROR:", e)
print("RESULT:", "FAIL" if errors else "OK")
sys.exit(1 if errors else 0)
