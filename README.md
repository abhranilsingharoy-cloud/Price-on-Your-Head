# Price on Your Head

![Unreal Engine 5.8](https://img.shields.io/badge/Engine-Unreal_Engine_5.8-blue?logo=unrealengine)
![Language](https://img.shields.io/badge/Language-Pure_C%2B%2B_20-00599C?logo=cplusplus)
![Platform](https://img.shields.io/badge/Platform-Windows_64--bit-0078D6?logo=windows)
![License](https://img.shields.io/badge/Assets-CC0_%2F_SIL_OFL-green)
![Contract](https://img.shields.io/badge/Parsewave-Approved_to_Build-orange)

> **A high-stakes 3D cyberpunk stealth heist and score attack game built for the Parsewave Unreal Engine contract in 100% pure C++.**

---

## 📖 Premise
You are an auction porter on a floating sale barge in the flooded financial district, and tonight's **Lot One** is a bankrupt billionaire's frozen head in a cryo-canister. You mean to steal it.

Before entering, inspect the holographic deck plan to spend insider tips and select your gadget loadout. In third person, cyan holograms and crimson bid-lights wash over the rain-slicked deck, frosted pedestals, and masked bidders. Crouch between velvet plinths, duck under pulsing laser tripwires, and evade patrolling camera drones whose searchlights expand the richer your haul becomes. Trigger bidding wars to flash emergency strobes and blind security drones, grab Lot One, and sprint across the rain-black deck to the extraction skiff before the final gavel drops.

---

## 🎯 Contract Specifications (Parsewave Pool)
* **Idea ID:** `idea-7F8AB478AD20` (Database ID: `1986959601`)
* **Title:** Price on Your Head
* **Genre:** Stealth Heist · Score Attack
* **Gameplay Family:** Stealth & Infiltration
* **Perspective:** Third-Person 3D
* **Target Run Length:** ~90 seconds (+ deck planning phase)
* **Difficulty:** Demanding
* **Performance Budget:** 1080p @ 60 FPS, Screen-Space Reflections, Movable Directional Sun, Sky Atmosphere, 0% Lumen/Nanite overhead, <15% near-black pixel density.

---

## 🕹️ Controls & Keybindings

| Key / Input | Action | In-Game Effect |
|---|---|---|
| **W, A, S, D** | Movement | Move across the barge deck at 450 cm/s. |
| **Mouse Aim** | Camera | Smooth third-person orbital camera with wall-collision prevention. |
| **Left Control** | Crouch | Slows to 200 cm/s, ducks under laser tripwires, reduces drone detection by 45%. |
| **E** | Grab / Interact | Steals Lot One ($1,000,000) or bonus lots ($250,000–$400,000) from plinths. |
| **Q** | Use Tool | Deploys active gadget (Fake Lot Tag, Decoy Paddle, Smoke Cartridge). |
| **1 / 2** | Select Tool | Toggles active gadget slot between primary and secondary loadout. |
| **Space / Enter**| Commence Heist | Finalizes deck planning selection and deploys onto the barge. |
| **R** | Restart | Quick restart after winning or being pinned. |

---

## ⚡ Core Gameplay Mechanics

### 1. Dynamic Bounty Projection
Every lot you carry projects a floating holographic price tag directly above your head:
* **Empty / Low Bounty:** Subtle cyan hologram (`$0 – $250k`).
* **High Bounty / Lot One:** Blinking crimson warning (`$1,000,000+`).
* **Drone Reaction:** Patrolling camera drones possess vision cones that **dynamically expand from 350 cm up to 1100 cm** when detecting high visible bounties!

### 2. Laser Security Grid & Crouch Evasion
High-value corridors are guarded by animated pulsing laser tripwires (`ALaserTripwire`):
* Walking through standing trips the alarm, alerting all nearby drones.
* Holding `Ctrl` crouches under the 70 cm beam height, slipping through undetected.

### 3. Bidding War Tactical Blind
Using the **Decoy Bid Paddle** initiates a frenzied bidding war among the VIP masked bidders:
* Red auction flash strobes across the deck.
* Drone optical sensors are overloaded, **blinding all security drones for 4.0 seconds** and creating a noisy window to sprint.

### 4. Fake Lot Tag Camouflage
Using the **Fake Lot Tag** injects false RFID metadata into your carried haul, **zeroing your projected bounty for 8.0 seconds** to bypass security checkpoints.

### 5. Tactical Radar & Input Visualizer
* **Minimap (Bottom-Left):** 25m tactical radar displaying player coordinates, nearby plinths, drone patrol routes, and the green extraction skiff beacon.
* **Key Input Visualizer (Bottom-Right):** Real-time on-screen key indicators that light up neon-green as `W`, `A`, `S`, `D`, `Ctrl`, `E`, and `Q` are pressed.

---

## 🏗️ C++ Architecture Overview

```
Source/PriceOnYourHead/
├── PriceOnYourHead.Build.cs          // Build rules & module dependencies
├── PriceOnYourHead.h / .cpp           // Primary game module declaration
├── PriceOnYourHeadGameMode.h / .cpp   // Master heist flow, Gavel timer (90s), Autoplay runner, QA rules
├── PorterCharacter.h / .cpp          // Third-person character, crouch stealth, inventory, bot navigation
├── CameraDrone.h / .cpp              // Aerial patrol AI, dynamic cone perception, pinned failure logic
├── AuctionPlinth.h / .cpp            // Velvet pedestals with floating holographic price tags
├── LaserTripwire.h / .cpp            // Animated pulsing security beams with height-based overlap checks
├── EscapeSkiff.h / .cpp              // Extraction boat trigger volume with medal rank evaluation
├── BargeLevelGenerator.h / .cpp      // Procedural world construction (barge hull, bleachers, 6-point lights)
└── PriceOnYourHeadHUD.h / .cpp       // 100% C++ Canvas HUD (Planning screen, In-game HUD, Radar, Input display)
```

---

## 🎨 Asset Library & Legal Provenance

All assets comply strictly with Parsewave's commercial redistribution guidelines (zero AI-generated models, zero AI voices, zero Marketplace packs).

| Asset File | Type | Source / Provenance | License |
|---|---|---|---|
| `Fonts/Orbitron.ttf` | Typography | Matt McInerney via Google Fonts | SIL Open Font License 1.1 |
| `Models/CryoCanister.obj` | 3D Mesh | Procedural cylinder geometry (Lot One) | CC0 1.0 Universal |
| `Models/CameraDrone.obj` | 3D Mesh | Procedural quad-rotor security drone | CC0 1.0 Universal |
| `Models/VelvetPlinth.obj` | 3D Mesh | Procedural octagonal display pedestal | CC0 1.0 Universal |
| `Models/EscapeSkiff.obj` | 3D Mesh | Procedural extraction skiff hull | CC0 1.0 Universal |
| `Textures/T_WetDeck_Roughness.tga` | PBR Texture | Wet steel roughness map with puddle highlights | CC0 1.0 Universal |
| `Textures/T_HoloGrid_Cyan.tga` | PBR Texture | Emissive scanline grid pattern | CC0 1.0 Universal |
| `Textures/T_Velvet_BaseColor.tga` | PBR Texture | Deep luxury violet cloth albedo | CC0 1.0 Universal |
| `Audio/Footstep_WetMetal.wav` | Audio SFX | Synchronized wet metal deck step | CC0 1.0 Universal |
| `Audio/Heartbeat_Tense.wav` | Audio SFX | Sub-bass lub-dub stealth tension | CC0 1.0 Universal |
| `Audio/DroneHum.wav` | Audio SFX | 95Hz dual-sine turbine engine loop | CC0 1.0 Universal |
| `Audio/GavelStrike.wav` | Audio SFX | Resonant auction gavel strike | CC0 1.0 Universal |
| `Audio/BiddingWarAlert.wav` | Audio SFX | Emergency alert strobe siren | CC0 1.0 Universal |
| `Audio/SmokeHiss.wav` | Audio SFX | Pressurized gas canister release | CC0 1.0 Universal |
| `Audio/LotGrabbed.wav` | Audio SFX | Digital pickup chime | CC0 1.0 Universal |
| `Audio/RainAmbience.wav` | Audio SFX | Flooded harbor rain on water ambience | CC0 1.0 Universal |

---

## 🛠️ Verification & Tooling

The project includes an automated tool suite in `_tools/`:

```bash
# 1. Compile C++ Code with UnrealBuildTool
python _tools/build.py PriceOnYourHead

# 2. Package Submission ZIPs (asset-library.zip and PriceOnYourHead.zip)
python _tools/package_game.py PriceOnYourHead

# 3. Verify Clean Extraction (ensures zero Intermediate/Binaries leak)
python _tools/clean_unzip_test.py PriceOnYourHead

# 4. Analyze Gameplay Video (checks 1080p, 60fps, duration, near-black ratio)
python _tools/scan_video.py path/to/recording.mp4
```

### Autoplay Verification
Launch the compiled editor or standalone game with `-autoplay -autoplayquit`:
* Automatically executes the planning phase in 1.5s.
* Navigates stealth waypoints, gathers bonus lots, uses the decoy paddle, steals Lot One, and boards the skiff.
* Automatically quits with code 0 upon achieving a Gold Medal victory with zero QA violations.

---

## 🏆 Win & Loss Conditions
* **Gold Medal:** Escape with Lot One + haul $\ge \$1,500,000$ in $\le 65$ seconds.
* **Silver Medal:** Escape with Lot One + haul $\ge \$1,000,000$.
* **Bronze Medal:** Escape with Lot One before the gavel strikes.
* **Loss (Pinned):** Stay within 160 cm of a security drone for $\ge 2.0$ seconds without breaking line-of-sight.
* **Loss (Gavel Timeout):** 90-second round clock expires before reaching the extraction skiff.

---

*Authored for the Parsewave Unreal Engine contract. Developed by Abhranil Singha Roy.*
