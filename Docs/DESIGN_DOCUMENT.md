# BLACK: REMASTERED - DESIGN DOCUMENT
## Complete Game Design Specification

---

## TABLE OF CONTENTS
1. [EXECUTIVE SUMMARY](#1-executive-summary)
2. [GAME OVERVIEW](#2-game-overview)
3. [STORY & NARRATIVE](#3-story--narrative)
4. [GAMEPLAY SYSTEMS](#4-gameplay-systems)
5. [WEAPONS & EQUIPMENT](#5-weapons--equipment)
6. [LEVEL DESIGN](#6-level-design)
7. [ENEMIES & AI](#7-enemies--ai)
8. [DESTRUCTION SYSTEM](#8-destruction-system)
9. [GRAPHICS & AUDIO](#9-graphics--audio)
10. [TECHNICAL SPECIFICATIONS](#10-technical-specifications)
11. [UI/UX DESIGN](#11-uiux-design)
12. [PROGRESSION SYSTEM](#12-progression-system)
13. [CONTROLS](#13-controls)
14. [ACCESSIBILITY](#14-accessibility)
15. [PLATFORM REQUIREMENTS](#15-platform-requirements)

---

## 1. EXECUTIVE SUMMARY

### 1.1 Project Overview
- **Title:** BLACK: REMASTERED (Working Title: Black Redux)
- **Genre:** Single-Player First-Person Shooter
- **Target Platform:** PC (Windows 10/11, 64-bit)
- **Target Engine:** Unreal Engine 5.4+
- **Target Audience:** Mature gamers (18+), FPS enthusiasts, fans of tactical shooters
- **Estimated Playtime:** 6-9 hours (main campaign)
- **Development Approach:** Autonomous AI-driven creation with human asset integration

### 1.2 Core Pillars
1. **DESTRUCTION IS KING:** Every surface, structure, and object can be destroyed with satisfying physics
2. **WEAPON PORN:** Each firearm has unique, weighty feel with authentic ballistics
3. **CINEMATIC BRUTALITY:** Gritty, filmic presentation with intense action sequences
4. **TACTICAL AGGRESSION:** Encourages aggressive playstyle with smart AI opponents
5. **AUTHENTIC ATMOSPHERE:** Eastern European warzone setting with realistic sound design

### 1.3 Unique Selling Points
- Unprecedented environmental destruction using UE5 Chaos Physics + Voxel-based systems
- Hyper-realistic weapon handling with custom ballistics simulation
- Dynamic AI that adapts to player's destructive tendencies
- Procedural damage system for all structures and vehicles
- 4K/120FPS support with full ray tracing implementation

---

## 2. GAME OVERVIEW

### 2.1 Concept
BLACK: REMASTERED is a complete reimagining of the 2006 cult classic FPS, built from the ground up for modern hardware. The game maintains the original's focus on sheer destructibility while expanding on every aspect: deeper narrative, more weapons, enhanced AI, and next-generation visuals.

### 2.2 Setting
- **Location:** Eastern Europe (Fictionalized Chechnya/Grozny region)
- **Time Period:** Modern day (2025-2026 timeline)
- **Atmosphere:** Gritty, war-torn, industrial, with heavy Russian/Soviet influences
- **Visual Style:** Dirty realism - not hyper-polished AAA, but authentically grimy and battle-scarred

### 2.3 Game Modes
- **Single-Player Campaign:** 8-10 missions, linear with branching paths
- **Difficulty Levels:**
  - Recruit (Easy) - For casual players
  - Veteran (Normal) - Balanced experience
  - Black Ops (Hard) - For FPS veterans
  - Hardcore (Unlockable) - Permadeath, limited saves

### 2.4 Target Experience
Players should feel like a one-man army, capable of laying waste to entire buildings with a single magazine. The game rewards aggression, precision, and creative use of the environment. Every shot should feel impactful, every explosion spectacular.

---

## 3. STORY & NARRATIVE

### 3.1 Narrative Structure
The story is told through a **non-linear framing device**: Jack Kellar is being interrogated about his actions in Eastern Europe. As he recounts his mission, players experience flashbacks of the actual events. This allows for:
- Unreliable narrator elements
- Multiple perspectives on key events
- Time jumps that reveal new information
- Psychological tension between interrogator and Kellar

### 3.2 Main Characters

#### 3.2.1 Jack Kellar (Player Character)
- **Age:** 38
- **Background:** Ex-SAS, now Black Ops specialist
- **Personality:** Stoic, professional, but with a dark sense of humor
- **Motivation:** Complete the mission, but begins to question his orders
- **Voice Actor:** Deep, gravelly voice (think Jason Statham/Idris Elba)

#### 3.2.2 The Interrogator
- **Role:** Unknown agency representative
- **Personality:** Cold, methodical, manipulative
- **Purpose:** Extract information about what really happened
- **Twist:** May have been involved in the original operation

#### 3.2.3 Key NPCs
- **Sergeant Viktor Petrov:** Russian defector providing intel
- **Captain Maria Volkov:** Local resistance leader
- **Colonel Alexei Orlov:** Main antagonist, former Spetsnaz
- **"The Ghost":** Mysterious figure from Kellar's past

### 3.3 Plot Summary

#### Act 1: The Setup
- Kellar is captured after a failed black ops mission in Eastern Europe
- Interrogation begins - Kellar recounts being sent to extract a defector
- Mission goes wrong - entire village is destroyed
- Flashbacks reveal Kellar's team was betrayed

#### Act 2: The Betrayal
- Kellar discovers his mission was a setup
- He's been used to eliminate a rogue Russian general
- The defector he was supposed to extract is actually the target
- Kellar goes rogue, determined to find the truth

#### Act 3: The Reckoning
- Kellar tracks down those responsible
- Final confrontation in a massive military complex
- The interrogator's true identity is revealed
- Multiple endings based on player choices during key moments

### 3.4 Key Story Moments
1. **Opening Ambush:** Team is wiped out in first 10 minutes
2. **Village Massacre:** Player can choose to save or abandon civilians
3. **Bridge Battle:** Iconic set-piece with massive destruction
4. **Factory Infiltration:** Stealth section with environmental kills
5. **Mountain Base:** Large-scale assault with vehicles
6. **Final Interrogation:** Truth is revealed, player makes final choice

### 3.5 Dialogue System
- **Full English Voice-Over:** All major characters and radio chatter
- **Polish Subtitles:** Complete localization
- **Dynamic Radio:** Enemies communicate in Russian with English subtitles
- **Interrogation Sequences:** Pre-rendered cutscenes with branching dialogue

---

## 4. GAMEPLAY SYSTEMS

### 4.1 Core Gameplay Loop
```
EXPLORE -> DESTROY -> ENGAGE -> PROGRESS -> UPGRADE -> REPEAT
```

### 4.2 Movement & Physics
- **Player Movement:**
  - Standard FPS controls (WASD + Mouse)
  - Sprint with stamina system
  - Slide (contextual)
  - Mantle over cover
  - No double-jump or advanced movement (keep it grounded)
  
- **Physics:**
  - Full ragdoll physics for all characters
  - Realistic bullet physics with penetration
  - Destructible cover system
  - Interactive objects (kick doors, move debris)

### 4.3 Health & Damage
- **Health System:**
  - Regenerating health (slow regen when not taking damage)
  - Health packs for instant healing
  - Armor system (absorbs % of damage)
  
- **Damage Types:**
  - Ballistic (bullets)
  - Explosive (grenades, rockets)
  - Fire (from explosions, environmental)
  - Melee (knife, butt-stock)
  - Fall damage

- **Hit Reactions:**
  - Screen shake intensity based on damage
  - Blood splatter on screen
  - Audio cues (heartbeat, pain grunts)
  - Visual wound effects on player model

### 4.4 Cover System (Optional)
- **Manual Cover:** Player can press against walls/objects
- **Auto-Cover:** Smart snapping to nearby cover
- **Cover Destruction:** All cover can be shot away
- **Lean Mechanic:** Left/Right lean for peeking
- **Blind Fire:** Ability to shoot without exposing self

### 4.5 Ammo & Inventory
- **Ammo Types:**
  - Standard (default)
  - Armor-Piercing (penetrates cover)
  - Incendiary (sets targets on fire)
  - Explosive (high damage, splash)
  - Hollow-Point (higher damage vs unarmored)
  
- **Inventory:**
  - Primary weapon (1)
  - Secondary weapon (1)
  - Throwables (grenades, etc.)
  - Melee weapon (1)
  - Limited ammo carry (realistic capacities)

### 4.6 Weapon Customization
- **Attachments:**
  - Scopes (Red Dot, ACOG, Sniper)
  - Suppressors (reduces noise, hides muzzle flash)
  - Extended Magazines (more ammo)
  - Foregrips (reduces recoil)
  - Bayonets (melee attacks)
  
- **Unlock System:**
  - New attachments unlocked via progression
  - Weapon-specific upgrades
  - Ammo type unlocks

### 4.7 Save System
- **Checkpoint System:** Auto-saves at key moments
- **Manual Saves:** Limited number (Hardcore: disabled)
- **Quick Save:** F5 key (disabled in Hardcore)
- **Cloud Saves:** Supported via UE5 systems

---

## 5. WEAPONS & EQUIPMENT

### 5.1 Weapon Categories

#### 5.1.1 Pistols (Secondary)
| Weapon | Caliber | Capacity | Rate of Fire | Unlock Level |
|--------|---------|----------|---------------|--------------|
| M9 Beretta | 9mm | 15+1 | Semi-Auto | Start |
| Glock 17 | 9mm | 17+1 | Semi-Auto | Start |
| Desert Eagle | .50 AE | 7+1 | Semi-Auto | Level 3 |
| Makarov PM | 9x18mm | 8+1 | Semi-Auto | Level 2 |
| SIG Sauer P226 | 9mm | 15+1 | Semi-Auto | Level 4 |

**Pistol Features:**
- Can be dual-wielded (reduced accuracy)
- Suppressor compatible
- Extended magazines available
- Akimbo mode for specific pistols

#### 5.1.2 Shotguns
| Weapon | Gauge | Capacity | Rate of Fire | Unlock Level |
|--------|-------|----------|---------------|--------------|
| Mossberg 500 | 12ga | 8+1 | Pump-Action | Start |
| Remington 870 | 12ga | 8+1 | Pump-Action | Start |
| Saiga-12 | 12ga | 10+1 | Semi-Auto | Level 2 |
| SPAS-12 | 12ga | 8+1 | Semi-Auto | Level 4 |
| Double Barrel | 12ga | 2 | Break-Action | Level 3 |

**Shotgun Features:**
- Different choke settings (spread patterns)
- Slug ammunition available
- Massive environmental destruction potential
- Pump-action animation with sound

#### 5.1.3 Submachine Guns
| Weapon | Caliber | Capacity | Rate of Fire | Unlock Level |
|--------|---------|----------|---------------|--------------|
| MP5 | 9mm | 30 | 900 RPM | Start |
| UMP | .45 ACP | 25 | 625 RPM | Level 2 |
| PP-19 Bizon | 9x18mm | 64 | 700 RPM | Level 3 |
| Kriss Vector | .45 ACP | 25 | 1200 RPM | Level 4 |

**SMG Features:**
- High mobility while firing
- Suppressor ready
- Large magazine capacities
- Good for close-quarters

#### 5.1.4 Assault Rifles
| Weapon | Caliber | Capacity | Rate of Fire | Unlock Level |
|--------|---------|----------|---------------|--------------|
| M4 Carbine | 5.56mm | 30 | 750 RPM | Start |
| AK-47 | 7.62mm | 30 | 600 RPM | Start |
| SCAR-H | 7.62mm | 20 | 625 RPM | Level 2 |
| G36 | 5.56mm | 30 | 750 RPM | Level 3 |
| FAL | 7.62mm | 20 | 650 RPM | Level 4 |
| ACR | 5.56mm | 30 | 700 RPM | Level 5 |

**Assault Rifle Features:**
- Three-round burst mode available
- Multiple optic attachments
- Underbarrel grenade launcher option
- Customizable fire modes

#### 5.1.5 Machine Guns
| Weapon | Caliber | Capacity | Rate of Fire | Unlock Level |
|--------|---------|----------|---------------|--------------|
| M249 SAW | 5.56mm | 200 | 750 RPM | Level 3 |
| M60 | 7.62mm | 100 | 550 RPM | Level 4 |
| PKM | 7.62mm | 100 | 650 RPM | Level 4 |
| M27 IAR | 5.56mm | 30 | 700 RPM | Level 5 |

**Machine Gun Features:**
- Bipod deployment for stability
- High ammo consumption
- Heavy recoil
- Suppression effect on enemies

#### 5.1.6 Sniper Rifles
| Weapon | Caliber | Capacity | Rate of Fire | Unlock Level |
|--------|---------|----------|---------------|--------------|
| M40A5 | 7.62mm | 5+1 | Bolt-Action | Level 2 |
| SV-98 | 7.62mm | 5+1 | Bolt-Action | Level 3 |
| Barrett M82 | .50 BMG | 10 | Semi-Auto | Level 4 |
| Dragunov SVD | 7.62mm | 10 | Semi-Auto | Level 3 |
| AWM | .338 Lapua | 5+1 | Bolt-Action | Level 5 |

**Sniper Features:**
- Bullet drop and windage simulation
- Scope sway based on player movement
- One-shot kills on headshots
- Suppressor compatible
- Spotter mode (highlight targets)

#### 5.1.7 Heavy Weapons
| Weapon | Caliber | Capacity | Rate of Fire | Unlock Level |
|--------|---------|----------|---------------|--------------|
| M79 Grenade Launcher | 40mm | 1 | Single-Shot | Level 2 |
| M203 Underbarrel | 40mm | 1 | Single-Shot | Level 1 |
| RPG-7 | 40mm | 1 | Single-Shot | Level 3 |
| M134 Minigun | 7.62mm | 200 | 3000 RPM | Level 4 |
| AT4 Anti-Tank | 84mm | 1 | Single-Shot | Level 5 |

**Heavy Weapon Features:**
- Massive environmental destruction
- Explosive radius effects
- Limited ammo supply
- Slow movement while equipped

#### 5.1.8 Throwables
| Item | Type | Quantity | Unlock Level |
|------|------|----------|--------------|
| M67 Fragmentation | Grenade | 3 | Start |
| M84 Stun | Grenade | 2 | Start |
| AN-M14 Incendiary | Grenade | 2 | Level 2 |
| RGD-5 | Grenade | 3 | Level 1 |
| Smoke Grenade | Grenade | 2 | Level 2 |
| Flashbang | Grenade | 2 | Level 3 |
| Molotov | Grenade | 1 | Level 3 |
| C4 Explosive | Charge | 2 | Level 4 |
| Remote Mine | Charge | 1 | Level 5 |

#### 5.1.9 Melee Weapons
| Weapon | Damage | Speed | Unlock Level |
|--------|--------|-------|--------------|
| Combat Knife | High | Fast | Start |
| Machete | Very High | Medium | Level 2 |
| Crowbar | High | Medium | Start |
| Entrenching Tool | High | Medium | Level 1 |
| Baseball Bat | Medium | Fast | Level 3 |
| Fire Axe | Very High | Slow | Level 4 |

### 5.2 Weapon Mechanics

#### 5.2.1 Ballistics System
- **Bullet Physics:**
  - Realistic bullet drop over distance
  - Wind resistance simulation
  - Ricochet system (angle-dependent)
  - Penetration through thin materials
  - Material-specific effects (wood, metal, concrete)

- **Damage Model:**
  ```
  Damage = BaseDamage * DistanceModifier * ArmorModifier * MaterialModifier
  ```
  
- **Hit Registration:**
  - Server-authoritative (even in single-player for consistency)
  - Hit scan for most weapons
  - Projectile-based for snipers and heavy weapons

#### 5.2.2 Recoil & Spread
- **Recoil Patterns:**
  - First-shot accuracy bonus
  - Vertical and horizontal recoil
  - Pattern-based (not random)
  - Recoil increases with sustained fire
  
- **Spread:**
  - Hip-fire spread (large)
  - ADS spread (tight)
  - Movement spread (increases while moving)
  - Jumping spread (very large)

#### 5.2.3 Reload System
- **Reload Types:**
  - Tactical reload (retains partial magazine)
  - Full reload (replaces entire magazine)
  - Emergency reload (faster, from reserve)
  
- **Reload Animations:**
  - Weapon-specific animations
  - Interruptible reloads
  - Magazines can be dropped and picked up

#### 5.2.4 Weapon Degradation
- **Overheating:**
  - Sustained fire causes barrel to heat up
  - Accuracy degrades with overheating
  - Visual heat shimmer effect
  - Cool-down period required

- **Jamming:**
  - Rare chance of jam with poor maintenance
  - More likely with suppressed weapons
  - Quick melee attack to clear jam

### 5.3 Equipment & Gear

#### 5.3.1 Armor
- **Light Armor:** +20% damage resistance, +10% movement speed
- **Medium Armor:** +40% damage resistance, normal movement
- **Heavy Armor:** +60% damage resistance, -10% movement speed
- **Tactical Vest:** +15% damage resistance, +extra ammo capacity

#### 5.3.2 Gadgets
- **Night Vision:** Green tint, enhances low-light visibility
- **Thermal Vision:** Heat signature detection
- **Ballistic Shield:** Portable cover, can be shot through
- **Medkit:** Instant full heal (limited use)
- **Adrenaline Shot:** Temporary speed/damage boost

#### 5.3.3 Backpack System
- **Ammo Capacity:** Increases max ammo carry
- **Grenade Capacity:** Extra throwable slots
- **Health Packs:** Additional healing items
- **Scavenger:** Auto-pickup ammo from dead enemies

---

## 6. LEVEL DESIGN

### 6.1 Level Overview
8-10 missions, each with distinct themes and gameplay focus. All levels feature:
- Multiple paths (2-3 main routes)
- Destructible environments
- Hidden collectibles (intel, weapons)
- Dynamic enemy spawns based on destruction
- Interactive objects and hazards

### 6.2 Mission List

#### Mission 1: "Black Dawn"
- **Location:** Rural village outskirts
- **Objective:** Extract defector, escape ambush
- **Key Features:**
  - Tutorial for basic mechanics
  - First major destruction sequence (farm buildings)
  - Introduction to weapon switching
  - Escape via stolen vehicle
- **Length:** 30-45 minutes
- **Boss/Set-piece:** Helicopter escape sequence

#### Mission 2: "Scorched Earth"
- **Location:** Abandoned industrial complex
- **Objective:** Locate missing team members
- **Key Features:**
  - Large open areas with destructible factories
  - First heavy weapon (RPG) available
  - Multiple sniper positions
  - Environmental hazards (gas leaks, explosions)
- **Length:** 45-60 minutes
- **Boss/Set-piece:** Fighting through collapsing factory

#### Mission 3: "Bridge of Sighs"
- **Location:** Massive suspension bridge
- **Objective:** Cross bridge under enemy fire
- **Key Features:**
  - Iconic destruction sequence (bridge collapse)
  - Vehicle combat (technicals with mounted guns)
  - Limited cover options
  - Weather effects (rain, wind)
- **Length:** 25-35 minutes
- **Boss/Set-piece:** Bridge destruction cutscene

#### Mission 4: "Ghost Town"
- **Location:** Abandoned city district
- **Objective:** Find and eliminate target
- **Key Features:**
  - Urban combat with destructible buildings
  - Civilians to protect (optional)
  - Stealth sections possible
  - Multiple entry points to buildings
- **Length:** 40-50 minutes
- **Boss/Set-piece:** Building collapse sequence

#### Mission 5: "Factory of Death"
- **Location:** Military ammunition factory
- **Objective:** Sabotage production, escape
- **Key Features:**
  - Massive explosions chain reactions
  - Conveyor belt hazards
  - Limited visibility (smoke, steam)
  - Time limit for escape
- **Length:** 35-45 minutes
- **Boss/Set-piece:** Factory explosion finale

#### Mission 6: "Tunnel Vision"
- **Location:** Underground tunnel system
- **Objective:** Infiltrate enemy base
- **Key Features:**
  - Claustrophobic combat
  - Limited destruction (reinforced tunnels)
  - Flashlight required
  - Ambush points
- **Length:** 30-40 minutes
- **Boss/Set-piece:** Tunnel collapse sequence

#### Mission 7: "Mountain Fortress"
- **Location:** High-altitude military base
- **Objective:** Assault and capture base
- **Key Features:**
  - Vehicle combat (jeeps, APCs)
  - Large open areas
  - Elevation changes
  - Snow/weather effects
- **Length:** 50-65 minutes
- **Boss/Set-piece:** APC battle

#### Mission 8: "Heart of Darkness"
- **Location:** Underground bunker complex
- **Objective:** Eliminate main antagonist
- **Key Features:**
  - Final boss fight
  - Multiple phases
  - Environmental storytelling
  - All weapons available
- **Length:** 45-60 minutes
- **Boss/Set-piece:** Final confrontation with Colonel Orlov

#### Mission 9: "The Reckoning" (Secret Mission)
- **Location:** Interrogation room (flashback)
- **Objective:** Unlockable based on collectibles
- **Key Features:**
  - Reveals true story
  - Alternative ending
  - All weapons/equipment available
- **Length:** 20-30 minutes
- **Boss/Set-piece:** Final truth revelation

### 6.3 Level Design Principles

#### 6.3.1 Destruction Guidelines
- **Destructibility Tiers:**
  - **Tier 1 (Full):** Wood, glass, plaster, furniture
  - **Tier 2 (Partial):** Concrete walls, metal structures (limited)
  - **Tier 3 (Minimal):** Reinforced concrete, steel beams (surface damage only)
  - **Tier 4 (Indestructible):** Plot-critical structures, load-bearing elements

- **Destruction Triggers:**
  - Direct weapon fire
  - Explosives (grenades, rockets)
  - Vehicle impacts
  - Chain reactions (gas leaks, ammunition)

#### 6.3.2 Pacing & Flow
- **Combat Intensity Curve:**
  ```
  Low -> Medium -> High -> Breather -> Medium -> High -> Climax
  ```
  
- **Encounter Design:**
  - Minimum 3 enemies per fight
  - Maximum 15-20 enemies in large set-pieces
  - Mix of enemy types in each encounter
  - Reinforcements arrive based on noise/destruction

#### 6.3.3 Environmental Storytelling
- **Narrative Elements:**
  - Blood trails
  - Dead bodies with context
  - Destroyed vehicles telling stories
  - Graffiti and propaganda
  - Radio chatter and ambient sounds

#### 6.3.4 Collectibles
- **Intel:**
  - Documents (emails, orders, maps)
  - Audio logs (radio transmissions)
  - Photos (recon images)
  - Total: 50-75 per playthrough
- **Weapons:**
  - Hidden weapon caches
  - Unique weapons (golden guns)
  - Prototype weapons
- **Upgrades:**
  - Ammo types
  - Weapon attachments
  - Armor pieces

### 6.4 Level Assets Requirements

#### 6.4.1 Common Assets
- **Props:**
  - Crates, barrels, sandbags
  - Furniture (tables, chairs, beds)
  - Vehicles (jeeps, trucks, APCs)
  - Debris (rubble, broken wood, metal)
  - Electronics (computers, radios, screens)

- **Structures:**
  - Wooden buildings (full destruction)
  - Concrete buildings (partial destruction)
  - Metal structures (limited destruction)
  - Bridges, towers, bunkers

#### 6.4.2 Level-Specific Assets
Each level has unique assets:
- **Mission 1:** Farm buildings, crops, livestock
- **Mission 2:** Factory machinery, conveyor belts, storage tanks
- **Mission 3:** Bridge sections, support cables, toll booths
- **Mission 4:** Urban buildings, street furniture, vehicles
- **Mission 5:** Industrial equipment, pipelines, control panels
- **Mission 6:** Tunnel supports, lighting, ventilation
- **Mission 7:** Military structures, snow-covered assets, vehicles
- **Mission 8:** Bunker equipment, servers, security systems

---

## 7. ENEMIES & AI

### 7.1 Enemy Types

#### 7.1.1 Basic Infantry
| Type | Health | Armor | Weapons | Behavior |
|------|--------|-------|---------|----------|
| Grunt | 100 | Light | AK-47, MP5 | Basic cover usage |
| Soldier | 125 | Medium | M4, G36 | Flanking attempts |
| Veteran | 150 | Medium | SCAR-H, AK-74 | Suppression fire |
| Sniper | 75 | Light | SVD, M40 | Long-range, relocates |
| Shotgunner | 200 | Heavy | Saiga-12, SPAS-12 | Close-range, aggressive |
| Machine Gunner | 250 | Heavy | M249, PKM | Suppression, stationary |

#### 7.1.2 Special Enemies
| Type | Health | Armor | Weapons | Behavior |
|------|--------|-------|---------|----------|
| Grenadier | 125 | Medium | AK-47 + Grenades | Throws grenades frequently |
| Engineer | 100 | Light | SMG + Repair Tool | Repairs structures, sets traps |
| Heavy | 300 | Heavy | Minigun | Slow, high damage |
| RPG Trooper | 150 | Medium | RPG-7 | Fires rockets, stays at distance |
| Shield Bearer | 200 | Heavy | Pistol + Shield | Blocks front, slow |
| Commander | 200 | Medium | M4 + Grenades | Directs others, calls reinforcements |

#### 7.1.3 Elite Enemies
| Type | Health | Armor | Weapons | Behavior |
|------|--------|-------|---------|----------|
| Spetsnaz | 175 | Heavy | Custom Loadout | Highly aggressive, tactical |
| Black Ops | 200 | Heavy | All Weapons | Uses all available cover |
| Boss (Orlov) | 500 | Heavy | Custom | Multiple phases, uses environment |

#### 7.1.4 Vehicles
| Type | Health | Armor | Weapons | Behavior |
|------|--------|-------|---------|----------|
| Jeep | 300 | Light | Mounted MG | Patrols, chases player |
| Truck | 500 | Medium | None | Transport, can be hijacked |
| APC | 1000 | Heavy | Cannon + MG | Heavy firepower, slow |
| Helicopter | 800 | Medium | Minigun + Rockets | Aerial attacks |
| Tank | 2000 | Heavy | Main Cannon + MG | Slow, devastating |

### 7.2 AI Behavior System

#### 7.2.1 Perception
- **Sight:** 180-degree FOV, distance-based
- **Hearing:** Gunshots, explosions, footsteps
- **Memory:** Remembers last known position
- **Communication:** Shares info with nearby allies

#### 7.2.2 Combat States
```
IDLE -> ALERT -> SEARCH -> ENGAGE -> SUPPRESS -> RETREAT -> REGROUP
```

- **Idle:** Patrols, performs routine actions
- **Alert:** Noticed something suspicious, investigates
- **Search:** Actively looking for player
- **Engage:** Firing at player
- **Suppress:** Laying down covering fire
- **Retreat:** Falling back to cover
- **Regroup:** Joining with other enemies

#### 7.2.3 Tactical Decisions
- **Cover Usage:**
  - Takes cover behind destructible objects
  - Moves between cover points
  - Leans out to fire
  - Blind fires when appropriate

- **Flanking:**
  - Attempts to get behind player
  - Uses alternate paths
  - Coordinates with team

- **Grenade Usage:**
  - Throws when player is in cover
  - Uses smoke for advancement
  - Flashbangs to disorient

- **Special Abilities:**
  - Engineers repair structures
  - Commanders call reinforcements
  - Medics revive downed allies

#### 7.2.4 Difficulty Scaling
- **Recruit:**
  - Slower reaction times
  - Less accurate
  - Fewer enemies
  - More health for player

- **Veteran:**
  - Normal reaction times
  - Standard accuracy
  - Balanced enemy counts
  - Normal health

- **Black Ops:**
  - Faster reaction times
  - More accurate
  - More enemies
  - Less health for player

- **Hardcore:**
  - Instant death (one-shot kills)
  - No HUD
  - No checkpoints
  - Permadeath

### 7.3 Enemy Spawn System

#### 7.3.1 Spawn Logic
- **Trigger-Based:** Enemies spawn when player enters area
- **Reinforcement:** Additional enemies based on noise/destruction
- **Patrol:** Enemies move between patrol points
- **Ambush:** Hidden enemies that attack when player passes

#### 7.3.2 Spawn Points
- **Static:** Fixed locations
- **Dynamic:** Based on destruction (new paths opened)
- **Vehicle:** Spawns with vehicles
- **Building:** Inside structures, exits when destroyed

#### 7.3.3 Spawn Limits
- **Maximum Active:** 20-30 enemies at once
- **Spawn Cooldown:** 5-10 seconds between waves
- **Total Cap:** Level-specific limits to prevent overload

---

## 8. DESTRUCTION SYSTEM

### 8.1 Destruction Philosophy
"If the player can see it, they can destroy it."

### 8.2 Technical Implementation

#### 8.2.1 Destruction Tiers

**Tier 1 - Voxel-Based Destruction**
- **Target Objects:** Wood, plaster, drywall, glass
- **Implementation:** Voxel grid (4-8cm resolution)
- **Performance:** GPU-accelerated via UE5 Nanite
- **Features:**
  - Per-voxel damage
  - Realistic fracture patterns
  - Debris generation
  - Dust/particle effects

**Tier 2 - Fracture-Based Destruction**
- **Target Objects:** Concrete, brick, metal panels
- **Implementation:** Pre-fractured meshes with Chaos Physics
- **Performance:** CPU/GPU hybrid
- **Features:**
  - Large chunk break-off
  - Structural integrity simulation
  - Support for load-bearing elements
  - Chain reactions

**Tier 3 - Surface Damage**
- **Target Objects:** Reinforced concrete, steel, thick metal
- **Implementation:** Decal-based with normal map changes
- **Performance:** Very lightweight
- **Features:**
  - Bullet holes
  - Scorch marks
  - Cracks and spalling
  - Material transitions

**Tier 4 - Indestructible**
- **Target Objects:** Plot-critical elements, world boundaries
- **Implementation:** Standard collision
- **Performance:** N/A
- **Features:**
  - Visual feedback (sparks, ricochets)
  - Audio feedback
  - No physical changes

#### 8.2.2 Destruction Mechanics

**Damage Propagation:**
- **Direct Damage:** From bullets, explosions
- **Indirect Damage:** From falling debris, chain reactions
- **Structural Damage:** Loss of support causes collapse
- **Fire Damage:** Spreads to nearby flammable objects

**Debris System:**
- **Debris Types:**
  - Small (particles)
  - Medium (physics objects)
  - Large (structural chunks)
  
- **Debris Behavior:**
  - Physics-based movement
  - Can cause additional damage
  - Can be used as cover
  - Can block paths

**Chain Reactions:**
- **Explosive Barrels:** Red barrels explode when shot
- **Gas Leaks:** Can be ignited by gunfire
- **Electrical Systems:** Sparks can cause fires
- **Ammunition:** Explodes when exposed to fire
- **Fuel Tanks:** Massive explosions when damaged

#### 8.2.3 Performance Optimization

**LOD System:**
- **LOD 0 (Close):** Full destruction physics
- **LOD 1 (Medium):** Simplified destruction
- **LOD 2 (Far):** Decal-only damage
- **LOD 3 (Very Far):** No destruction

**Culling:**
- **Distance Culling:** Objects beyond X meters use simplified destruction
- **Occlusion Culling:** Objects not visible skip destruction calculations
- **Priority System:** Important objects get higher destruction priority

**Batching:**
- **Instanced Destruction:** Similar objects share destruction data
- **GPU Acceleration:** Uses UE5's Chaos Physics GPU support
- **Async Processing:** Destruction calculations on background threads

### 8.3 Destruction Visuals

#### 8.3.1 Particle Effects
- **Wood:** Splinters, sawdust
- **Concrete:** Dust, small chunks
- **Metal:** Sparks, shards
- **Glass:** Shards, glitter effect
- **Fabric:** Fibers, dust

#### 8.3.2 Audio Effects
- **Wood:** Cracking, splintering
- **Concrete:** Crumbling, dust settling
- **Metal:** Clanging, screeching
- **Glass:** Shattering, tinkling
- **Explosions:** Deep booms, debris impacts

#### 8.3.3 Camera Effects
- **Screen Shake:** Intensity based on destruction size
- **Slow Motion:** For large explosions (optional)
- **Debris on Screen:** Temporary overlay
- **Focus Shift:** Camera adjustment for large destruction

### 8.4 Destruction Gameplay Integration

#### 8.4.1 Destruction as Gameplay
- **New Paths:** Destroy walls to create shortcuts
- **Cover Creation:** Destroy objects to create cover
- **Traps:** Cause chain reactions to damage enemies
- **Distractions:** Create noise to attract enemies
- **Environmental Kills:** Drop debris on enemies

#### 8.4.2 Destruction Feedback
- **Score System:** Points for destruction (optional)
- **Achievements:** Destruction-related challenges
- **Visual Feedback:** Destruction counter, damage indicators
- **Audio Feedback:** Announcer comments on massive destruction

#### 8.4.3 Destruction Limits
- **Performance:** System scales based on hardware
- **Gameplay:** Some objects must remain for progression
- **Memory:** Debris cleanup after X seconds
- **Network:** N/A (single-player only)

---

## 9. GRAPHICS & AUDIO

### 9.1 Visual Style

#### 9.1.1 Art Direction
- **Theme:** Gritty, realistic, war-torn
- **Color Palette:**
  - Primary: Dark greys, browns, blacks
  - Secondary: Rust reds, faded blues
  - Accent: Bright oranges (fire, explosions)
  
- **Lighting:**
  - **Direction:** Natural, directional
  - **Color:** Cool blues (shadows), warm oranges (highlights)
  - **Intensity:** Low-key, high contrast
  - **Style:** Film noir influences

#### 9.1.2 Materials & Textures
- **Resolution:** 4K textures for key assets, 2K for secondary
- **PBR:** Full Physically-Based Rendering
- **Detail:** High-resolution normal maps, ambient occlusion
- **Weathering:** All surfaces show wear and tear
- **Dirt:** Procedural dirt accumulation based on gameplay

#### 9.1.3 Effects
- **Particle Systems:**
  - Bullet impacts (material-specific)
  - Explosions (multi-layered)
  - Fire and smoke
  - Dust and debris
  - Blood and gore

- **Post-Processing:**
  - Film grain
  - Color grading (cool tones)
  - Depth of field
  - Motion blur
  - Screen-space reflections
  - Ambient occlusion

### 9.2 Rendering Features

#### 9.2.1 Core Features
- **Resolution:** 4K native, 8K downscaled
- **Frame Rate:** 120FPS target, 60FPS minimum
- **Anti-Aliasing:** TAA + DLSS/FSR
- **Anisotropic Filtering:** 16x
- **Texture Filtering:** Anisotropic

#### 9.2.2 Advanced Features
- **Ray Tracing:**
  - Ray Traced Shadows (movable lights)
  - Ray Traced Global Illumination
  - Ray Traced Reflections
  - Ray Traced Ambient Occlusion
  - Hybrid approach (RT + rasterized fallback)

- **Nanite:**
  - Virtualized geometry for high-poly assets
  - Millions of polygons on-screen
  - No LOD popping

- **Lumen:**
  - Dynamic global illumination
  - Realistic lighting interactions
  - No lightmap baking required

- **Niagara:**
  - Advanced particle systems
  - GPU-accelerated effects
  - Complex fluid simulations

#### 9.2.3 Platform-Specific Features
- **DLSS 3.5:** Frame generation + ray reconstruction (NVIDIA)
- **FSR 3:** Frame generation + upscaling (AMD)
- **XeSS:** Intel upscaling
- **Ultrawide Support:** 21:9, 32:9 aspect ratios
- **Multi-Monitor:** 4K surround support

### 9.3 Audio Design

#### 9.3.1 Audio Philosophy
"Every sound should feel real, heavy, and impactful."

#### 9.3.2 Sound Categories

**Weapons:**
- **Pistols:** Sharp, metallic cracks
- **Shotguns:** Deep, resonant booms
- **Assault Rifles:** Rapid, punchy pops
- **Snipers:** Loud, echoing cracks
- **Heavy Weapons:** Deep, rumbling roars
- **Explosions:** Massive, bass-heavy booms

**Environment:**
- **Destruction:** Material-specific sounds
- **Ambient:** Wind, distant gunfire, animal noises
- **Weather:** Rain, thunder, snow
- **Interior:** Echo, reverb based on room size

**Character:**
- **Player:** Breathing, footsteps, voice
- **Enemies:** Shouts, pain sounds, radio chatter
- **NPCs:** Dialogue, reactions

**Music:**
- **Style:** Orchestral with electronic elements
- **Intensity:** Dynamic based on action
- **Layers:** Multiple stems that mix based on gameplay
- **Themes:** Unique tracks for each mission

#### 9.3.3 Audio Implementation

**3D Audio:**
- **HRTF:** Head-Related Transfer Function for accurate spatial audio
- **Occlusion:** Sounds muffled through walls
- **Reverb:** Room-specific reverb tails
- **Doppler:** Pitch shift for moving sound sources

**Audio Engine:**
- **UE5 MetaSounds:** Dynamic audio generation
- **Real-time Processing:** Effects applied based on game state
- **Voice Codec:** Opus for high-quality compression
- **Memory:** Streaming for large audio files

**Sound Design Tools:**
- **Layering:** Multiple samples per sound
- **Randomization:** Slight variations to prevent repetition
- **Pitch Modulation:** Dynamic pitch changes
- **Volume Automation:** Dynamic volume based on distance/occlusion

### 9.4 Audio Assets List

#### 9.4.1 Weapon Sounds
- **Pistols:** 10-15 samples per weapon
- **Shotguns:** 8-12 samples per weapon
- **Rifles:** 12-18 samples per weapon
- **Machine Guns:** 15-20 samples per weapon
- **Snipers:** 10-15 samples per weapon
- **Heavy Weapons:** 12-18 samples per weapon
- **Reloads:** 3-5 samples per weapon
- **Melee:** 5-8 samples per weapon

#### 9.4.2 Environment Sounds
- **Destruction:** 50+ samples (wood, concrete, metal, glass)
- **Impacts:** 20+ samples (bullet, melee, explosive)
- **Ambient:** 30+ loops (wind, rain, fire, etc.)
- **Interior:** 10+ reverb impulses

#### 9.4.3 Character Sounds
- **Player:** 20+ voice lines, 10+ pain sounds, footsteps
- **Enemies:** 50+ voice lines (Russian), 15+ pain sounds per type
- **NPCs:** 100+ dialogue lines

#### 9.4.4 Music
- **Main Theme:** 1 track (3-5 minutes)
- **Mission Themes:** 8-10 tracks (2-4 minutes each)
- **Combat Music:** 5-8 action tracks
- **Ambient Music:** 5-8 atmospheric tracks
- **Stings:** 20+ short musical cues

---

## 10. TECHNICAL SPECIFICATIONS

### 10.1 Engine & Tools
- **Engine:** Unreal Engine 5.4+
- **Language:** C++ (core systems), Blueprints (gameplay)
- **IDE:** Visual Studio 2022, Rider
- **Version Control:** Git (GitHub)
- **Build System:** Unreal Build Tool, CMake

### 10.2 Project Structure
```
BlackRemastered/
├── Config/                    # Configuration files
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   ├── DefaultInput.ini
│   └── ...
├── Content/                   # Game assets
│   ├── Characters/            # Character meshes, animations
│   ├── Weapons/               # Weapon meshes, sounds
│   ├── Environments/          # Level assets, props
│   ├── Effects/               # VFX, particles
│   ├── Sounds/                # Audio assets
│   ├── UI/                    # UI elements
│   ├── Materials/             # Materials, textures
│   ├── Blueprints/            # Blueprint classes
│   └── Levels/                # Level files
├── Source/                    # C++ source code
│   └── BlackRemastered/        # Game module
│       ├── Private/           # Private implementation
│       ├── Public/            # Public headers
│       └── Classes/            # Class definitions
├── Plugins/                   # Third-party plugins
├── Docs/                      # Documentation
└── Build/                     # Build outputs
```

### 10.3 System Requirements

#### 10.3.1 Minimum Requirements
- **OS:** Windows 10 64-bit
- **CPU:** Intel i5-4460 / AMD Ryzen 3 1200
- **RAM:** 16 GB
- **GPU:** NVIDIA GTX 970 / AMD RX 480 (4GB VRAM)
- **Storage:** 50 GB SSD
- **DirectX:** Version 12

#### 10.3.2 Recommended Requirements
- **OS:** Windows 11 64-bit
- **CPU:** Intel i7-8700K / AMD Ryzen 7 3700X
- **RAM:** 32 GB
- **GPU:** NVIDIA RTX 3070 / AMD RX 6800 XT (8GB VRAM)
- **Storage:** 50 GB NVMe SSD
- **DirectX:** Version 12 Ultimate

#### 10.3.3 Ultra Requirements
- **OS:** Windows 11 64-bit
- **CPU:** Intel i9-13900K / AMD Ryzen 9 7950X
- **RAM:** 64 GB
- **GPU:** NVIDIA RTX 4090 / AMD RX 7900 XTX (24GB VRAM)
- **Storage:** 50 GB NVMe SSD
- **DirectX:** Version 12 Ultimate

### 10.4 Performance Targets
- **Resolution:** 1080p-4K
- **Frame Rate:** 60-120 FPS
- **Ray Tracing:** On at 1440p+ with DLSS/FSR
- **Load Times:** < 5 seconds per level
- **Memory Usage:** < 12GB VRAM at 4K

### 10.5 Build Configurations
- **Development:** Full debugging, all features enabled
- **Testing:** Optimized, some features disabled
- **Shipping:** Fully optimized, minimal debugging
- **Debug:** Maximum debugging, minimal optimization

---

## 11. UI/UX DESIGN

### 11.1 UI Philosophy
"Minimal, functional, immersive. Never take the player out of the experience."

### 11.2 HUD Elements

#### 11.2.1 Persistent HUD
- **Health Bar:** Bottom-left, segmented
- **Armor Bar:** Below health, segmented
- **Ammo Counter:** Bottom-center, "X/Y" format
- **Weapon Icon:** Bottom-center, current weapon
- **Mini-Map:** Top-right (optional, can be disabled)
- **Objective Marker:** Top-center, dynamic
- **Crosshair:** Center, dynamic (changes based on weapon)

#### 11.2.2 Contextual HUD
- **Interact Prompt:** Appears near interactable objects
- **Damage Indicator:** Screen edge glow when hit
- **Low Health Warning:** Red screen tint + heartbeat sound
- **Reload Progress:** Ammo counter animation
- **Grenade Indicator:** When primed grenade is held
- **Vehicle HUD:** Appears when in vehicle

#### 11.2.3 Weapon HUD
- **Crosshair Styles:**
  - Default: Simple dot
  - Pistol: Small circle
  - Shotgun: Spread indicator
  - Sniper: Precision crosshair
  - Heavy: Large, bold

- **Ammo Types:**
  - Current ammo type displayed
  - Color-coded (red=explosive, blue=AP, etc.)

- **Weapon Status:**
  - Overheating indicator
  - Jam indicator
  - Suppressor attached indicator

#### 11.2.4 Enemy HUD
- **Health Bars:** Above enemies when damaged (optional)
- **Hit Markers:** Small cross on hit
- **Headshot Indicator:** Special animation
- **Kill Confirm:** Name/type of killed enemy

### 11.3 Menu System

#### 11.3.1 Main Menu
- **Background:** Animated, shows game footage
- **Options:**
  - Campaign
  - Load Game
  - Options
  - Extras
  - Quit

#### 11.3.2 Campaign Menu
- **Mission Select:** Grid of missions
- **Difficulty:** Selectable before starting
- **New Game+:** Unlocked after completion

#### 11.3.3 Options Menu
- **Graphics:**
  - Resolution, refresh rate
  - Quality presets (Low, Medium, High, Ultra, Custom)
  - Individual settings (shadows, textures, effects, etc.)
  - Ray tracing toggle
  - DLSS/FSR settings
  - Motion blur toggle
  - FOV slider (70-120)
  - Ultrawide support toggle

- **Audio:**
  - Master volume
  - Music volume
  - SFX volume
  - Voice volume
  - Subtitles (on/off, size, color)
  - 3D audio toggle
  - Audio device selection

- **Gameplay:**
  - Difficulty
  - Controls (keyboard/mouse, controller)
  - Mouse sensitivity (X/Y, separate sliders)
  - Invert Y-axis
  - Aim assist (controller only)
  - Crosshair style
  - HUD elements toggle
  - Subtitles
  - Language (English, Polish)

- **Accessibility:**
  - Colorblind mode
  - Screen shake intensity
  - Motion sickness reduction
  - Text size
  - High contrast mode
  - Closed captions

#### 11.3.4 Pause Menu
- **Resume:** Returns to game
- **Save Game:** Manual save
- **Load Game:** Access save files
- **Options:** Quick access to key settings
- **Restart Mission:** Restart current mission
- **Main Menu:** Return to main menu
- **Quit Game:** Exit to desktop

#### 11.3.5 In-Game Menus
- **Inventory:** Weapon/equipment selection
- **Map:** Full-screen map (if available)
- **Objective Log:** Current and past objectives
- **Collectibles:** Found intel/weapons

### 11.4 UI Visual Design

#### 11.4.1 Color Scheme
- **Primary:** Dark grey (#1A1A1A)
- **Secondary:** Light grey (#E0E0E0)
- **Accent:** Orange (#FF6B35)
- **Warning:** Red (#FF3333)
- **Success:** Green (#33FF33)

#### 11.4.2 Typography
- **Primary Font:** Roboto Condensed (or similar military font)
- **Secondary Font:** Standard UE5 font
- **Size:** Scalable based on resolution
- **Style:** Bold, clean, readable

#### 11.4.3 Animations
- **Transitions:** Smooth fades, slides
- **Feedback:** Button presses have visual/audio feedback
- **Hover:** Subtle glow/color change
- **Selection:** Clear highlighting

### 11.5 Controller UI
- **Button Prompts:** Platform-specific icons
- **Radial Menus:** For weapon/equipment selection
- **Contextual Buttons:** Appear when near interactables
- **Vibration:** Controller feedback for important events

---

## 12. PROGRESSION SYSTEM

### 12.1 Experience & Leveling

#### 12.1.1 XP Sources
- **Kills:** Base XP per kill, bonus for headshots
- **Destruction:** XP based on damage dealt to environment
- **Objectives:** XP for completing objectives
- **Challenges:** Bonus XP for special achievements
- **Collectibles:** XP for finding intel/weapons

#### 12.1.2 XP Formula
```
Total XP = (Kills * KillXP) + (Destruction * DestructionXP) + 
           (Objectives * ObjectiveXP) + (Challenges * ChallengeXP) +
           (Collectibles * CollectibleXP)
```

#### 12.1.3 Leveling Curve
- **Levels:** 1-50
- **XP per Level:** Exponential curve (more XP needed for higher levels)
- **Formula:** `XPForLevel = BaseXP * (1.1 ^ (Level - 1))`
- **BaseXP:** 1000

### 12.2 Unlock System

#### 12.2.1 Weapon Unlocks
| Level | Unlock |
|-------|--------|
| 1 | All starting weapons |
| 2 | Makarov PM, Desert Eagle |
| 3 | Saiga-12, SPAS-12 |
| 4 | UMP, PP-19 Bizon |
| 5 | SCAR-H, G36 |
| 6 | M249 SAW, PKM |
| 7 | M40A5, SV-98 |
| 8 | Barrett M82, AWM |
| 9 | RPG-7, M134 Minigun |
| 10 | AT4 Anti-Tank |

#### 12.2.2 Attachment Unlocks
| Level | Unlock |
|-------|--------|
| 1 | Red Dot Sight |
| 2 | Suppressor |
| 3 | Extended Magazine |
| 4 | ACOG Scope |
| 5 | Foregrip |
| 6 | Sniper Scope |
| 7 | Bayonet |
| 8 | Underbarrel Grenade Launcher |
| 9 | Bipod |
| 10 | All attachments |

#### 12.2.3 Ammo Type Unlocks
| Level | Unlock |
|-------|--------|
| 1 | Standard Ammo |
| 2 | Armor-Piercing |
| 3 | Incendiary |
| 4 | Explosive |
| 5 | Hollow-Point |

#### 12.2.4 Gear Unlocks
| Level | Unlock |
|-------|--------|
| 1 | Light Armor |
| 2 | Tactical Vest |
| 3 | Night Vision |
| 4 | Medium Armor |
| 5 | Thermal Vision |
| 6 | Heavy Armor |
| 7 | Ballistic Shield |
| 8 | Adrenaline Shot |

### 12.3 Skill Tree (Optional)

#### 12.3.1 Combat Skills
- **Headshot Bonus:** +10% headshot damage
- **Critical Hit:** Chance for double damage
- **Fast Reload:** +15% reload speed
- **Ammo Scavenger:** +20% ammo from pickups
- **Explosive Expert:** +25% explosive damage

#### 12.3.2 Destruction Skills
- **Demolition Man:** +20% destruction XP
- **Chain Reaction:** Explosions cause larger chain reactions
- **Structural Weakness:** +15% damage to structures
- **Debris Master:** Debris causes more damage to enemies
- **Collateral Damage:** Kills from destruction count as headshots

#### 12.3.3 Survival Skills
- **Toughness:** +10% damage resistance
- **Regeneration:** +20% health regen rate
- **Armor Expert:** +15% armor effectiveness
- **Medic:** +25% healing from packs
- **Adrenaline Rush:** Adrenaline shot lasts longer

### 12.4 Challenges & Achievements

#### 12.4.1 Challenge Types
- **Kills:** Kill X enemies with Y weapon
- **Destruction:** Destroy X objects in a level
- **Accuracy:** Achieve X% accuracy with Y weapon
- **Speed:** Complete level in under X time
- **Stealth:** Complete level without alerting enemies
- **Combination:** Perform specific sequences (headshot + explosion, etc.)

#### 12.4.2 Achievement List
| Name | Description | XP Reward |
|------|-------------|-----------|
| First Blood | Kill first enemy | 100 |
| Demolition Man | Destroy 100 objects | 500 |
| Headshot | Get 10 headshots | 250 |
| Perfect Shot | Headshot from 100m+ | 500 |
| Boom! | Kill 3 enemies with one explosion | 750 |
| Chain Reaction | Cause 5+ chain explosions | 1000 |
| Silent Killer | Kill 10 enemies with suppressed weapon | 500 |
| Overkill | Deal 1000+ damage in one shot | 750 |
| Untouchable | Complete level without taking damage | 1000 |
| Speedrun | Complete level in under 10 minutes | 1500 |
| Ghost | Complete level without killing anyone | 2000 |
| Master of All | Unlock all weapons | 5000 |

### 12.5 New Game+ (NG+)
- **Unlock:** Complete campaign on any difficulty
- **Features:**
  - All weapons/attachments available from start
  - Enemies have more health
  - More enemies spawn
  - New enemy types
  - Harder difficulty variants
  - Unique NG+ challenges

---

## 13. CONTROLS

### 13.1 Keyboard & Mouse Defaults

#### 13.1.1 Movement
| Action | Key |
|--------|-----|
| Move Forward | W |
| Move Back | S |
| Move Left | A |
| Move Right | D |
| Sprint | Shift |
| Crouch | Ctrl |
| Jump | Space |
| Prone | Z |
| Slide | Ctrl (while moving) |
| Mantle | E (near ledge) |

#### 13.1.2 Combat
| Action | Key |
|--------|-----|
| Fire | Mouse Button 1 |
| Aim Down Sights | Mouse Button 2 |
| Reload | R |
| Weapon Switch | Mouse Wheel / 1-9 |
| Throw Grenade | G |
| Melee | Mouse Button 3 |
| Use | E |
| Interact | E |
| Lean Left | Q |
| Lean Right | E |
| Cover | Ctrl (near cover) |

#### 13.1.3 Weapons & Equipment
| Action | Key |
|--------|-----|
| Primary Weapon | 1 |
| Secondary Weapon | 2 |
| Melee Weapon | 3 |
| Grenade | 4 |
| Special Weapon | 5 |
| Next Weapon | Mouse Wheel Up |
| Previous Weapon | Mouse Wheel Down |
| Toggle Fire Mode | B |
| Attach/Detach | T |

#### 13.1.4 Menu & System
| Action | Key |
|--------|-----|
| Pause | Esc |
| Menu | Esc |
| Quick Save | F5 |
| Quick Load | F8 |
| Map | Tab |
| Inventory | I |
| Objective Log | J |
| Console | ~ |

### 13.2 Controller Defaults

#### 13.2.1 Movement
| Action | Button |
|--------|--------|
| Move | Left Stick |
| Look | Right Stick |
| Sprint | Left Stick Click |
| Crouch | Right Stick Click |
| Jump | A |
| Prone | B |
| Slide | Right Stick Click (while moving) |
| Mantle | A (near ledge) |

#### 13.2.2 Combat
| Action | Button |
|--------|--------|
| Fire | Right Trigger |
| Aim Down Sights | Left Trigger |
| Reload | X |
| Weapon Switch | Y / D-Pad Up/Down |
| Throw Grenade | Right Bumper |
| Melee | Left Bumper |
| Use | A |
| Interact | A |
| Lean Left | Left Bumper |
| Lean Right | Right Bumper |
| Cover | Left Stick Click (near cover) |

#### 13.2.3 Weapons & Equipment
| Action | Button |
|--------|--------|
| Primary Weapon | D-Pad Up |
| Secondary Weapon | D-Pad Down |
| Melee Weapon | D-Pad Left |
| Grenade | D-Pad Right |
| Toggle Fire Mode | Left Stick Click |

#### 13.2.4 Menu & System
| Action | Button |
|--------|--------|
| Pause | Start |
| Menu | Start |
| Quick Save | Select + Start |
| Quick Load | Select + Back |
| Map | Back |
| Inventory | Select |

### 13.3 Control Customization
- **Key Binding:** All actions can be rebound
- **Controller Layout:** Multiple presets + custom
- **Sensitivity:** Separate X/Y sensitivity sliders
- **Inversion:** Toggle for Y-axis
- **Deadzone:** Adjustable for sticks/triggers
- **Vibration:** Toggle + intensity slider

---

## 14. ACCESSIBILITY

### 14.1 Visual Accessibility
- **Colorblind Modes:**
  - Deuteranopia (red-green)
  - Protanopia (red-green)
  - Tritanopia (blue-yellow)
  - Custom color adjustments

- **High Contrast Mode:** Enhanced outlines, brighter UI
- **Screen Reader:** Text-to-speech for menus
- **Subtitles:**
  - Size adjustment (small, medium, large)
  - Color customization
  - Background opacity
  - Speaker labels
  - Sound descriptions

- **UI Scaling:** Adjustable UI size (50%-200%)
- **Crosshair Customization:** Color, size, style
- **Motion Blur:** Toggle + intensity slider
- **Screen Shake:** Toggle + intensity slider

### 14.2 Audio Accessibility
- **Volume Controls:** Individual sliders for all audio categories
- **Visual Audio Cues:** Subtitles for all sounds
- **Audio Descriptions:** Narrated descriptions of key visual events
- **Mono Audio:** Option for single-channel audio
- **Frequency Adjustment:** EQ sliders for different frequencies

### 14.3 Motor Accessibility
- **Button Remapping:** Full control customization
- **Sticky Keys:** Toggle for modifier keys
- **Hold Duration:** Adjustable for held actions
- **Double-Press Speed:** Adjustable for double-press actions
- **Controller Deadzone:** Adjustable to reduce accidental inputs

### 14.4 Cognitive Accessibility
- **Difficulty Options:** Adjustable game difficulty
- **Tutorials:** Optional, can be skipped or repeated
- **Objective Markers:** Clear visual indicators
- **Waypoints:** Optional path markers
- **Text Speed:** Adjustable for dialogue
- **Auto-Pause:** Pause when menu is open

---

## 15. PLATFORM REQUIREMENTS

### 15.1 PC Requirements

#### 15.1.1 Hardware
- **CPU:** x86-64 compatible processor
- **RAM:** Minimum 16GB, recommended 32GB
- **GPU:** DirectX 12 compatible with 4GB+ VRAM
- **Storage:** 50GB free space on SSD
- **OS:** Windows 10/11 64-bit

#### 15.1.2 Software
- **DirectX:** Version 12 Ultimate
- **Visual C++ Redistributable:** 2015-2022
- **UE5 Prerequisites:** Automatically installed
- **Drivers:** Latest GPU drivers

#### 15.1.3 Input Devices
- **Keyboard:** Standard 104-key
- **Mouse:** 3-button with scroll wheel
- **Controller:** XInput compatible (Xbox, PlayStation via DS4Windows)

### 15.2 Peripheral Support
- **Controllers:**
  - Xbox Series X|S
  - PlayStation 5 (via DS4Windows/Steam Input)
  - Nintendo Switch Pro
  - Third-party controllers

- **VR:** Not supported (flat-screen only)
- **TrackIR:** Supported for head tracking
- **Steam Controller:** Supported
- **Foot Pedals:** Optional (for special actions)

### 15.3 Online Features
- **Single-Player Only:** No multiplayer, no online requirements
- **Cloud Saves:** Optional (via platform or custom solution)
- **Updates:** Automatic or manual
- **DLC:** None planned (complete game at launch)

---

## APPENDIX A: FILE FORMATS

### A.1 Asset File Formats
- **3D Models:** FBX (preferred), OBJ
- **Textures:** PNG, TGA, EXR, JPEG
- **Audio:** WAV (44.1kHz, 16-bit), MP3, OGG
- **Animations:** FBX with embedded animations
- **Materials:** UE5 Material Instances
- **Particles:** Niagara Systems
- **Levels:** UE5 World Files

### A.2 Source Code Formats
- **C++:** Standard C++17
- **Blueprints:** UE5 Blueprint Files
- **Shaders:** HLSL (for custom materials)
- **UI:** UMG Widget Blueprints

### A.3 Configuration Files
- **INI:** UE5 configuration files
- **JSON:** Data files (weapons, enemies, etc.)
- **CSV:** Spreadsheet data (level design, etc.)

---

## APPENDIX B: NAMING CONVENTIONS

### B.1 Asset Naming
- **Characters:** CH_CharacterName_Variant
- **Weapons:** WP_WeaponName_Variant
- **Props:** SM_PropName_Variant
- **Textures:** T_TextureName_Type_Size
- **Materials:** M_MaterialName_Type
- **Particles:** P_ParticleName_Type
- **Sounds:** A_SoundName_Type
- **Animations:** A_CharacterName_Action
- **Levels:** L_LevelName_Variant

### B.2 Code Naming
- **Classes:** PascalCase (ABlackRemasteredGameMode)
- **Variables:** CamelCase (m_Health, CurrentAmmo)
- **Functions:** PascalCase (FireWeapon, TakeDamage)
- **Enums:** PascalCase (EWeaponType, EEnemyState)
- **Constants:** UPPER_SNAKE_CASE (MAX_HEALTH, DEFAULT_AMMO)

### B.3 File Organization
- **Folders:** PascalCase (Characters, Weapons, Levels)
- **Files:** Same as asset/class name
- **Prefixes:** Type-specific (BP_ for Blueprints, WBP_ for Widgets)

---

## APPENDIX C: BUILD INSTRUCTIONS

### C.1 Setting Up Development Environment
1. Install Visual Studio 2022 with C++ support
2. Install Unreal Engine 5.4+ from Epic Games Launcher
3. Clone repository to local machine
4. Generate project files using UE5
5. Open solution in Visual Studio
6. Build project (Development configuration)

### C.2 Building the Game
1. Open project in Unreal Editor
2. Set target platform (Win64)
3. Select build configuration (Development/Shipping)
4. Package project for distribution
5. Test packaged build

### C.3 Deployment
1. Create installer using NSIS or similar
2. Package with required redistributables
3. Create desktop shortcut
4. Generate documentation
5. Upload to distribution platform

---

## APPENDIX D: MISSING ASSETS LIST

### D.1 Required Assets to Source

#### D.1.1 Character Assets
- [ ] Jack Kellar 3D model (high-poly, rigged)
- [ ] Enemy character models (5+ types)
- [ ] Civilian models (3+ types)
- [ ] Animation sets (idle, walk, run, shoot, etc.)
- [ ] Facial animations (for cutscenes)

#### D.1.2 Weapon Assets
- [ ] All weapon 3D models (25+ weapons)
- [ ] Weapon animations (reload, fire, etc.)
- [ ] Weapon sounds (fire, reload, etc.)
- [ ] Weapon textures (diffuse, normal, etc.)

#### D.1.3 Environment Assets
- [ ] Building models (wood, concrete, metal)
- [ ] Prop models (crates, barrels, furniture, etc.)
- [ ] Vehicle models (jeeps, trucks, APCs, etc.)
- [ ] Terrain assets (ground, rocks, vegetation)
- [ ] Destruction assets (fractured meshes, debris)

#### D.1.4 Effect Assets
- [ ] Particle effects (muzzle flash, explosions, etc.)
- [ ] Material effects (bullet holes, decals, etc.)
- [ ] Post-processing materials
- [ ] Shader networks

#### D.1.5 Audio Assets
- [ ] Weapon sounds (500+ samples)
- [ ] Environment sounds (100+ samples)
- [ ] Character voice lines (200+ lines)
- [ ] Music tracks (20+ tracks)
- [ ] Ambient loops (30+ loops)

#### D.1.6 UI Assets
- [ ] HUD elements (health bar, ammo counter, etc.)
- [ ] Menu backgrounds
- [ ] Icons (weapons, abilities, etc.)
- [ ] Fonts (custom military font)

---

## APPENDIX E: THIRD-PARTY TOOLS & PLUGINS

### E.1 Required Plugins
- **Chaos Physics:** Built-in UE5
- **Nanite:** Built-in UE5
- **Lumen:** Built-in UE5
- **Niagara:** Built-in UE5
- **MetaSounds:** Built-in UE5

### E.2 Recommended Plugins
- **VA Rest Plugin:** For voice acting integration
- **Advanced Sessions Plugin:** For save system
- **Common UI Plugin:** For UI elements
- **Gameplay Ability System:** For progression

### E.3 External Tools
- **Blender:** 3D modeling and animation
- **Substance Painter:** Texture creation
- **FMOD/Wwise:** Advanced audio implementation
- **Audacity:** Audio editing
- **GIMP/Photoshop:** Texture editing
- **Notepad++/VS Code:** Code editing

---

## APPENDIX F: TESTING CHECKLIST

### F.1 Core Systems
- [ ] Player movement (walk, run, crouch, jump, etc.)
- [ ] Weapon firing and reloading
- [ ] Damage and health system
- [ ] Enemy AI behavior
- [ ] Destruction system
- [ ] Save/load system

### F.2 Level Testing
- [ ] All levels playable from start to finish
- [ ] All objectives completable
- [ ] All collectibles obtainable
- [ ] All paths navigable
- [ ] All enemies defeatable

### F.3 Performance
- [ ] 60+ FPS on recommended hardware
- [ ] No crashes or freezes
- [ ] Memory usage within limits
- [ ] Load times acceptable

### F.4 Audio/Visual
- [ ] All sounds play correctly
- [ ] Music transitions smoothly
- [ ] All visual effects display correctly
- [ ] No graphical glitches

### F.5 UI/UX
- [ ] All menus functional
- [ ] All HUD elements visible
- [ ] Controls responsive
- [ ] Accessibility options work

---

## REVISION HISTORY

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2025-01-XX | AI System | Initial design document |

---

**END OF DOCUMENT**
