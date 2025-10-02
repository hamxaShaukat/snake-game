# Snake Game (C++ OOP Project)

An object-oriented implementation of the **classic Snake game** in C++.  
The project supports single-player and multiplayer (up to 4 snakes), different map types, scoring, and special gameplay modes.

## 🎮 Features
- **Movement** – Snakes can move in four directions, restricted from reversing into themselves.
- **Multiplayer Support** – Up to 4 snakes with at least one controlled via arrow keys.
- **Food Generation** – Randomly placed food that grows the snake when eaten. Ensures food never spawns on a snake or wall.
- **Toroidal Map ("Duniya Gol Hai")** – When snakes exit one edge, they reappear from the opposite side.
- **Score Tracking** – Keeps score based on food eaten, displayed during gameplay.
- **Bounded Mode** – Snakes die if they hit map boundaries.
- **Stages** – Includes maps with walls and obstacles.

## ⭐ Bonus Features
- Multiple stage options (not just empty map).
- Time-based mode: target score + timer.
- Time-based food that disappears after a few seconds.
- Sound effects and background music (for enhanced gameplay).
- Simple GUI console based (checkboxes, text input for player names, game mode selection).

## 🛠️ Tech Stack
- Language: **C++ (OOP)**
- Concepts: Structs, Objects, Inheritance, Encapsulation
- Console/GUI utilities (for rendering, input handling, sound, UI)

## 🚀 How to Run
1. Clone this repository:
   ```bash
   git clone https://github.com/hamxaShaukat/Snake-Game.git
