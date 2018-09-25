This is a specification sheet. It is a design specification. The specification also includes internal specifications.  

__Caution__ The following configuration is at the time of writing, there is a possibility of future change.  

# Current system configuration  
In this chapter, please write only the system configuration which has already been realized.  
Please describe another chapter as much as possible for the description of future design goals and write it there. Please do not write future design goals in this "current system configuration".

## composition of the source file 
- sujinikuRpgRuntimeCppWin32.cpp is the entry point.  
- GameData contains materials for games (images etc).  
- File division is not done.  

## Mode specification
### Basic mode
During the game, the game state is classified into the following basic mode.  

Mode name     |Mode number|Variable name|
-----------:|-------:|-------:|
Opening  |     100|    MODE_OP|
Initial event  |     200|     MODE_INITIAL|
Map       |     300|     MODE_MAP|
Menu screen  |     400|     MODE_MENU|
Battle         |     500|     MODE_BATTLE_COMMAND|

There is no particular deep meaning in the number.  
"Menu screen" is a screen for doing "tools" and "equipment".  

### Detail mode

In fact, in addition to the above basic modes, there are also cases where it is classified more finely.  

Combat command: MODE_BATTLE_COMMAND
Battle victory: MODE_BATTLE_WIN  
In some cases, it may be classified into individual modes.  

Battle victory (MODE_BATTLE_WIN) is "580" number. In this way, the second digit from the top is used for detailed classification. I need to check the source code.  

## font  
It is a setting that uses Windows standard font.  
This game itself does not incorporate fonts.  
In the source code, we do not perform processing as specified for fonts.  

## Function contents  
WM_PAINT performs drawing only. WM_PAINT does not perform calculation for game progression other than drawing. Processing for game progress is in WM_KEYDOWN handling keyboard input or timer processing WM_TIMER.  

__Function on map screen__  
Draw_map : Function for drawing map screen. check_movable: A function for judging that the planned location specified by the arrow key is movable.   check_encount_enemy: Judgment of encounters with monsters.
check_MapTransition : Determining whether the moved location is a warp zone or not. (The entrance of the map is also included in the warp zone.)  

__Menu screen__
item_select : For calculating the cursor position in the item menu.  

__Battle function__
battle_start: We make the necessary substitution when entering battle. Assignment of enemy HP etc.  
hero_attack: Process in the case where the main character attacks in battle.  
enemy_attack: Processing when an enemy attacks in battle.  

draw_battle_common_before: A function that draws the background of the battle screen. However, at present, there is only a pure white background.  
draw_battle_common_after: A function for drawing the HP display field of the battle screen. HP is displayed for the whole time in battle, in principle.  

draw_battle_HeroDamage: Draws damage received by the main character party. Since the display position is different from the enemy, it is done with another function.  
draw_battle_EnemyDamage: Draws the damage the enemy received.  

## Contents of structure
chara_chip_def: A structure for grouping the image handles of the character chips in each direction (up, down, left, and right) on the main character's map screen.  

item_def: a structure that defines items such as "medicinal herbs"  
item_have: A structure that is scheduled to keep the number of items held by the leading character. (Incomplete as it is working)  

monster_def: Definition of a monster. Name (slime, cobolt etc.) and max HP etc. Also include the monster number (ID) in this.  

MapTrans_def: Warp zone structure. We use this structure as we treat entrances and exits of each map as warp zones.  
## Contents of variables  
Since we have too many variables, we do not introduce all the variables.  

mode_scene: Variable for managing the mode number of the current mode (battle mode, opening screen mode, etc.).  
  
next_attack_flag: Who will attack next? In the current system, only two of the enemies and the hero participate in the battle in the battle mode, so we manage it by this.  

encount_monters_id: When encountering an enemy, save the ID of that monster. He records who fights against his opponent.  
monster_name: Save the name of the monster you encountered.  
monster_hp: You save the HP of the monster you encountered.  
encount_mons_alive: Flag to distinguish between life and death of the monster encountered. Set to 0 if you died. Set to 1 if you are alive.  

__Selection item__
selecting_OP: This is a selection screen such as "From the beginning" "From the continuation" on the opening screen, and records the number of commands selected. The first is "from the beginning", the second is "from the continuation".  

selecting_mainmenu: The main menu screen for "tools" and "equipment" etc. records the number of commands selected.  

__The main character's parameter__
your_money: Possession of the main character party. Although it is said that at present the party has only one hero.  

hero_hp: The current hit point of the hero. HP. When it becomes 0, the game is over.  
hero_hp_max: Highest hit point of the hero.  
hero_agility: The hero's quickness.  

hero_exp: The cumulative experience value that the hero has acquired up to now.  

__Conversation system__
drawTalkString1: The text of the first line of the conversation window.  
drawTalkString 2: The text of the second line of the conversation window.  
drawTalkString3: The text of the third line of the conversation window.  

Since text is included, drawTalkString 1 to 3 are array variables.  
When you use the parenthesis sign of the array, GitHub misunderstands that it is a web link, so the brackets are omitted in the specification.

__map__
where_map: What number of map is the main character party  
maptable: A two-dimensional array showing the topographic data of the map of the current position of the hero. The definition of individual maps is done with another variable.

chara_x: What is the hero in the X coordinate on the map  
chara_y: What is the hero in the Y coordinate on the map  

start_x: On the map at the start of the game, what number is the hero in the X coordinate.  
start_y: On the map at the start of the game, what number is the hero in the Y coordinate.  

desti_x: The X coordinate of the hero's destination to move or move to. Calculated when the arrow key is pressed etc.
desti_y: The Y coordinate of the main character of the hero or the destination to move to.
If the planned destination of the destination is a movable mass, the content of desti is substituted into the position of the leading character (such as chara_x).

direction: An enumeration that indicates the direction of the character, such as the hero on the map. Either up, down, right, or left. hero1_direction: Variable representing the direction of the main character on the map. Only the hero. Other characters are represented by other variables.

MapTrans_position_x: X position of warp zone. We use this variable because the entrance and exit of each map is also treated as a warp zone.  
MapTrans_position_Y: Y position of the warp zone.  

positionX_enemy: The X coordinate of the position of the monster on the map. This game has the function that monsters are displayed on the map.  
positionY_enemy: The Y coordinate of the position of the monster on the map.  

kCount: Variable that limits the keyboard input on the map screen to a certain amount within a certain period of time and increases with the lapse of time.  

## Other
When you finish entering the command, battle automatically progresses with a timer until the next turn. We use a variable named iCount as the count variable for the timer.  
This game is a turn-based battle RPG.  
