# ABYSS CRAWLER

A terminal-based dungeon survival game written in C using NCurses for macOS.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🎮 About the Game
**Abyss Crawler** is a roguelike ASCII survival game where the player explores a dark arena filled with monsters, potions, and treasure.

Your mission:
- Survive as long as possible
- Avoid monsters
- Collect treasure
- Heal with potions
- Score the highest points

The game features sound effects, background music, colors, and is designed to run in a macOS terminal.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🕹️ Controls

| Key | Action |
|-----|--------|
| W / ↑ | Move up |
| S / ↓ | Move down |
| A / ← | Move left |
| D / → | Move right |
| Q | Quit game |

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🧩 Game Elements

| Symbol | Meaning |
|--------|--------|
| `@` | Player |
| `m` | Monster |
| `+` | Healing potion |
| `$` | Treasure |
| `#` | Wall |
| `.` | Open floor |

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🔊 Audio System
The game includes sound and background music using macOS `afplay`.

File requirements:

```

intro.mp3   -> Background music
respawn.mp3 -> Respawn sound
death.mp3   -> Death music
heal.mp3    -> Potion sound
coin.mp3    -> Treasure sound
step.mp3    -> Footstep sound

````

All sound files must be in the same directory as the executable.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🛠️ How to Compile and Run (macOS)

### Step 1: Install ncurses (if not already installed)

```bash
brew install ncurses
````

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Step 2: Compile the game

```bash
gcc abyss_crawler.c -lncurses -o abyss
```

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

### Step 3: Run the game

```bash
./abyss
```

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🎯 Features

* Full-screen scalable map
* Color-coded characters
* Random respawn system
* Enemy chase AI
* Item pickup system
* Background music & sound effects
* Score & health system
* Clean ASCII UI

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 📦 Folder Structure

```
assignment3-abyss-crawler/
│
├── abyss_crawler.c
├── intro.mp3
├── respawn.mp3
├── death.mp3
├── heal.mp3
├── coin.mp3
├── step.mp3
└── README.md
```

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 👨‍🎓 Author

**Ayush Tomar**
Assignment 3 – Terminal Game Project

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## ⚠️ Notes

* The game is designed for macOS and requires `afplay` (available by default)
* Future expansions may include:

  * Boss fights
  * Multiple levels
  * Save system

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Have fun playing Abyss Crawler!

````

---

## ✅ SAVE + PUSH README

Then in VS Code Terminal:

```bash
git add README.md
git commit -m "Add README for Abyss Crawler assignment"
git push
````

---

