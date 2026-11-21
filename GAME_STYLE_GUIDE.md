# Choosing a style for an agent-built Unreal game

A short guide for picking camera, characters, world and visual style when an AI agent builds the game and a person reviews it on video. It comes from seven small Unreal Engine 5 games built this way on a laptop with a 4 GB graphics card.

**For people:** read "The short version", then only the table for the choice you are making.
**For your agent:** give it this whole file and say "use this when we plan the game; recommend from it and tell me the risk of each option".

Each risk is marked **(seen)** if it happened in one of our games, or **(expected)** if it is reasoning we have not tested.

## The short version

1. **Looks come first, but spend on lighting, colour and atmosphere, not on detailed models.** Simple clean models under good lighting beat detailed models under flat lighting, and cost far less.
2. **Pick the cheapest option that still suits the game.** Cheapest first:
   - Camera: first person or top-down, then isometric, then side-on, then third person.
   - Characters: rigid things (robots, drones, vehicles), then none, then simple creatures, then humans.
   - World: one enclosed space, then a few small connected areas, then open outdoors.
3. **Nothing on screen may clip or flicker.** Fewer objects with no faults beat more objects with one fault. Most of our polish time went on removing flicker, not on adding anything.
4. **Decide the style before building, and look at one still image of it first.** A style changes how every material is made, so changing it later means redoing all of them.

## Camera

| Camera | Cost | Good | What goes wrong |
|---|---|---|---|
| First person | Lowest | No player body to model or animate. The world fills the screen, so lighting does the work. | Screen-fixed effects look wrong because the view moves constantly **(seen)**. Fine detail far away shimmers **(seen)**. Mouse sensitivity needs tuning by a person. |
| Top-down | Low | Everything is small, so models need little detail. Easy for a bot to play. | Things can be in range but off screen, so a bot or player cannot click them **(seen)**. Roofs and walls hide the action **(expected)**. |
| Isometric | Low to medium | Same as top-down, with more depth and a better look at models. | Tall objects cover what is behind them **(expected)**. |
| Side-on | Medium | Levels are simple to lay out. Models only need to look right from one side. | A character is on screen the whole time, so it still needs decent animation **(expected)**. |
| Third person | Highest | Shows off a character; the most familiar look. | Needs a rigged, animated character always in view, where every flaw shows. The camera goes through walls and the character's parts pass through each other unless both are handled **(expected)**. |

## Characters and enemies

| Kind | Cost | Good | What goes wrong |
|---|---|---|---|
| Rigid things: robots, drones, vehicles, turrets | Low | Solid parts, no bending skin. Stiff movement looks right on them. | Flying ones pass through walls unless they are told about obstacles **(seen)**. The model can come out bigger than its collision size **(seen)**. |
| None: hazards, objects, puzzles | Lowest | Nothing to rig or animate. | The game needs another source of challenge and life, or it looks empty **(seen)**. |
| Simple creatures | Medium | More personality than a machine. | Each needs a skeleton and a few animations; imported animations can shrink or hide the model if scale is wrong **(seen)**. |
| Humans | Highest | Relatable. | Many animations, and people spot wrong human movement at once **(expected)**. |

## World

| Kind | Cost | Good | What goes wrong |
|---|---|---|---|
| One enclosed space | Lowest | Few assets, lighting fully controlled, steady frame rate, fits a short video. | Looks bare if only the gameplay objects are placed. Plan background dressing as its own list **(seen)**. |
| A few small connected areas | Medium | More variety on video. | Each area needs its own assets and lighting **(expected)**. |
| Open outdoors | Highest | Scale and scenery. | Terrain, sky, plants and distance are the most work to build and to run **(expected)**. Large buried pieces from different makers end up overlapping **(seen)**. |

## Visual style

Realism is the most expensive style and the one where agent-made models fall shortest. These stylised looks are done mostly with screen effects and simple materials, so plain models look deliberate.

| Style | Cost | Suits | What it needs | What goes wrong |
|---|---|---|---|---|
| Cel / comic | Low | Action, comedy | Stepped lighting, thick outlines | Outlines shimmer on small far-away detail **(seen)**. Dot (halftone) shading slides across objects if tied to the screen **(seen)**. Lighting worked out inside the material means no real shadows **(seen)**. |
| Retro (PS1, CRT) | Lowest | Horror, arcade | Low-resolution image, wobbly vertices, dithering | Small text becomes unreadable; draw the HUD at full resolution **(expected)**. |
| Pixelated 3D | Lowest | Roguelikes | Low-resolution image scaled up sharply, limited palette | Pixels crawl when the camera moves slowly; thin objects vanish **(expected)**. |
| Ink and wash | Low | Detective, period pieces | Outlines, flat shading, paper texture | Same outline shimmer as cel. A paper texture fixed to the screen slides like the dots did **(expected)**. |
| Painted | Medium | Pastoral, storybook | A smearing filter over the whole image, canvas grain | The filter runs on every frame and can lower frame rate; it also smears HUD text unless that is drawn after it **(expected)**. |
| Woodcut / engraving | Medium | Historical, nautical | Hatching lines driven by light | Hatching flickers as light changes, and slides if tied to the screen **(expected)**. |
| Paper cut-out | Low to run, high to make | Theatre, folk tales | Flat cards, drop shadows, layers | Every object needs drawn 2D art. Cards at the same depth flicker against each other **(expected)**. |
| Stop-motion / clay | Highest | Toys, miniature worlds | Fingerprint surface on every asset, 12 frames-per-second animation, blurred background | Most work per asset. Stepped animation can look like stutter if the camera is also stepped **(expected)**. |

## The faults to design out

These caused nearly all our rework. Each has a cheap rule that prevents it.

| Fault | Cause | Rule |
|---|---|---|
| Surfaces flicker where they meet | Two faces in exactly the same plane | Nothing sits flush. Details stand out or sink in by a few millimetres, and parts made separately use different depths so they cannot land on the same plane. Check all pieces of a scene together, not one at a time. |
| Far detail shimmers | Detail smaller than a pixel, with outlines drawn on it | Fade inner outlines with distance and keep only silhouettes. Use anti-aliasing that works across frames. Adding more small detail makes this worse. |
| Patterns slide over objects | A pattern fixed to the screen instead of the surface | Fix patterns to the surface. |
| Signs and labels flicker | See-through objects in front of other see-through objects | Anything with text is solid, never see-through, when glass is in the scene. |
| Everything is tiny or invisible | Unit mismatch when importing models and animations | Measure one imported object against its planned size before anything else. |
| Things pass through walls | Enemies given no collision so they cannot trap the player | Moving things still test against walls and furniture. |
| The level looks empty or random | Only gameplay objects were planned | List background dressing separately. Give every free-standing object a base and a reason to be where it is. |

## Smaller choices

- **HUD:** a few small elements at the screen edges in a font chosen for the game's setting. Blocks of text need a dark backing to be readable; bars and counters do not.
- **Showing controls on video:** a small corner panel of keys that light up as they are pressed shows every input for the whole video.
- **Sound:** one free music track for mood plus generated sound effects is the cheapest set that still sounds complete.
- **Size:** make one complete run, start to win screen, fit the video. Anything the video cannot show is work nobody reviews.
- **If a bot plays for the video:** script only moves it can guarantee, give each beat an earliest start time so the run is not too short, and make sure it can only target what is on screen.

## Rules to give the agent

1. Recommend from the cheapest option that suits the idea, and say what each option costs and what can go wrong with it.
2. For every object and effect proposed, say what could clip or flicker and how the plan prevents it. Offer the leaner list as well.
3. Show one still of the chosen style before building it into every material.
4. Do not judge the look yourself. Build what was agreed, record it, and let the person review the video.
