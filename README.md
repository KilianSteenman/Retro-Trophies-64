# N64 Achievements

An Everdrive achievement system that lets you earn 'achievement' for N64 games.

## How does it work?

As we can't check the status of the game at runtime let alone render anything. 
This application reads the save files of games on the Everdrive and rewards achievements according to the save state.

### Limitations

- In some cases events are not stored in the save data (eg. finish zelda), making it impossible to reward an achievement.
- Some events are stored in save data, but will be removed when the action is undone. For example losing Mario's hat. To earn the achievement, the player needs to lose the hat, save and open the trophy app to obtain the coin. (Will this even be possible without keeping track?)
- No runtime popup to indicate if something is achieved.
- Player needs to quit the game to see the achievement overview.
- Games that use a memory pack are not available (yet?).

## Coins

When the user achieves something in the game, the player is rewarded with a coin. A coin can be rewarded for a specific event or reaching a specific amount. 

It's possible that a coin is rewarded for a big event in the game, in this case the coin description should be hidden.

If a coin is rewarded for reaching a specific amount, a progress indicator is displayed. 

### Coin levels
Not every achievement requires the same amount of effort, so there are multiple "levels" of coins to rewards.

#### Yellow coin
A basic coin, for a basic achievement.

- Mario:
  - First star
  - Lost cap
  - Found cap
- Zelda:
  - First Skulltula

#### Red coin
Finished a level or dungeon

- Mario:
  - Got all stars in bob-omb
- Zelda:
  - Finished Deku Tree

#### Blue coin
Milestones.

- Mario:
  - Unlocked basement
  - Unlocked top floor
  - Wing cap
  - Metal cap
  - Invincibility cap
- Zelda:
  - Got the Master sword
  - Deku Tree died
  - Deku sprout
  - Got all skulltulas

#### Gold coin
Finished the main game

- Mario:
  - Win final bowser battle
- Zelda:
  - Beat Ganon (if possible in save file) or all dungeons and got to Gannon.

#### Star (Game specific platinum trophy?)
100% Completed the game
- Mario:
  - Earned all 120 stars
  - Received all Yoshi's lives
- Zelda
  - Got all item upgrades
  - Maxed all collectibles (rupees, arrows, etc)?
  - Beat all dungeons
  - Got all Skulltulas
  - Traded all masks

## Easter egg
- Use rupees instead of coins for Zelda games.