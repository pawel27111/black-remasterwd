# BLACK: REMASTERED - ASSET SPECIFICATIONS

## Complete List of Required Assets

---

## TABLE OF CONTENTS
1. [3D MODELS](#1-3d-models)
2. [TEXTURES](#2-textures)
3. [ANIMATIONS](#3-animations)
4. [AUDIO ASSETS](#4-audio-assets)
5. [PARTICLE EFFECTS](#5-particle-effects)
6. [MATERIALS](#6-materials)
7. [UI ASSETS](#7-ui-assets)
8. [LEVEL ASSETS](#8-level-assets)

---

## 1. 3D MODELS

### 1.1 Character Models

#### 1.1.1 Player Character (Jack Kellar)
- **File:** `CH_JackKellar.fbx`
- **Type:** Skeletal Mesh
- **Polycount:** 15,000-25,000 triangles
- **LODs:** 4 (High, Medium, Low, Lowest)
- **Rig:** UE5 Mannequin compatible
- **Features:**
  - High-detail facial features
  - Realistic proportions
  - Military gear (vest, pouches, etc.)
  - Customizable appearance

**Required Bones:**
- Full body rig (80+ bones)
- Facial bones for animations
- Finger bones for weapon handling
- IK rig for feet and hands

**Materials:**
- Skin (PBR)
- Clothing (fabric PBR)
- Gear (metal/plastic PBR)
- Boots (leather/rubber PBR)

---

#### 1.1.2 Enemy Characters

**Grunt (Basic Infantry)**
- **File:** `CH_Enemy_Grunt.fbx`
- **Polycount:** 8,000-12,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Outfits:** 3 variants (different uniforms)

**Soldier (Standard Infantry)**
- **File:** `CH_Enemy_Soldier.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Outfits:** 4 variants

**Veteran (Elite Infantry)**
- **File:** `CH_Enemy_Veteran.fbx`
- **Polycount:** 12,000-18,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** More detailed gear, heavy armor

**Sniper**
- **File:** `CH_Enemy_Sniper.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Ghillie suit variant, heavy weapon

**Shotgunner**
- **File:** `CH_Enemy_Shotgunner.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Heavy build, shotgun on back

**Machine Gunner**
- **File:** `CH_Enemy_MachineGunner.fbx`
- **Polycount:** 12,000-18,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Heavy weapon, ammo belts

**Grenadier**
- **File:** `CH_Enemy_Grenadier.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Grenade pouches, explosive gear

**Engineer**
- **File:** `CH_Enemy_Engineer.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Tool belt, repair equipment

**Heavy**
- **File:** `CH_Enemy_Heavy.fbx`
- **Polycount:** 15,000-20,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Minigun, heavy armor

**RPG Trooper**
- **File:** `CH_Enemy_RPG.fbx`
- **Polycount:** 12,000-18,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** RPG on back, rocket tubes

**Shield Bearer**
- **File:** `CH_Enemy_Shield.fbx`
- **Polycount:** 12,000-18,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Ballistic shield, sidearm

**Commander**
- **File:** `CH_Enemy_Commander.fbx`
- **Polycount:** 15,000-20,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Officer uniform, radio equipment

**Spetsnaz**
- **File:** `CH_Enemy_Spetsnaz.fbx`
- **Polycount:** 12,000-18,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Special forces gear, balaclava

**Black Ops**
- **File:** `CH_Enemy_BlackOps.fbx`
- **Polycount:** 15,000-20,000 triangles
- **LODs:** 3
- **Rig:** UE5 Mannequin compatible
- **Features:** Stealth suit, night vision

**Boss (Colonel Orlov)**
- **File:** `CH_Boss_Orlov.fbx`
- **Polycount:** 25,000-30,000 triangles
- **LODs:** 4
- **Rig:** Custom rig with facial bones
- **Features:** High-detail uniform, unique gear

---

#### 1.1.3 Civilian Models

**Civilian Male 1**
- **File:** `CH_Civilian_Male1.fbx`
- **Polycount:** 5,000-8,000 triangles
- **LODs:** 2
- **Rig:** Simple rig (no facial bones needed)

**Civilian Male 2**
- **File:** `CH_Civilian_Male2.fbx`
- **Polycount:** 5,000-8,000 triangles
- **LODs:** 2

**Civilian Female 1**
- **File:** `CH_Civilian_Female1.fbx`
- **Polycount:** 5,000-8,000 triangles
- **LODs:** 2

**Civilian Female 2**
- **File:** `CH_Civilian_Female2.fbx`
- **Polycount:** 5,000-8,000 triangles
- **LODs:** 2

**Civilian Child**
- **File:** `CH_Civilian_Child.fbx`
- **Polycount:** 3,000-5,000 triangles
- **LODs:** 2

---

### 1.2 Weapon Models

#### 1.2.1 Pistols

**M9 Beretta**
- **File:** `WP_M9.fbx`
- **Polycount:** 2,000-3,000 triangles
- **LODs:** 2
- **Features:** Slide, magazine, detailed textures

**Glock 17**
- **File:** `WP_Glock17.fbx`
- **Polycount:** 2,000-3,000 triangles
- **LODs:** 2

**Desert Eagle**
- **File:** `WP_DesertEagle.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2
- **Features:** Large caliber, detailed engravings

**Makarov PM**
- **File:** `WP_Makarov.fbx`
- **Polycount:** 1,500-2,500 triangles
- **LODs:** 2

**SIG Sauer P226**
- **File:** `WP_P226.fbx`
- **Polycount:** 2,000-3,000 triangles
- **LODs:** 2

---

#### 1.2.2 Shotguns

**Mossberg 500**
- **File:** `WP_Mossberg500.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2
- **Features:** Pump action, wooden stock

**Remington 870**
- **File:** `WP_Remington870.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2

**Saiga-12**
- **File:** `WP_Saiga12.fbx`
- **Polycount:** 3,500-4,500 triangles
- **LODs:** 2
- **Features:** Magazine-fed, semi-auto

**SPAS-12**
- **File:** `WP_SPAS12.fbx`
- **Polycount:** 4,000-5,000 triangles
- **LODs:** 2
- **Features:** Folding stock, dual-mode

**Double Barrel**
- **File:** `WP_DoubleBarrel.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2
- **Features:** Break-action, side-by-side barrels

---

#### 1.2.3 Submachine Guns

**MP5**
- **File:** `WP_MP5.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2

**UMP**
- **File:** `WP_UMP.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2

**PP-19 Bizon**
- **File:** `WP_Bizon.fbx`
- **Polycount:** 3,500-4,500 triangles
- **LODs:** 2
- **Features:** Helical magazine

**Kriss Vector**
- **File:** `WP_Vector.fbx`
- **Polycount:** 3,500-4,500 triangles
- **LODs:** 2
- **Features:** Unique delayed blowback

---

#### 1.2.4 Assault Rifles

**M4 Carbine**
- **File:** `WP_M4.fbx`
- **Polycount:** 4,000-5,000 triangles
- **LODs:** 2
- **Features:** Modular design, rail system

**AK-47**
- **File:** `WP_AK47.fbx`
- **Polycount:** 4,000-5,000 triangles
- **LODs:** 2
- **Features:** Wooden furniture, curved magazine

**SCAR-H**
- **File:** `WP_SCARH.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2
- **Features:** Modern design, rail system

**G36**
- **File:** `WP_G36.fbx`
- **Polycount:** 4,000-5,000 triangles
- **LODs:** 2

**FAL**
- **File:** `WP_FAL.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2

**ACR**
- **File:** `WP_ACR.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2
- **Features:** Modular design

---

#### 1.2.5 Machine Guns

**M249 SAW**
- **File:** `WP_M249.fbx`
- **Polycount:** 5,000-6,000 triangles
- **LODs:** 2
- **Features:** Belt-fed, bipod

**M60**
- **File:** `WP_M60.fbx`
- **Polycount:** 5,000-6,000 triangles
- **LODs:** 2

**PKM**
- **File:** `WP_PKM.fbx`
- **Polycount:** 5,000-6,000 triangles
- **LODs:** 2

**M27 IAR**
- **File:** `WP_M27.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2

---

#### 1.2.6 Sniper Rifles

**M40A5**
- **File:** `WP_M40A5.fbx`
- **Polycount:** 4,000-5,000 triangles
- **LODs:** 2
- **Features:** Bolt-action, scope

**SV-98**
- **File:** `WP_SV98.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2

**Barrett M82**
- **File:** `WP_BarrettM82.fbx`
- **Polycount:** 5,000-6,000 triangles
- **LODs:** 2
- **Features:** .50 caliber, massive size

**Dragunov SVD**
- **File:** `WP_SVD.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2

**AWM**
- **File:** `WP_AWM.fbx`
- **Polycount:** 4,500-5,500 triangles
- **LODs:** 2

---

#### 1.2.7 Heavy Weapons

**M79 Grenade Launcher**
- **File:** `WP_M79.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2
- **Features:** Break-action, short barrel

**M203 Underbarrel**
- **File:** `WP_M203.fbx`
- **Polycount:** 2,000-3,000 triangles
- **LODs:** 2

**RPG-7**
- **File:** `WP_RPG7.fbx`
- **Polycount:** 4,000-5,000 triangles
- **LODs:** 2
- **Features:** Rocket tube, detailed launch mechanism

**M134 Minigun**
- **File:** `WP_M134.fbx`
- **Polycount:** 8,000-10,000 triangles
- **LODs:** 2
- **Features:** Rotating barrels, ammo feed

**AT4 Anti-Tank**
- **File:** `WP_AT4.fbx`
- **Polycount:** 3,000-4,000 triangles
- **LODs:** 2
- **Features:** Disposable launcher

---

#### 1.2.8 Melee Weapons

**Combat Knife**
- **File:** `WP_CombatKnife.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Machete**
- **File:** `WP_Machete.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Crowbar**
- **File:** `WP_Crowbar.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Entrenching Tool**
- **File:** `WP_EntrenchingTool.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Baseball Bat**
- **File:** `WP_BaseballBat.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Fire Axe**
- **File:** `WP_FireAxe.fbx`
- **Polycount:** 1,000-1,500 triangles
- **LODs:** 1

---

#### 1.2.9 Grenades

**M67 Fragmentation**
- **File:** `WP_M67Grenade.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1
- **Features:** Spoon, pin, detailed texture

**M84 Stun**
- **File:** `WP_M84Grenade.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**AN-M14 Incendiary**
- **File:** `WP_ANM14Grenade.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**RGD-5**
- **File:** `WP_RGD5Grenade.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Smoke Grenade**
- **File:** `WP_SmokeGrenade.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Flashbang**
- **File:** `WP_Flashbang.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

**Molotov**
- **File:** `WP_Molotov.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1
- **Features:** Bottle, rag, liquid inside

**C4 Explosive**
- **File:** `WP_C4.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1
- **Features:** Plastic blocks, detonator

**Remote Mine**
- **File:** `WP_RemoteMine.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 1

---

### 1.3 Environment Models

#### 1.3.1 Buildings

**Wooden House (Destructible)**
- **File:** `SM_Building_WoodenHouse.fbx`
- **Polycount:** 5,000-10,000 triangles
- **LODs:** 3
- **Features:** Full destruction, separate parts

**Concrete Building (Partial Destruction)**
- **File:** `SM_Building_Concrete.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Features:** Fractured meshes, debris

**Factory Building**
- **File:** `SM_Building_Factory.fbx`
- **Polycount:** 15,000-20,000 triangles
- **LODs:** 3

**Hangar**
- **File:** `SM_Building_Hangar.fbx`
- **Polycount:** 20,000-30,000 triangles
- **LODs:** 3

**Bridge Section**
- **File:** `SM_Bridge_Section.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3

**Tunnel Section**
- **File:** `SM_Tunnel_Section.fbx`
- **Polycount:** 5,000-10,000 triangles
- **LODs:** 3

**Bunker**
- **File:** `SM_Bunker.fbx`
- **Polycount:** 15,000-20,000 triangles
- **LODs:** 3

---

#### 1.3.2 Props

**Wooden Crate**
- **File:** `SM_Prop_Crate_Wood.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 2
- **Features:** Destructible, multiple variants

**Metal Crate**
- **File:** `SM_Prop_Crate_Metal.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 2

**Barrel (Explosive)**
- **File:** `SM_Prop_Barrel.fbx`
- **Polycount:** 1,000-1,500 triangles
- **LODs:** 2
- **Features:** Red color, explosive tag

**Sandbags**
- **File:** `SM_Prop_Sandbags.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 2
- **Features:** Multiple configurations

**Table**
- **File:** `SM_Prop_Table.fbx`
- **Polycount:** 1,000-1,500 triangles
- **LODs:** 2

**Chair**
- **File:** `SM_Prop_Chair.fbx`
- **Polycount:** 500-1,000 triangles
- **LODs:** 2

**Bed**
- **File:** `SM_Prop_Bed.fbx`
- **Polycount:** 1,500-2,000 triangles
- **LODs:** 2

**Desk**
- **File:** `SM_Prop_Desk.fbx`
- **Polycount:** 1,500-2,000 triangles
- **LODs:** 2

**Cabinet**
- **File:** `SM_Prop_Cabinet.fbx`
- **Polycount:** 1,000-1,500 triangles
- **LODs:** 2

---

#### 1.3.3 Vehicles

**Jeep**
- **File:** `SM_Vehicle_Jeep.fbx`
- **Polycount:** 10,000-15,000 triangles
- **LODs:** 3
- **Features:** Destructible, mounted weapon

**Truck**
- **File:** `SM_Vehicle_Truck.fbx`
- **Polycount:** 15,000-20,000 triangles
- **LODs:** 3

**APC**
- **File:** `SM_Vehicle_APC.fbx`
- **Polycount:** 20,000-30,000 triangles
- **LODs:** 3
- **Features:** Turret, tracks

**Helicopter**
- **File:** `SM_Vehicle_Helicopter.fbx`
- **Polycount:** 15,000-25,000 triangles
- **LODs:** 3
- **Features:** Rotating rotors

**Tank**
- **File:** `SM_Vehicle_Tank.fbx`
- **Polycount:** 30,000-40,000 triangles
- **LODs:** 3
- **Features:** Turret, tracks, main cannon

---

#### 1.3.4 Fractured Meshes

All destructible objects need fractured versions:
- **Naming:** `SM_[Name]_Fractured.fbx`
- **Chunks:** 10-50 pieces depending on object size
- **LODs:** 1-2
- **Features:** Proper collision, physics materials

Examples:
- `SM_Building_WoodenHouse_Fractured.fbx`
- `SM_Prop_Crate_Wood_Fractured.fbx`
- `SM_Prop_Barrel_Fractured.fbx`

---

#### 1.3.5 Debris Models

**Wood Debris**
- **File:** `SM_Debris_Wood_[Size].fbx`
- **Polycount:** 50-500 triangles
- **LODs:** 1
- **Sizes:** Small, Medium, Large

**Concrete Debris**
- **File:** `SM_Debris_Concrete_[Size].fbx`
- **Polycount:** 50-500 triangles
- **LODs:** 1

**Metal Debris**
- **File:** `SM_Debris_Metal_[Size].fbx`
- **Polycount:** 50-500 triangles
- **LODs:** 1

**Glass Shards**
- **File:** `SM_Debris_Glass_[Size].fbx`
- **Polycount:** 50-200 triangles
- **LODs:** 1

---

## 2. TEXTURES

### 2.1 Character Textures

#### 2.1.1 Player Character

**Diffuse:**
- **File:** `T_JackKellar_Diffuse.png`
- **Size:** 4096x4096
- **Format:** RGBA8
- **Features:** High-detail skin, clothing

**Normal:**
- **File:** `T_JackKellar_Normal.png`
- **Size:** 4096x4096
- **Format:** RGBA8

**Specular/Roughness/Metallic:**
- **File:** `T_JackKellar_SRM.png` (Packed)
- **Size:** 4096x4096
- **Format:** RGBA8

**Emissive (Optional):**
- **File:** `T_JackKellar_Emissive.png`
- **Size:** 2048x2048
- **Format:** RGBA8

**Mask (Optional):**
- **File:** `T_JackKellar_Mask.png`
- **Size:** 2048x2048
- **Format:** RGBA8

---

#### 2.1.2 Enemy Characters

Each enemy type needs:
- Diffuse: 2048x2048 or 4096x4096
- Normal: 2048x2048 or 4096x4096
- SRM (Specular/Roughness/Metallic): 2048x2048 or 4096x4096

**Grunt:**
- `T_Enemy_Grunt_Diffuse.png`
- `T_Enemy_Grunt_Normal.png`
- `T_Enemy_Grunt_SRM.png`

**Soldier:**
- `T_Enemy_Soldier_Diffuse.png`
- `T_Enemy_Soldier_Normal.png`
- `T_Enemy_Soldier_SRM.png`

**Veteran:**
- `T_Enemy_Veteran_Diffuse.png`
- `T_Enemy_Veteran_Normal.png`
- `T_Enemy_Veteran_SRM.png`

**Sniper:**
- `T_Enemy_Sniper_Diffuse.png` (Ghillie suit variant)
- `T_Enemy_Sniper_Normal.png`
- `T_Enemy_Sniper_SRM.png`

**Shotgunner:**
- `T_Enemy_Shotgunner_Diffuse.png`
- `T_Enemy_Shotgunner_Normal.png`
- `T_Enemy_Shotgunner_SRM.png`

**Machine Gunner:**
- `T_Enemy_MachineGunner_Diffuse.png`
- `T_Enemy_MachineGunner_Normal.png`
- `T_Enemy_MachineGunner_SRM.png`

**Grenadier:**
- `T_Enemy_Grenadier_Diffuse.png`
- `T_Enemy_Grenadier_Normal.png`
- `T_Enemy_Grenadier_SRM.png`

**Engineer:**
- `T_Enemy_Engineer_Diffuse.png`
- `T_Enemy_Engineer_Normal.png`
- `T_Enemy_Engineer_SRM.png`

**Heavy:**
- `T_Enemy_Heavy_Diffuse.png`
- `T_Enemy_Heavy_Normal.png`
- `T_Enemy_Heavy_SRM.png`

**RPG Trooper:**
- `T_Enemy_RPG_Diffuse.png`
- `T_Enemy_RPG_Normal.png`
- `T_Enemy_RPG_SRM.png`

**Shield Bearer:**
- `T_Enemy_Shield_Diffuse.png`
- `T_Enemy_Shield_Normal.png`
- `T_Enemy_Shield_SRM.png`

**Commander:**
- `T_Enemy_Commander_Diffuse.png`
- `T_Enemy_Commander_Normal.png`
- `T_Enemy_Commander_SRM.png`

**Spetsnaz:**
- `T_Enemy_Spetsnaz_Diffuse.png`
- `T_Enemy_Spetsnaz_Normal.png`
- `T_Enemy_Spetsnaz_SRM.png`

**Black Ops:**
- `T_Enemy_BlackOps_Diffuse.png`
- `T_Enemy_BlackOps_Normal.png`
- `T_Enemy_BlackOps_SRM.png`

**Boss (Colonel Orlov):**
- `T_Boss_Orlov_Diffuse.png` (4096x4096)
- `T_Boss_Orlov_Normal.png` (4096x4096)
- `T_Boss_Orlov_SRM.png` (4096x4096)

---

### 2.2 Weapon Textures

Each weapon needs:
- Diffuse: 2048x2048
- Normal: 2048x2048
- SRM: 2048x2048

**Pistols:**
- `T_WP_M9_Diffuse.png`
- `T_WP_M9_Normal.png`
- `T_WP_M9_SRM.png`
- `T_WP_Glock17_Diffuse.png`
- `T_WP_Glock17_Normal.png`
- `T_WP_Glock17_SRM.png`
- And so on for all pistols...

**Shotguns:**
- `T_WP_Mossberg500_Diffuse.png`
- `T_WP_Mossberg500_Normal.png`
- `T_WP_Mossberg500_SRM.png`
- And so on for all shotguns...

**Assault Rifles:**
- `T_WP_M4_Diffuse.png`
- `T_WP_M4_Normal.png`
- `T_WP_M4_SRM.png`
- And so on for all assault rifles...

**Machine Guns:**
- `T_WP_M249_Diffuse.png`
- `T_WP_M249_Normal.png`
- `T_WP_M249_SRM.png`
- And so on for all machine guns...

**Sniper Rifles:**
- `T_WP_M40A5_Diffuse.png`
- `T_WP_M40A5_Normal.png`
- `T_WP_M40A5_SRM.png`
- And so on for all sniper rifles...

**Heavy Weapons:**
- `T_WP_RPG7_Diffuse.png`
- `T_WP_RPG7_Normal.png`
- `T_WP_RPG7_SRM.png`
- And so on for all heavy weapons...

**Melee Weapons:**
- `T_WP_CombatKnife_Diffuse.png`
- `T_WP_CombatKnife_Normal.png`
- `T_WP_CombatKnife_SRM.png`
- And so on for all melee weapons...

---

### 2.3 Environment Textures

#### 2.3.1 Building Textures

**Wood:**
- `T_Building_Wood_Diffuse.png` (2048x2048)
- `T_Building_Wood_Normal.png` (2048x2048)
- `T_Building_Wood_SRM.png` (2048x2048)
- `T_Building_Wood_Weathered_Diffuse.png` (2048x2048)
- `T_Building_Wood_Weathered_Normal.png` (2048x2048)

**Concrete:**
- `T_Building_Concrete_Diffuse.png` (2048x2048)
- `T_Building_Concrete_Normal.png` (2048x2048)
- `T_Building_Concrete_SRM.png` (2048x2048)
- `T_Building_Concrete_Cracked_Diffuse.png` (2048x2048)

**Brick:**
- `T_Building_Brick_Diffuse.png` (2048x2048)
- `T_Building_Brick_Normal.png` (2048x2048)
- `T_Building_Brick_SRM.png` (2048x2048)

**Metal:**
- `T_Building_Metal_Diffuse.png` (2048x2048)
- `T_Building_Metal_Normal.png` (2048x2048)
- `T_Building_Metal_SRM.png` (2048x2048)
- `T_Building_Metal_Rusted_Diffuse.png` (2048x2048)

**Glass:**
- `T_Building_Glass_Diffuse.png` (1024x1024)
- `T_Building_Glass_Normal.png` (1024x1024)
- `T_Building_Glass_SRM.png` (1024x1024)

---

#### 2.3.2 Ground Textures

**Dirt:**
- `T_Ground_Dirt_Diffuse.png` (2048x2048)
- `T_Ground_Dirt_Normal.png` (2048x2048)
- `T_Ground_Dirt_SRM.png` (2048x2048)

**Gravel:**
- `T_Ground_Gravel_Diffuse.png` (2048x2048)
- `T_Ground_Gravel_Normal.png` (2048x2048)
- `T_Ground_Gravel_SRM.png` (2048x2048)

**Asphalt:**
- `T_Ground_Asphalt_Diffuse.png` (2048x2048)
- `T_Ground_Asphalt_Normal.png` (2048x2048)
- `T_Ground_Asphalt_SRM.png` (2048x2048)

**Concrete:**
- `T_Ground_Concrete_Diffuse.png` (2048x2048)
- `T_Ground_Concrete_Normal.png` (2048x2048)
- `T_Ground_Concrete_SRM.png` (2048x2048)

**Snow:**
- `T_Ground_Snow_Diffuse.png` (2048x2048)
- `T_Ground_Snow_Normal.png` (2048x2048)
- `T_Ground_Snow_SRM.png` (2048x2048)

---

#### 2.3.3 Prop Textures

**Wooden Crate:**
- `T_Prop_Crate_Wood_Diffuse.png` (1024x1024)
- `T_Prop_Crate_Wood_Normal.png` (1024x1024)
- `T_Prop_Crate_Wood_SRM.png` (1024x1024)

**Metal Crate:**
- `T_Prop_Crate_Metal_Diffuse.png` (1024x1024)
- `T_Prop_Crate_Metal_Normal.png` (1024x1024)
- `T_Prop_Crate_Metal_SRM.png` (1024x1024)

**Barrel:**
- `T_Prop_Barrel_Diffuse.png` (1024x1024)
- `T_Prop_Barrel_Normal.png` (1024x1024)
- `T_Prop_Barrel_SRM.png` (1024x1024)

**Sandbags:**
- `T_Prop_Sandbags_Diffuse.png` (1024x1024)
- `T_Prop_Sandbags_Normal.png` (1024x1024)
- `T_Prop_Sandbags_SRM.png` (1024x1024)

---

#### 2.3.4 Vehicle Textures

**Jeep:**
- `T_Vehicle_Jeep_Diffuse.png` (2048x2048)
- `T_Vehicle_Jeep_Normal.png` (2048x2048)
- `T_Vehicle_Jeep_SRM.png` (2048x2048)

**Truck:**
- `T_Vehicle_Truck_Diffuse.png` (2048x2048)
- `T_Vehicle_Truck_Normal.png` (2048x2048)
- `T_Vehicle_Truck_SRM.png` (2048x2048)

**APC:**
- `T_Vehicle_APC_Diffuse.png` (2048x2048)
- `T_Vehicle_APC_Normal.png` (2048x2048)
- `T_Vehicle_APC_SRM.png` (2048x2048)

---

### 2.4 Decal Textures

**Bullet Holes:**
- `T_Decal_BulletHole_Concrete.png` (512x512)
- `T_Decal_BulletHole_Wood.png` (512x512)
- `T_Decal_BulletHole_Metal.png` (512x512)
- `T_Decal_BulletHole_Glass.png` (512x512)

**Explosion Marks:**
- `T_Decal_Explosion_Large.png` (1024x1024)
- `T_Decal_Explosion_Medium.png` (512x512)
- `T_Decal_Explosion_Small.png` (256x256)

**Blood Splatter:**
- `T_Decal_Blood_Splatter.png` (512x512)
- `T_Decal_Blood_Pool.png` (512x512)
- `T_Decal_Blood_Trail.png` (256x256)

**Dirt/Smudges:**
- `T_Decal_Dirt.png` (512x512)
- `T_Decal_Smudge.png` (512x512)

---

## 3. ANIMATIONS

### 3.1 Character Animations

#### 3.1.1 Player Character

**Locomotion:**
- `A_JackKellar_Idle` - Standing idle
- `A_JackKellar_Walk` - Walking forward
- `A_JackKellar_Walk_Back` - Walking backward
- `A_JackKellar_Walk_Left` - Walking left
- `A_JackKellar_Walk_Right` - Walking right
- `A_JackKellar_Run` - Running forward
- `A_JackKellar_Run_Back` - Running backward
- `A_JackKellar_Crouch_Idle` - Crouching idle
- `A_JackKellar_Crouch_Walk` - Crouch walking
- `A_JackKellar_Prone_Idle` - Prone idle
- `A_JackKellar_Prone_Crawl` - Prone crawling

**Jumping/Climbing:**
- `A_JackKellar_Jump_Start` - Jump start
- `A_JackKellar_Jump_Loop` - Jump loop
- `A_JackKellar_Jump_Land` - Jump landing
- `A_JackKellar_Jump_Land_Hard` - Hard landing
- `A_JackKellar_Jump_Land_Roll` - Landing roll
- `A_JackKellar_Mantle_Short` - Short mantle
- `A_JackKellar_Mantle_Medium` - Medium mantle
- `A_JackKellar_Mantle_High` - High mantle
- `A_JackKellar_Climb_Up` - Climbing up
- `A_JackKellar_Climb_Down` - Climbing down

**Cover:**
- `A_JackKellar_Cover_Enter_Left` - Enter left cover
- `A_JackKellar_Cover_Enter_Right` - Enter right cover
- `A_JackKellar_Cover_Exit_Left` - Exit left cover
- `A_JackKellar_Cover_Exit_Right` - Exit right cover
- `A_JackKellar_Cover_Idle_Left` - Left cover idle
- `A_JackKellar_Cover_Idle_Right` - Right cover idle
- `A_JackKellar_Cover_Lean_Left` - Lean left from cover
- `A_JackKellar_Cover_Lean_Right` - Lean right from cover
- `A_JackKellar_Cover_Lean_Back` - Lean back from cover

**Combat:**
- `A_JackKellar_Fire_Pistol` - Pistol firing
- `A_JackKellar_Fire_Rifle` - Rifle firing
- `A_JackKellar_Fire_Shotgun` - Shotgun firing
- `A_JackKellar_Fire_Sniper` - Sniper firing
- `A_JackKellar_Fire_MachineGun` - Machine gun firing
- `A_JackKellar_Reload_Pistol` - Pistol reload
- `A_JackKellar_Reload_Rifle` - Rifle reload
- `A_JackKellar_Reload_Shotgun` - Shotgun reload
- `A_JackKellar_Reload_Sniper` - Sniper reload
- `A_JackKellar_Melee_Swing` - Melee swing
- `A_JackKellar_Melee_Stab` - Melee stab
- `A_JackKellar_Melee_Kick` - Melee kick
- `A_JackKellar_Throw_Grenade` - Grenade throw
- `A_JackKellar_Aim_Idle` - Aiming idle
- `A_JackKellar_Aim_Walk` - Aiming walk

**Damage/Death:**
- `A_JackKellar_Hit_Head` - Head hit reaction
- `A_JackKellar_Hit_Body` - Body hit reaction
- `A_JackKellar_Hit_Arm_Left` - Left arm hit
- `A_JackKellar_Hit_Arm_Right` - Right arm hit
- `A_JackKellar_Hit_Leg_Left` - Left leg hit
- `A_JackKellar_Hit_Leg_Right` - Right leg hit
- `A_JackKellar_Death_Forward` - Death falling forward
- `A_JackKellar_Death_Backward` - Death falling backward
- `A_JackKellar_Death_Left` - Death falling left
- `A_JackKellar_Death_Right` - Death falling right
- `A_JackKellar_Death_Headshot` - Headshot death

**Special:**
- `A_JackKellar_Slide` - Sliding animation
- `A_JackKellar_Dive` - Diving/rolling
- `A_JackKellar_Interact` - Interaction animation
- `A_JackKellar_Pickup` - Picking up item

---

#### 3.1.2 Enemy Animations

Each enemy type needs similar animations to the player, but with variations:

**Basic Locomotion (All Enemies):**
- `A_Enemy_[Type]_Idle`
- `A_Enemy_[Type]_Walk`
- `A_Enemy_[Type]_Run`
- `A_Enemy_[Type]_Crouch_Idle`
- `A_Enemy_[Type]_Crouch_Walk`

**Combat (All Enemies):**
- `A_Enemy_[Type]_Fire_[Weapon]`
- `A_Enemy_[Type]_Reload_[Weapon]`
- `A_Enemy_[Type]_Aim`
- `A_Enemy_[Type]_Throw_Grenade`
- `A_Enemy_[Type]_Melee`

**Cover (All Enemies):**
- `A_Enemy_[Type]_Cover_Enter`
- `A_Enemy_[Type]_Cover_Exit`
- `A_Enemy_[Type]_Cover_Idle`
- `A_Enemy_[Type]_Cover_Fire`

**Damage/Death (All Enemies):**
- `A_Enemy_[Type]_Hit_[Location]`
- `A_Enemy_[Type]_Death_[Variation]`

**Special (Type-Specific):**
- `A_Enemy_Sniper_Aim_DownSights`
- `A_Enemy_Shotgunner_Pump`
- `A_Enemy_MachineGunner_DeployBipod`
- `A_Enemy_Shield_Raise`
- `A_Enemy_Shield_Lower`
- `A_Enemy_Grenadier_PrimeGrenade`

---

### 3.2 Weapon Animations

Each weapon needs:
- `A_WP_[Weapon]_Fire` - Firing animation
- `A_WP_[Weapon]_Reload` - Reload animation
- `A_WP_[Weapon]_Reload_Tactical` - Tactical reload
- `A_WP_[Weapon]_Cock` - Cocking (for shotguns, etc.)
- `A_WP_[Weapon]_Idle` - Idle animation
- `A_WP_[Weapon]_Draw` - Drawing weapon
- `A_WP_[Weapon]_Holster` - Holstering weapon

**Pistols:**
- `A_WP_M9_Fire`
- `A_WP_M9_Reload`
- `A_WP_M9_Cock`
- And so on for all pistols...

**Shotguns:**
- `A_WP_Mossberg500_Fire`
- `A_WP_Mossberg500_Pump`
- `A_WP_Mossberg500_Reload`
- And so on for all shotguns...

**Assault Rifles:**
- `A_WP_M4_Fire`
- `A_WP_M4_Reload`
- `A_WP_M4_Cock`
- And so on for all assault rifles...

**Machine Guns:**
- `A_WP_M249_Fire`
- `A_WP_M249_Reload`
- `A_WP_M249_DeployBipod`
- And so on for all machine guns...

**Sniper Rifles:**
- `A_WP_M40A5_Fire`
- `A_WP_M40A5_Reload`
- `A_WP_M40A5_BoltAction`
- And so on for all sniper rifles...

---

## 4. AUDIO ASSETS

### 4.1 Weapon Sounds

#### 4.1.1 Pistols

**M9 Beretta:**
- `A_WP_M9_Fire_01.wav` - Fire sound 1
- `A_WP_M9_Fire_02.wav` - Fire sound 2
- `A_WP_M9_Fire_03.wav` - Fire sound 3
- `A_WP_M9_Reload.wav` - Reload sound
- `A_WP_M9_Cock.wav` - Cocking sound
- `A_WP_M9_DryFire.wav` - Dry fire
- `A_WP_M9_OutOfAmmo.wav` - Out of ammo click

**Glock 17:**
- `A_WP_Glock17_Fire_01.wav`
- `A_WP_Glock17_Fire_02.wav`
- `A_WP_Glock17_Fire_03.wav`
- `A_WP_Glock17_Reload.wav`
- `A_WP_Glock17_Cock.wav`

**Desert Eagle:**
- `A_WP_DesertEagle_Fire_01.wav` (deep, powerful)
- `A_WP_DesertEagle_Fire_02.wav`
- `A_WP_DesertEagle_Reload.wav`

---

#### 4.1.2 Shotguns

**Mossberg 500:**
- `A_WP_Mossberg500_Fire.wav` (deep boom)
- `A_WP_Mossberg500_Pump.wav` - Pump action
- `A_WP_Mossberg500_Reload.wav`
- `A_WP_Mossberg500_ShellEject.wav` - Shell ejection

**Remington 870:**
- `A_WP_Remington870_Fire.wav`
- `A_WP_Remington870_Pump.wav`
- `A_WP_Remington870_Reload.wav`

**Saiga-12:**
- `A_WP_Saiga12_Fire.wav` (rapid, semi-auto)
- `A_WP_Saiga12_Reload.wav`

---

#### 4.1.3 Submachine Guns

**MP5:**
- `A_WP_MP5_Fire_01.wav` (rapid, high-pitched)
- `A_WP_MP5_Fire_02.wav`
- `A_WP_MP5_Fire_03.wav`
- `A_WP_MP5_Reload.wav`

**UMP:**
- `A_WP_UMP_Fire_01.wav` (deep, .45 cal)
- `A_WP_UMP_Fire_02.wav`
- `A_WP_UMP_Reload.wav`

---

#### 4.1.4 Assault Rifles

**M4 Carbine:**
- `A_WP_M4_Fire_01.wav` (5.56mm crack)
- `A_WP_M4_Fire_02.wav`
- `A_WP_M4_Fire_03.wav`
- `A_WP_M4_Reload.wav`
- `A_WP_M4_MagazineOut.wav`
- `A_WP_M4_MagazineIn.wav`

**AK-47:**
- `A_WP_AK47_Fire_01.wav` (7.62mm thump)
- `A_WP_AK47_Fire_02.wav`
- `A_WP_AK47_Reload.wav`
- `A_WP_AK47_MagazineOut.wav`

---

#### 4.1.5 Machine Guns

**M249 SAW:**
- `A_WP_M249_Fire_Loop.wav` (sustained fire)
- `A_WP_M249_Fire_Start.wav` - First shot
- `A_WP_M249_Fire_End.wav` - Last shot
- `A_WP_M249_Reload.wav` (long reload)
- `A_WP_M249_AmmoBox_Attach.wav`

**M60:**
- `A_WP_M60_Fire_Loop.wav`
- `A_WP_M60_Fire_Start.wav`
- `A_WP_M60_Reload.wav`

**M134 Minigun:**
- `A_WP_M134_SpinUp.wav` - Barrel spin up
- `A_WP_M134_SpinDown.wav` - Barrel spin down
- `A_WP_M134_Fire_Loop.wav` (very fast, continuous)

---

#### 4.1.6 Sniper Rifles

**M40A5:**
- `A_WP_M40A5_Fire.wav` (loud, echoing crack)
- `A_WP_M40A5_Reload.wav`
- `A_WP_M40A5_BoltAction.wav`

**Barrett M82:**
- `A_WP_BarrettM82_Fire.wav` (extremely loud, deep boom)
- `A_WP_BarrettM82_Reload.wav`

---

#### 4.1.7 Heavy Weapons

**RPG-7:**
- `A_WP_RPG7_Fire.wav` (whoosh + explosion)
- `A_WP_RPG7_Reload.wav`
- `A_WP_RPG7_TubeOpen.wav`

**AT4:**
- `A_WP_AT4_Fire.wav`

---

#### 4.1.8 Melee Weapons

**Combat Knife:**
- `A_WP_CombatKnife_Swing.wav`
- `A_WP_CombatKnife_Stab.wav`
- `A_WP_CombatKnife_Hit_Flesh.wav`
- `A_WP_CombatKnife_Hit_Metal.wav`

**Fire Axe:**
- `A_WP_FireAxe_Swing.wav`
- `A_WP_FireAxe_Hit.wav`

---

### 4.2 Environment Sounds

#### 4.2.1 Destruction Sounds

**Wood:**
- `A_Destruction_Wood_Crack_01.wav`
- `A_Destruction_Wood_Crack_02.wav`
- `A_Destruction_Wood_Splinter.wav`
- `A_Destruction_Wood_Collapse.wav`

**Concrete:**
- `A_Destruction_Concrete_Crack.wav`
- `A_Destruction_Concrete_Chunk.wav`
- `A_Destruction_Concrete_Collapse.wav`

**Metal:**
- `A_Destruction_Metal_Clang.wav`
- `A_Destruction_Metal_Screech.wav`
- `A_Destruction_Metal_Collapse.wav`

**Glass:**
- `A_Destruction_Glass_Shatter_01.wav`
- `A_Destruction_Glass_Shatter_02.wav`
- `A_Destruction_Glass_Break.wav`

---

#### 4.2.2 Impact Sounds

**Bullet Impacts:**
- `A_Impact_Bullet_Flesh.wav`
- `A_Impact_Bullet_Wood.wav`
- `A_Impact_Bullet_Concrete.wav`
- `A_Impact_Bullet_Metal.wav`
- `A_Impact_Bullet_Glass.wav`
- `A_Impact_Bullet_Sand.wav`

**Explosion Impacts:**
- `A_Impact_Explosion_Distant.wav`
- `A_Impact_Explosion_Near.wav`
- `A_Impact_Explosion_VeryNear.wav`

**Melee Impacts:**
- `A_Impact_Melee_Flesh.wav`
- `A_Impact_Melee_Metal.wav`
- `A_Impact_Melee_Wood.wav`
- `A_Impact_Melee_Concrete.wav`

---

#### 4.2.3 Ambient Sounds

**Weather:**
- `A_Ambient_Wind_Light.wav` (loop)
- `A_Ambient_Wind_Strong.wav` (loop)
- `A_Ambient_Rain_Light.wav` (loop)
- `A_Ambient_Rain_Heavy.wav` (loop)
- `A_Ambient_Thunder_Distant.wav`
- `A_Ambient_Thunder_Near.wav`
- `A_Ambient_Snow.wav` (loop)

**Environment:**
- `A_Ambient_Forest.wav` (loop)
- `A_Ambient_City_Ruins.wav` (loop)
- `A_Ambient_Factory.wav` (loop)
- `A_Ambient_Tunnel.wav` (loop)
- `A_Ambient_Indoor.wav` (loop)

**Fire:**
- `A_Ambient_Fire_Small.wav` (loop)
- `A_Ambient_Fire_Medium.wav` (loop)
- `A_Ambient_Fire_Large.wav` (loop)
- `A_Ambient_Fire_Crackle.wav` (loop)

---

### 4.3 Character Sounds

#### 4.3.1 Player Character

**Movement:**
- `A_Player_Footstep_Walk_Wood.wav`
- `A_Player_Footstep_Walk_Concrete.wav`
- `A_Player_Footstep_Walk_Metal.wav`
- `A_Player_Footstep_Walk_Gravel.wav`
- `A_Player_Footstep_Run_Wood.wav`
- `A_Player_Footstep_Run_Concrete.wav`
- `A_Player_Footstep_Crouch.wav`
- `A_Player_Footstep_Prone.wav`
- `A_Player_Jump.wav`
- `A_Player_Land.wav`
- `A_Player_Land_Hard.wav`
- `A_Player_Slide.wav`
- `A_Player_Mantle.wav`

**Breathing:**
- `A_Player_Breath_Idle.wav` (loop)
- `A_Player_Breath_Running.wav` (loop)
- `A_Player_Breath_Exhausted.wav` (loop)
- `A_Player_Gasp.wav`

**Pain:**
- `A_Player_Pain_01.wav`
- `A_Player_Pain_02.wav`
- `A_Player_Pain_03.wav`
- `A_Player_Pain_Headshot.wav`
- `A_Player_Pain_Critical.wav`

**Death:**
- `A_Player_Death_01.wav`
- `A_Player_Death_02.wav`
- `A_Player_Death_03.wav`
- `A_Player_Death_Headshot.wav`

**Voice Lines:**
- `A_Player_Voice_Ready.wav`
- `A_Player_Voice_Go.wav`
- `A_Player_Voice_CoverMe.wav`
- `A_Player_Voice_Reloading.wav`
- `A_Player_Voice_OutOfAmmo.wav`
- `A_Player_Voice_TakingFire.wav`
- `A_Player_Voice_EnemyDown.wav`
- `A_Player_Voice_Grenade.wav`

---

#### 4.3.2 Enemy Characters

Each enemy type needs voice lines in Russian:

**Grunt:**
- `A_Enemy_Grunt_Voice_Alert_01.wav`
- `A_Enemy_Grunt_Voice_Alert_02.wav`
- `A_Enemy_Grunt_Voice_Attack.wav`
- `A_Enemy_Grunt_Voice_Pain.wav`
- `A_Enemy_Grunt_Voice_Death.wav`

**Soldier:**
- `A_Enemy_Soldier_Voice_Alert.wav`
- `A_Enemy_Soldier_Voice_Flank.wav`
- `A_Enemy_Soldier_Voice_Suppress.wav`
- `A_Enemy_Soldier_Voice_Retreat.wav`

**Commander:**
- `A_Enemy_Commander_Voice_Order_Attack.wav`
- `A_Enemy_Commander_Voice_Order_Retreat.wav`
- `A_Enemy_Commander_Voice_Order_Flank.wav`
- `A_Enemy_Commander_Voice_Order_Regroup.wav`

**Radio Chatter:**
- `A_Enemy_Radio_Chatter_01.wav` (loop)
- `A_Enemy_Radio_Chatter_02.wav` (loop)
- `A_Enemy_Radio_Chatter_03.wav` (loop)

---

#### 4.3.3 Civilian Sounds

**Pain:**
- `A_Civilian_Pain_01.wav`
- `A_Civilian_Pain_02.wav`

**Death:**
- `A_Civilian_Death_01.wav`
- `A_Civilian_Death_02.wav`

**Voice Lines:**
- `A_Civilian_Voice_Help.wav`
- `A_Civilian_Voice_Please.wav`
- `A_Civilian_Voice_Run.wav`
- `A_Civilian_Voice_Hide.wav`

---

### 4.4 Grenade Sounds

**M67 Fragmentation:**
- `A_Grenade_M67_Arm.wav`
- `A_Grenade_M67_Explosion.wav` (loud, with debris)

**M84 Stun:**
- `A_Grenade_M84_Arm.wav`
- `A_Grenade_M84_Explosion.wav` (pop + buzz)

**AN-M14 Incendiary:**
- `A_Grenade_ANM14_Arm.wav`
- `A_Grenade_ANM14_Explosion.wav` (whoosh + fire)

**Smoke Grenade:**
- `A_Grenade_Smoke_Arm.wav`
- `A_Grenade_Smoke_Explosion.wav` (pop + hiss)

**Flashbang:**
- `A_Grenade_Flashbang_Arm.wav`
- `A_Grenade_Flashbang_Explosion.wav` (bright pop)

**Molotov:**
- `A_Grenade_Molotov_Throw.wav`
- `A_Grenade_Molotov_Shatter.wav`
- `A_Grenade_Molotov_Fire.wav` (loop)

**C4:**
- `A_Grenade_C4_Arm.wav`
- `A_Grenade_C4_Beep.wav` (loop)
- `A_Grenade_C4_Explosion.wav` (massive)

---

### 4.5 Vehicle Sounds

**Jeep:**
- `A_Vehicle_Jeep_Engine_Idle.wav` (loop)
- `A_Vehicle_Jeep_Engine_Rev.wav`
- `A_Vehicle_Jeep_Engine_Accelerate.wav`
- `A_Vehicle_Jeep_Tires_Gravel.wav` (loop)
- `A_Vehicle_Jeep_Tires_Asphalt.wav` (loop)
- `A_Vehicle_Jeep_Horn.wav`
- `A_Vehicle_Jeep_Crash.wav`

**Truck:**
- `A_Vehicle_Truck_Engine_Idle.wav` (loop, deep)
- `A_Vehicle_Truck_Engine_Rev.wav`
- `A_Vehicle_Truck_Tires.wav` (loop)

**APC:**
- `A_Vehicle_APC_Engine_Idle.wav` (loop, very deep)
- `A_Vehicle_APC_Engine_Move.wav` (loop)
- `A_Vehicle_APC_Tracks.wav` (loop)
- `A_Vehicle_APC_Turret_Rotate.wav` (loop)
- `A_Vehicle_APC_Cannon_Fire.wav`

**Helicopter:**
- `A_Vehicle_Helicopter_Engine.wav` (loop)
- `A_Vehicle_Helicopter_Rotor.wav` (loop)
- `A_Vehicle_Helicopter_Rotor_Start.wav`
- `A_Vehicle_Helicopter_Minigun_Fire.wav` (loop)

---

## 5. PARTICLE EFFECTS

### 5.1 Weapon Effects

**Muzzle Flash:**
- `P_WP_MuzzleFlash_Pistol` - Small, quick flash
- `P_WP_MuzzleFlash_Rifle` - Medium flash
- `P_WP_MuzzleFlash_Shotgun` - Large, bright flash
- `P_WP_MuzzleFlash_MachineGun` - Continuous flash
- `P_WP_MuzzleFlash_Sniper` - Large, visible flash

**Shell Ejection:**
- `P_WP_ShellEjection_Pistol`
- `P_WP_ShellEjection_Rifle`
- `P_WP_ShellEjection_Shotgun`

**Bullet Tracers:**
- `P_WP_BulletTracer_Standard`
- `P_WP_BulletTracer_Tracer` (bright, visible)
- `P_WP_BulletTracer_Incendiary` (with fire trail)

---

### 5.2 Impact Effects

**Bullet Impacts:**
- `P_Impact_Bullet_Concrete`
- `P_Impact_Bullet_Wood`
- `P_Impact_Bullet_Metal`
- `P_Impact_Bullet_Glass`
- `P_Impact_Bullet_Flesh`
- `P_Impact_Bullet_Sand`

**Explosion Effects:**
- `P_Explosion_Small`
- `P_Explosion_Medium`
- `P_Explosion_Large`
- `P_Explosion_Massive` (for RPG, etc.)

**Melee Impacts:**
- `P_Impact_Melee_Flesh` (blood spray)
- `P_Impact_Melee_Metal` (sparks)
- `P_Impact_Melee_Wood` (splinters)

---

### 5.3 Destruction Effects

**Wood Destruction:**
- `P_Destruction_Wood_Splinters`
- `P_Destruction_Wood_Dust`

**Concrete Destruction:**
- `P_Destruction_Concrete_Dust`
- `P_Destruction_Concrete_Chunks`

**Metal Destruction:**
- `P_Destruction_Metal_Sparks`
- `P_Destruction_Metal_Shards`

**Glass Destruction:**
- `P_Destruction_Glass_Shards`
- `P_Destruction_Glass_Dust`

---

### 5.4 Environmental Effects

**Weather:**
- `P_Weather_Rain`
- `P_Weather_Snow`
- `P_Weather_Fog`
- `P_Weather_Dust`

**Fire:**
- `P_Fire_Small`
- `P_Fire_Medium`
- `P_Fire_Large`
- `P_Fire_Smoke`

**Dust/Debris:**
- `P_Dust_Kickup` (from footsteps)
- `P_Debris_Generic`

---

## 6. MATERIALS

### 6.1 Master Materials

**M_Master_PBR** - Main PBR material with:
- Base Color
- Normal
- Roughness
- Metallic
- AO
- Emissive
- Opacity

**M_Master_Destructible** - For destructible objects:
- Damage texture blending
- Fracture parameters
- Debris spawning

**M_Master_Decal** - For decals:
- Diffuse
- Normal
- Roughness
- Opacity
- Fade over time

---

### 6.2 Character Materials

**M_Character_Skin**
- Subsurface scattering
- Skin normal map
- Porosity

**M_Character_Clothing**
- Fabric roughness
- Wear and tear
- Dirt accumulation

**M_Character_Gear**
- Metal/plastic PBR
- Scratches and wear

---

### 6.3 Weapon Materials

**M_Weapon_Metal**
- High metallic
- Low roughness
- Scratches

**M_Weapon_Wood**
- Medium roughness
- Wood grain normal

**M_Weapon_Plastic**
- Medium roughness
- Plastic sheen

---

### 6.4 Environment Materials

**M_Environment_Wood**
- Wood grain
- Weathering
- Paint options

**M_Environment_Concrete**
- Rough surface
- Cracks
- Dirt accumulation

**M_Environment_Metal**
- Rust
- Scratches
- Paint chipping

**M_Environment_Glass**
- Transparency
- Refraction
- Reflection

---

## 7. UI ASSETS

### 7.1 Fonts

**Font_Main** - Primary UI font (military style)
- Size: Multiple sizes (12pt, 14pt, 16pt, 18pt, 24pt, 32pt, 48pt, 64pt)
- Style: Bold, clean
- Format: TTF

**Font_Title** - Title/heading font
- Size: 48pt, 64pt, 96pt
- Style: Bold, impactful

---

### 7.2 HUD Elements

**Health Bar:**
- `UI_HUD_HealthBar_Background.png`
- `UI_HUD_HealthBar_Fill.png`
- `UI_HUD_HealthBar_Frame.png`

**Armor Bar:**
- `UI_HUD_ArmorBar_Background.png`
- `UI_HUD_ArmorBar_Fill.png`

**Ammo Counter:**
- `UI_HUD_Ammo_Background.png`
- `UI_HUD_Ammo_Icon.png`

**Weapon Icon:**
- `UI_HUD_Weapon_[WeaponType].png` (for each weapon type)

**Crosshair:**
- `UI_HUD_Crosshair_Default.png`
- `UI_HUD_Crosshair_Pistol.png`
- `UI_HUD_Crosshair_Shotgun.png`
- `UI_HUD_Crosshair_Sniper.png`
- `UI_HUD_Crosshair_MachineGun.png`

**Mini-Map:**
- `UI_HUD_MiniMap_Frame.png`
- `UI_HUD_MiniMap_Compass.png`
- `UI_HUD_MiniMap_PlayerIcon.png`
- `UI_HUD_MiniMap_EnemyIcon.png`
- `UI_HUD_MiniMap_ObjectiveIcon.png`

**Damage Indicator:**
- `UI_HUD_Damage_Indicator.png`
- `UI_HUD_Damage_Blood.png`

---

### 7.3 Menu Elements

**Main Menu:**
- `UI_Menu_Main_Background.png` (1920x1080)
- `UI_Menu_Main_Title.png` (game title)
- `UI_Menu_Main_Button_Normal.png`
- `UI_Menu_Main_Button_Hover.png`
- `UI_Menu_Main_Button_Pressed.png`

**Pause Menu:**
- `UI_Menu_Pause_Background.png`
- Same button assets as main menu

**Inventory Menu:**
- `UI_Menu_Inventory_Background.png`
- `UI_Menu_Inventory_Slot.png`
- `UI_Menu_Inventory_Slot_Selected.png`
- `UI_Menu_Inventory_WeaponIcon_[Weapon].png`

**Map Menu:**
- `UI_Menu_Map_Background.png`
- `UI_Menu_Map_Grid.png`
- `UI_Menu_Map_PlayerIcon.png`
- `UI_Menu_Map_ObjectiveIcon.png`

**Options Menu:**
- `UI_Menu_Options_Background.png`
- `UI_Menu_Options_Slider_Background.png`
- `UI_Menu_Options_Slider_Thumb.png`
- `UI_Menu_Options_Checkbox_Unchecked.png`
- `UI_Menu_Options_Checkbox_Checked.png`

---

### 7.4 Loading Screens

- `UI_Loading_Level_[LevelName].png` (1920x1080 for each level)
- `UI_Loading_Tips_[Number].png` (loading screen tips)
- `UI_Loading_ProgressBar.png`

---

### 7.5 Cutscene Assets

- `UI_Cutscene_Border.png` (cinematic border)
- `UI_Cutscene_Subtitle_Background.png`

---

## 8. LEVEL ASSETS

### 8.1 Level 1: Black Dawn

**Models:**
- `SM_Level1_FarmHouse.fbx`
- `SM_Level1_Barn.fbx`
- `SM_Level1_Fence.fbx`
- `SM_Level1_Tree.fbx` (multiple variants)
- `SM_Level1_Rock.fbx` (multiple variants)

**Textures:**
- `T_Level1_Ground_Dirt.png`
- `T_Level1_Ground_Grass.png`
- `T_Level1_Building_Wood.png`

---

### 8.2 Level 2: Scorched Earth

**Models:**
- `SM_Level2_Factory.fbx`
- `SM_Level2_ConveyorBelt.fbx`
- `SM_Level2_StorageTank.fbx`
- `SM_Level2_Machinery.fbx`

**Textures:**
- `T_Level2_Ground_Concrete.png`
- `T_Level2_Building_Industrial.png`

---

### 8.3 Level 3: Bridge of Sighs

**Models:**
- `SM_Level3_Bridge_Section.fbx` (multiple)
- `SM_Level3_Bridge_Support.fbx`
- `SM_Level3_Bridge_Cable.fbx`
- `SM_Level3_TollBooth.fbx`

**Textures:**
- `T_Level3_Bridge_Metal.png`
- `T_Level3_Bridge_Concrete.png`

---

### 8.4 Level 4: Ghost Town

**Models:**
- `SM_Level4_Building_Apartment.fbx`
- `SM_Level4_Building_Store.fbx`
- `SM_Level4_Street_Lamp.fbx`
- `SM_Level4_Car.fbx` (multiple variants)

**Textures:**
- `T_Level4_Ground_Asphalt.png`
- `T_Level4_Building_Brick.png`

---

### 8.5 Level 5: Factory of Death

**Models:**
- `SM_Level5_Factory_Large.fbx`
- `SM_Level5_Pipeline.fbx`
- `SM_Level5_ControlPanel.fbx`
- `SM_Level5_AmmoCrate.fbx`

**Textures:**
- `T_Level5_Building_Industrial.png`
- `T_Level5_Pipeline_Metal.png`

---

### 8.6 Level 6: Tunnel Vision

**Models:**
- `SM_Level6_Tunnel_Section.fbx`
- `SM_Level6_Tunnel_Support.fbx`
- `SM_Level6_Light.fbx`
- `SM_Level6_Ventilation.fbx`

**Textures:**
- `T_Level6_Tunnel_Concrete.png`
- `T_Level6_Tunnel_Metal.png`

---

### 8.7 Level 7: Mountain Fortress

**Models:**
- `SM_Level7_Bunker.fbx`
- `SM_Level7_Tower.fbx`
- `SM_Level7_Wall.fbx`
- `SM_Level7_Gate.fbx`

**Textures:**
- `T_Level7_Ground_Snow.png`
- `T_Level7_Building_Concrete.png`

---

### 8.8 Level 8: Heart of Darkness

**Models:**
- `SM_Level8_Bunker_Complex.fbx`
- `SM_Level8_Server.fbx`
- `SM_Level8_Computer.fbx`
- `SM_Level8_Light.fbx`

**Textures:**
- `T_Level8_Building_Metal.png`
- `T_Level8_Building_Concrete.png`

---

## TOTAL ASSET COUNT

### 3D Models: ~150
- Characters: 25
- Weapons: 30
- Environment: 50
- Vehicles: 5
- Props: 40

### Textures: ~300
- Character: 50
- Weapon: 60
- Environment: 100
- Decals: 20
- UI: 70

### Animations: ~200
- Character: 50
- Enemy: 100
- Weapon: 50

### Audio: ~500
- Weapon: 100
- Environment: 100
- Character: 150
- Vehicle: 50
- Grenade: 20
- Music: 20
- Ambient: 60

### Particle Effects: ~50
- Weapon: 20
- Impact: 15
- Destruction: 10
- Environmental: 5

### Materials: ~50
- Master: 5
- Character: 10
- Weapon: 10
- Environment: 25

---

**Total Assets: ~1,050**

---

## FILE SIZE ESTIMATES

- **3D Models:** ~5-10 GB (FBX + textures)
- **Textures:** ~2-5 GB
- **Audio:** ~1-2 GB
- **Particles:** ~500 MB
- **Total:** ~8-18 GB

---

## ORGANIZATION

```
Content/
├── Characters/
│   ├── JackKellar/
│   ├── Enemies/
│   └── Civilians/
├── Weapons/
│   ├── Pistols/
│   ├── Shotguns/
│   ├── SMGs/
│   ├── AssaultRifles/
│   ├── MachineGuns/
│   ├── SniperRifles/
│   ├── Heavy/
│   ├── Melee/
│   └── Grenades/
├── Environments/
│   ├── Buildings/
│   ├── Props/
│   ├── Vehicles/
│   ├── Fractured/
│   └── Debris/
├── Textures/
│   ├── Characters/
│   ├── Weapons/
│   ├── Environments/
│   ├── Decals/
│   └── UI/
├── Animations/
│   ├── Characters/
│   ├── Enemies/
│   └── Weapons/
├── Sounds/
│   ├── Weapons/
│   ├── Environment/
│   ├── Characters/
│   ├── Vehicles/
│   ├── Grenades/
│   ├── Music/
│   └── Ambient/
├── Effects/
│   ├── Particles/
│   └── Materials/
└── UI/
    ├── Fonts/
    ├── HUD/
    ├── Menus/
    └── LoadingScreens/
```

---

**END OF DOCUMENT**
