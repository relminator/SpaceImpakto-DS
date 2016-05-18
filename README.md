# SpaceImpakto-DS
A Bullet-Hell SHMUP(Space Shooter) Nintendo DS Homebrew

1st Place winner in the GBAtemp Homebrew Bounty 2011 DS Category.

![Alt text](http://www.rel.phatcode.net/nds_temp/SIDS_screen_03.png "SHMUP")

![Alt text](http://www.rel.phatcode.net/nds_temp/SIDS_screen_02.png "SHMUP")


Instructions:

	Copy "SpaceImpaktoDS.nds" to your flashcart and enjoy!!!

	*Also try to wait for a few seconds(approximately 20 seconds) while in the main menu
	to see a 1cc - 2 loop - demo.

	*If you don't have a DS and a flashcart, you need an emulator to play this
	on the PC.  Try No$GBA or DesMuMe(both are free).
	
Tips:
	Shoot everything that moves.
	There is always a safe area no matter how crazy the patterns are.
	Try to destroy the boss as much as possible.  There is a boss timer
	   that will auto destroy the boss but you won't get a score bonus
	   this way.
	Replay is only available after 1 game( doesn't matter if you loose or not) or...
	Replay is also available when there is a saved replay on the flashcart.
	Replay is not available when you return to the main menu when the game is paused.
	Score should be a little higher with autobomb set to OFF.
	Ask around at the GBAtemp forums for the cheatz.
	If all else fails, don't move!
	

GameType: Bullet-hell SHMUP (Space Shooter/Shoot 'Em Up) for your DS


Controls: (Change within the Options menu)
	B = Shoot
	A = Bomb
	R = Slowdown

Features:

	* 2d gameplay
	* 3d background
	* Optimized texture use via texture packing
	* Oldskool 8-bit/4bit sprites
	* Newskool bullet hell gameplay
	* Source included( use and abuse license but give credit where it is due ;*) )
	* Replay system
	* Button config
	* Varied and animated Bosses
	* Proceduraly generated backgrounds
	* Proceduraly generated 3d models 
	* 3 types of ships
	* 2 Game Modes (Normal and Boss Rush)
	* Auto-bomb (see Options menu)
		(When autobomb is ON, you won't have bomb powerups at the end of the stage
		 so "Pick Your Poison".)

Development:
	(See in-game Credits for complete list)


	Official Site: Http://Rel.Phatcode.Net

	Programming: Richard Eric M. Lope BSN RN (Relminator)

	Design: Anya Therese B. Lope
		Relminator
		The whole GBAtemp community



Tools: 	DevkitPro - Devkipro.org
	libnds - Devkipro.org
	Easy GL2D - http://rel.phatcode.net (Junk->DS Homebrews->EasyGL2D)
	MaxMod - Devkitpro.org
	Rel's 8-bit Texture Packer - http://rel.phatcode.net (Junk->FB trash->Texture Packer)

	
Updates/Changes:

v 2.0.0 FINAL  (09/21/11)
	Migrated code to latest DKP toolchain
	CPU controlled demo(wait 20 seconds in the main menu to activate)
	AutoBombs "pick your poison" version.
		* When you use autobomb, you won't have bomb powerups at the end of the stage.
		* AutoBomb State is saved for future gameplay. 
	Saves are now incompatible with older versions.
	Player name is saved for future gameplay as Cyan suggested.
	Button Configuration is saved future gameplay. 
	Loop and Stage is displayed on the lower right corner.
	Cheat c0d3z!!! Score is limited to 50k. Ask Wasim how.
	Score bonus system changed 
	Demo and Replay games skip the intermission as suggested.
	Included some bragging rights in the splash screen for kicks.
	Bug Fixes
	
v 1.3.0  (06/21/11)
	New Ship!!!!
	Autobombs!(Only for bullet collisions)
	Added boss revenge bullets when timer runs out (both loops)
	Added boss revenge bullets when boss is destroyed in loop 2
	New BombType
	Changed drawing order of ship and shots for the new ship
	Changed Main Menu font colors
	Bug Fix

v 1.2.0  (06/17/11)
	Changed the order ships' shots vs. enemy bullets get drawn
	Included another 1 cc replay (until 2-3 Blue Bagwis).
	Added all the assets with the source so anyone can compile the game.

v 1.1.0  (06/04/11)
	Recompiled without debugging features
	Included my 1 cc replay (until 2-3 Red Agila).
	
v 1.0.2  (06/03/11)
	Increased the firepower of Blue Bagwis
	Fixed the bug where you get the ending screen even if you force exited.
	HighScore table now saves the highest score achieved on multi-credit game
	Added a new boss (Gluteus Maximus)
	Added A new Stage
	Only the 1st credit score gets to be recorded on the highscore table
	Replay now records both game loops
	
v 1.0.1  (05/30/11)
	Fixed the bug in the replay system interface
	Changed the Replay System Interface to a more intuitive one.
	Normal and Boss Rush has different highscore tables
	Second loop!
	Loop 2 is the "HARD MODE"(For HardCore BulletHell players) ala Strikers 1999 and Gunbird 2 
	Loop 2 is reminiscent of Psykyo 2nd loops where you kill something, it explodes bullets.
	Level is also spawnign "popcorn" enemies when facing bosses to up the ante of difficulty.
	Edited some enemy sequence to make the first loop beatable w/o using a single bomb.
	
	
v 1.0.0  (05/27/11)
	Finally got it non beta!!!
	Fixed the replay system for Red Agila (mistyped variable)
	Changed how the intermission screen works
	Boss Rush Mode
	BGM is attenuated when pausing 
	Added more menu items
	Added a way to save or load Replays(3 slots)
	Player is able return to main menu when paused.

v 0.9.1b  (05/26/11)
	Added bomb powerup
	Added 1-Up
	Changed the font on the subscreen BG to make it bigger
	Added Modariffic.com on the splash screen
	Player "aura" when invincible
	Intermission screen
	Can't continue when in replay mode
	Each ship has a different bombtype
	Current Hi-score is loaded from file if present
	Reconfigured sound modules for better channel use.
 
v 0.9.0b  (05/23/11)
	Finalized level BGs
	Added per level BGMs
	Fixed projection matrix bugs (3d and 2d z-fighting)
	Added more SFX
	Sequenced stages 2-4
	HighScore table is externally saved
	Fixed some sound issues
	Reactivated "Replay" system
	Replay now saved even if you don't finish the level.
	

v 0.8.7b  (05/19/11)
	Sequenced Stage 1
	Changed boss life meter
	Splash screen
	Almost proper level BG's
	
v 0.8.6b  (05/09/11)
	Added a new boss (Taknom)
	Boss Hp can be seen on-screen
	Moar enemies

v 0.8.5b  (05/04/11)
	Fixed the plasma color bug (used memcpy() instead of DMA)
	Changed the HighScore text entry button interface (now very arcade-ish).
	Added a new capability to BulletRel
 after playing Ketsui DS
	Added another boss (Tentakill) that behaves like the 2nd level lifeforce(NES) boss
	Added the 3rd boss (Transporma)
	Boss Rush Mode
	

v 0.8.4b  (04/09/11)
	Finalized Boss sequence
	Added a lot of enemies
	Enemy sequence test
	boss timer to remove "boss milking"
	Bugfixes

v 0.8.3b  (04/04/11)
	Boss Sequence Test
	Bugfix
	

v 0.8.2b  (03/30/11)
	HighScore Table
	Name Entry

v 0.8.1b  (03/29/11)
	Replay System (Just 32 KB. Yay!)
	Key Config (Cearn's Idea)
	Plasma BGs (I went for the symmetry this time)
	Sub BG renderer
	
v 0.8.0b  (03/23/11)
	Sexy Menu and GUI
	Plasma Fonts
	Sub BG renderer
	Warp FX
	Fonts		

v 0.7.2b  (02/25/11)
	Updated the source for libnds 1.5.0 (took me hours!)
	Ditched stencil effects(glAssignTexture() is kinda slow)
	Fx3D() singleton
	Added some memory handler/feedback (2mb+ free memory still to use. yay!)		

	

v 0.7.1b  (01/19/11)
	Centipede enemy
	Bombs
	"Engine-ized" the game

	

v 0.7b  (01/16/11)
	Added a second player (the R9)

	Missiles
	More enemies
	


v 0.6b  (01/11/11)
	Decided
 that the current engine sucks and needs an overhaul	
	Recoded everything using the system I used in Bubble Fight EX
	

v 0.5b  (07/23/10)
	glFonts
	oamFonts
	Encapsulated maxmod for a better sound module
	Return to homebrew menu 
	Decreased all the enemy's HP
	

v 0.4b  (07/18/10)
	Integrated bullet Rel
	Added sound effects and BG music (via MaxMod)
	Bucket based memory model
	Data driven engine
	Ditched the options
	More enemies

v 0.3b	(06/05/10)
	Added an enemy handler
	Added collision detection (AABB)
	Added explosion handler
	Encapsulated most sprites
	Developed a tool for texture packing

v 0.2b = Migrated rendering engine from OAM to 3d core

v 0.1b = OAM sprites implemented

v 0.0b = (04/20/10) started coding.



