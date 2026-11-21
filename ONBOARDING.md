# Parsewave UE games: start here

Read this once before your first game. It covers what the job is, what you need installed, what you do yourself and what Claude does for you.

This pack has two files:

| File | Who reads it | What to do with it |
|---|---|---|
| `ONBOARDING.md` | You | Read it now. Keep it open during your first game. |
| `CLAUDE.md` | Claude | Put it in the root of your games folder. Claude reads it by itself at the start of every session. |

Console facts in this guide were checked on 14 November 2026. If the console looks different, trust the console.

## 1. The job

You claim a game idea, and Claude builds it in Unreal Engine 5: the code, art, UI, sound, tests and the package. You do not write the game. Your job is to choose a good idea, look hard at what Claude makes, record yourself playing it, and upload it. A moderator then accepts the game or asks for changes.

Moderators judge five things:

1. **Looks.** Polished and bright. No grey boxes, placeholder art or dark scenes.
2. **UI.** Simple and themed, like a shipped game. Not cluttered, not generic.
3. **Characters.** Real, artist-made models with living eyes and smooth movement. Never a mannequin.
4. **No obvious bugs.** Nothing passes through walls, floats, flickers or gets stuck.
5. **A story.** A hero worth caring about and real stakes, not just a run from A to B.

Visual quality decides pass or fail. A game that works but looks weak is rejected.

Watch the sample games before anything else. They set the quality bar: https://parsewave.ai/ue5-gameplay

**A note from the review team.** Our job is to accept your games, not to review them into shape. So play your own game before you send it. Most of what we send back is easy to spot once you do, and catching it yourself saves everyone the back and forth. Some contributors send us great work that needs very few changes. They are the people we most want to keep working with, and we would love every one to get these : )

## 2. The rules of the contract

| Area | Rule |
|---|---|
| Engine | Unreal Engine 5.8.x |
| Code | Real C++ gameplay. No Blueprints of any kind |
| Length | The run length on the idea's card. Pool ideas run from 30 seconds to 5 minutes. Complete play with a clear goal |
| Autoplay | The game can play itself from the title screen to the real win |
| Assets | Only assets that are free to use and redistribute. No Fab or Marketplace packs, even free ones. Nothing paid, nothing tagged NoAI, no AI-generated 3D models, no AI-generated voices |
| Originality | Inspiration is fine, copies are not. No existing characters, names or shows |
| The idea | What the idea says is what the game must do. Never swap its mechanic for another |
| Deliverables | A gameplay video, then an asset-library ZIP and a project ZIP |
| Payment | Weekly, by Wise or crypto |

`CLAUDE.md` holds the full version of these rules, so Claude follows them without being told.

## 3. What you need before you start

### A computer

Windows and Mac both work. These three machines have all shipped accepted games:

| Machine | Notes |
|---|---|
| Windows desktop, 6-core CPU, 32 GB RAM, 16 GB graphics card | Comfortable. Fast builds and recordings |
| Windows laptop, 8-core CPU, 16 GB RAM, 4 GB graphics card | Works. Run at most two games at once |
| MacBook Air, Apple M-series | Works. The screen is smaller than 1080p, so captures are scaled up |

Keep at least 100 GB of disk free. Every game fills the drive with build caches, test builds and recordings.

### Software

| | Windows | Mac |
|---|---|---|
| Engine | Unreal Engine 5.8, through the Epic Games Launcher | The same |
| Compiler | Visual Studio with the "Game development with C++" workload | Xcode from the App Store. Open it once and accept the licence |
| Video tools | FFmpeg | FFmpeg |
| Scripting | Python 3 and Git | Python 3 and Git |
| Screen recorder | Xbox Game Bar (built in) or OBS | OBS. The built-in Mac recorder does not record game sound |
| AI | Claude Code, provided by Parsewave | The same |

Quick installs for the small tools:

```
Windows (PowerShell):
winget install Gyan.FFmpeg
winget install Python.Python.3.12
winget install Git.Git

Mac (Terminal, with Homebrew):
brew install ffmpeg python git
```

You do not have to check any of this by hand. In your first session Claude tests the machine and tells you exactly what is missing.

Optional: the free `unreal-engine-skills` collection (MIT licence) teaches Claude good Unreal C++ habits. Install it in your games folder with `npx skills add quodsoler/unreal-engine-skills`.

### A games folder

Make one folder for all your games, for example `GameDev`. Put `CLAUDE.md` in its root. Each game gets its own subfolder, which Claude creates.

```
GameDev/
├── CLAUDE.md          the rules Claude follows
├── _tools/            scripts Claude writes on your first game and reuses after
├── FirstGame/
└── SecondGame/
```

### Accounts

- Your Parsewave console login: https://parsewave.ai/hermes-unreal-games
- Optional: a free Sketchfab account. Some good free models need a sign-in to download.

## 4. How the console works

1. **Idea pool.** Over a thousand ready ideas, sorted by gameplay, core mechanic, setting and mood. Ideas someone else has taken are not listed.
2. **Claim.** Claiming an idea makes it your approved idea straight away. From that moment you have 48 hours to send its first gameplay video: if none comes, the idea goes back to the pool and you cannot claim it again. A claim cannot be given back early, so choose before you click. You can have up to 16 in progress. One stops counting when its video is approved.
3. **Video.** You upload a 1 to 2 minute gameplay video for the idea. The form also asks for a two-sentence summary: the game's goal, and what the video shows. You must type it in your own words. Pasting is switched off.
4. **Review.** A moderator replies with "Video approved" or "Changes requested", with notes.
5. **ZIPs.** After "Video approved", you upload two ZIP files in this order: the asset library first, then the Unreal project.
6. **Accepted.** The game is done, and it is safe to delete it from your computer.

A video that is declined ends that game, and its idea goes back to the pool for others.

Pitching your own idea is closed at the moment. Ideas come from the pool.

The console's **Submission guide** page has the official requirements and example ZIP files. Read it once before your first upload.

## 5. Choosing ideas

The idea decides most of the outcome before any code exists. Pick ideas Claude can make look stunning.

| Look for | Avoid |
|---|---|
| One clear, unusual rule a viewer understands in seconds | A mechanic that is hard to explain |
| One main character | Many humans on screen, crowds |
| A compact place: a room, an island, an arena | Open worlds and long journeys |
| Bright settings: daylight, a lit hall, a sunny reef | Night, caves, horror, dark water |
| Room for a difficulty ramp, a fail state and a score | Gameplay that is over in 20 seconds |
| A short, clear story with a satisfying ending | Long dialogue or cut-scenes |

**For your first three games, choose a non-human hero:** an animal, an insect, a bird, a fish, a machine or a vehicle. Human hands, faces and clothes are the hardest things to get right, and they cause the longest rounds of fixes. A bee or a submarine has no fingers to get wrong.

Not sure which to claim? Copy a few ideas you like from the pool and paste them to Claude with "help me choose". It explains each one in plain words, ranks them, and checks that a free hero model exists. Then you claim the winner on the console.

**Claim one idea at a time, when you are ready to start it.** Each claim gives you 48 hours to send its first gameplay video, and an idea that runs out of time goes back to the pool for good. You can hold up to 16, but claiming several at once usually means losing most of them.

## 6. What you do, step by step

There are six things only you can do. Claude does everything else and tells you each time it is your turn.

### Step 1. Set up, once

Install the software above and create the games folder. Open Claude Code in that folder and send:

```
First session on this machine. Follow CLAUDE.md and check that everything is ready.
```

Claude reports pass or fail for each tool, with the exact fix. Carry on when everything passes.

### Step 2. Claim an idea and start the game

Claim the idea on the console. Then open a new Claude session in your games folder and send:

```
New game. Follow CLAUDE.md.
This is game number <1, 2, 3...> for me. My computer is <Windows or Mac>.
Here is the idea I claimed, copied exactly:

<paste the title, the pitch, the look and the length from the console>
```

Claude now works alone, often for hours. You can leave. Use one session per game, and send every later message about that game to the same session.

Your first game takes longer than later ones, because Claude also writes its build and test scripts.

### Step 3. Sign in, once

If Claude says a model needs a Sketchfab sign-in, sign in yourself in the browser. Claude never types passwords. If you skip this, Claude uses the next-best free model.

### Step 4. Watch the review video, twice per game

Claude stops twice and gives you a video of the game running:

- **Review stop 1**, early: the hero, one real interaction, the UI and the lighting.
- **Review stop 2**, before recording: the whole game, from the title screen to the ending.

Watch each video full screen, from start to end, and pause on anything that looks off. A video shows what a picture hides: things passing through each other, flicker and stiff movement. Use the checklist in section 7, then reply:

```
Review stop <1 or 2> for <Game>. I watched the whole video.
1. <time in the video, for example 0:17>: <what looks wrong>
2. <time in the video>: <what looks wrong>
Fix these, then carry on.
```

If nothing is wrong, say so and Claude carries on. This look is the most valuable thing you do. Claude has already checked its own work, and it still misses things a person sees at once.

### Step 5. Play the game and record it

When Claude says the game is ready, it gives you a shot list and the command that starts the game. Play it yourself and record:

- Windows: Xbox Game Bar (Windows key + Alt + R) or OBS.
- Mac: OBS.

| Rule | Detail |
|---|---|
| Start | From game launch, on the title screen |
| Length | 1 to 2 minutes |
| Format | 1080p MP4, under 200 MB, the game filling the whole frame |
| Sound | Game sound on. Check it before sending |
| Shows | The goal, every core mechanic, the hard parts, one failure and the restart, then the win |
| Pace | Brisk but easy to follow. Never idle, never rushed |
| Clean | No mouse cursor, notifications or editor windows. Never cut frames out of the middle |

Playing is also how you find flaws: a stuck camera, a character walking through a chair, a card covering the action. Tell Claude about each one and record again after the fix.

Then tell Claude where the video file is. It checks the length, size and sound, and finishes the package.

### Step 6. Upload, and answer the moderator

Claude leaves everything in `Submission_<Game>` inside the game's folder. The asset library must hold the raw source of everything the game shows and plays, down to icons, fonts and sounds. Before you upload, ask Claude for its asset check: every file in the game matched to its source. The console takes ZIP files only.

1. Upload the video on the console and type the two-sentence summary.
2. After "Video approved", upload `asset-library.zip` first and `<Game>.zip` second.

When a moderator asks for changes, paste their words to the same Claude session, exactly as written:

```
Changes requested on <Game>. The moderator's exact words:
"<paste them, typos and all>"
Timestamp: <for example 0:17>   Screenshot: <attach it if there is one>
I watched the whole video again. I also noticed:
<time in the video>: <what looks wrong>
```

Do not summarise. The exact words matter. Then watch your whole video again, from start to end, before Claude fixes anything. The moderator names a few examples, not everything, so add whatever else you notice to the same message. Claude goes through the whole video too, fixes the named points and the rest, and tells you when to record again. Before you re-upload, play the whole game yourself, not only the reported spot.

Most accepted games went through one to three rounds of changes, so expect them.

## 7. What to look for at a review stop

These came up again and again in real reviews. Check each one at both stops and again while you play.

1. **Everything is solid.** No character walks through furniture, walls, props or another character. Nothing sinks into chairs, beds or the ground.
2. **Padding in every card.** Text has clear space on all sides and never touches its box or other text.
3. **Nothing overlaps.** No object sits inside another object, and no UI covers other UI or the hero.
4. **Natural grip.** Hands close around what they hold. Objects never pass through hands.
5. **Natural movement.** Smooth walking, turning, sitting and jumping, with feet planted and no sliding.
6. **Good assets.** Detailed textures and believable materials. Nothing blurry, stretched or plastic.
7. **A real character.** A proper face and eyes, natural hair, neat clothes. Not a mannequin, and only one body.
8. **The camera behaves.** It never goes inside anything, and nothing blocks your view of the hero.
9. **It is bright enough.** You can always see the hero and what is happening.
10. **The UI fits the game.** It looks made for this game's world, not like plain blue boxes.
11. **It is not too simple.** There are several ideas used together, it gets harder, and you can fail.
12. **It matches the idea.** Everything the claimed idea names is in the game.

## 8. Why games get rejected

| What moderators said | How to avoid it |
|---|---|
| The hero looks like a test bot or mannequin | A new, artist-made character per game, checked close up from every side |
| The UI is basic, generic or cluttered | A UI made from the game's own world, with few, large readouts |
| Too dark | Choose bright ideas. Look for dark stretches at review stop 2 |
| Things pass through each other or float | Watch for it in both review videos, and play the game off the usual route |
| Hands or held objects look broken | Avoid human heroes at first. Later, look at every hand action from the side |
| Flicker | Watch your own recording once from start to end before uploading |
| Too simple, just a course | Pick ideas with room for several mechanics, a fail state and a score |
| The game drifted from the idea | Compare the game with the idea's text at both review stops |
| Plain, low-quality assets | Ask Claude to compare three sources for anything that looks cheap |
| The fix only worked in the video | Always play the fixed game yourself before re-uploading |

## 9. Good habits

- **At most two games at once** on one computer. A third makes all of them slow or fail.
- **One session per game.** It keeps that game's context for every later fix.
- **Ask "how much more time?"** whenever you like. A good answer lists what is done, what is left and what is uncertain.
- **Free the disk after each upload.** Ask Claude to clean the game's build caches and test folders. It never removes the source, the content or the asset library.
- **A new character for every game.** Never reuse one, even recoloured.
- **Turn every rejection into a rule.** After a round of changes, ask Claude to add a one-line lesson to `CLAUDE.md`, so the next game never repeats it.
- **Never share other people's names or files** inside your package.
