# BattleBlaster

![BattleBlaster gameplay](Docs/Gameplay.gif)

BattleBlaster is a small third-person tank combat game developed with Unreal Engine 5 and C++.

The player controls a tank, destroys enemy towers, progresses through campaign levels, and receives a victory or defeat result depending on the match outcome.

The project began as a course project and was later extended with additional gameplay systems and architectural improvements.

## Gameplay

The player controls a tank and fights stationary enemy towers.

The match starts after a countdown. The player must destroy all towers to win. If the player's tank is destroyed, the match ends in defeat.

After a victory, the next campaign level is loaded. After a defeat, the current level restarts. Completing the final level restarts the campaign.

## Features

- Tank movement and cursor-based aiming
- Enhanced Input-based controls
- Enemy detection using an overlap sphere
- Enemy line-of-sight checks
- Turret rotation and aim-lock delay
- Projectile-based combat
- Health, damage, and death system
- Victory and defeat conditions
- Match start countdown
- Gameplay enable and disable state
- Campaign level progression
- Screen messages for countdown, victory, and defeat
- Niagara projectile trail, impact, expiration, and death effects
- Launch, impact, expiration, and death sounds
- Camera shake on projectile impact and pawn death

## Architecture

### GameMode

`ABattleBlasterGameMode` is the single source of truth for the current match state.

It:

- tracks living enemy towers;
- controls the gameplay enabled state;
- runs the match start countdown;
- determines victory and defeat;
- broadcasts match-state changes;
- requests level progression or restart after the match ends.

Gameplay actors and the player controller subscribe to these state changes and update their own behaviour.

### GameInstance

`UBattleBlasterGameInstance` manages campaign progression between level transitions.

It:

- stores the campaign map list;
- validates the campaign configuration;
- determines the current level index;
- loads the next level after victory;
- restarts the current level after defeat;
- restarts the campaign after the final level.

### BasePawn

`ABasePawn` contains functionality shared by the player tank and enemy towers:

- collision and mesh components;
- turret aiming component;
- health component;
- projectile spawning;
- gameplay state handling;
- death effects and sounds.

When a pawn dies, it notifies `GameMode`, plays the configured feedback, and then applies its death behaviour.

Enemy towers use the default behaviour and are destroyed. The player tank overrides it by disabling input, aiming, visibility, collision, and Tick.

### Tank

`ATank` handles player-specific behaviour.

It:

- configures Enhanced Input;
- moves and rotates the tank;
- fires projectiles;
- traces from the mouse cursor into the game world;
- passes the resulting target location to the turret aiming component.

### Tower

`ATower` handles enemy behaviour.

It:

- detects the player using an overlap sphere;
- checks line of sight using a line trace;
- rotates its turret toward the player;
- checks whether the muzzle is aligned with the target;
- fires after maintaining aim for a configured duration;
- returns the turret to its initial rotation when the target is unavailable.

### Components

`UHealthComponent` stores the current health, receives damage, prevents repeated death processing, and broadcasts a death event when health reaches zero.

`UTurretAimingComponent` rotates a configured scene component toward a target location while the component is active.

### Projectile

`AProjectile` uses `UProjectileMovementComponent` for movement and a Niagara component for its trail.

On impact, it:

- applies damage to valid pawns;
- spawns an impact effect;
- plays an impact sound;
- starts a camera shake;
- destroys itself.

A projectile that reaches the end of its lifespan can play a separate expiration effect and sound.

### PlayerController

`ABattleBlasterPlayerController` manages player-facing feedback.

It:

- displays the crosshair cursor;
- creates the screen message widget;
- displays the countdown, `Go!`, `Victory!`, and `Defeat!`;
- reacts to state changes broadcast by `GameMode`;
- starts camera shakes for projectile impacts and pawn deaths.

### UI

`UScreenMessage` is a small `UUserWidget` class that updates the text displayed by its bound text block.

## Controls

| Action | Input |
|---|---|
| Move forward and backward | W / S |
| Rotate tank | A / D |
| Aim | Mouse |
| Fire | Left Mouse Button |

## Running the Project

1. Clone the repository.
2. Open `BattleBlaster.uproject` in Unreal Engine 5.6.
3. Generate project files if required.
4. Build the project.
5. Open the main level and press Play.

## Project Status

The project is complete and is no longer in active development.

It represents the result of my current Unreal Engine C++ learning stage and will remain available as a reference project.