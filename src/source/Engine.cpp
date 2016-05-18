/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.phatcode.net

	Engine class
	
	
*******************************************************************************
******************************************************************************/

#include "Engine.h"

// ------------ Texture Packer --------------

#include "uvcoord_bullets_sprite.h"
#include "uvcoord_viper_sprite.h"
#include "uvcoord_r9_sprite.h"
#include "uvcoord_vic_sprite.h"
#include "uvcoord_explosions_sprite.h"

#include "uvcoord_font_si.h"

#include "uvcoord_enemies_sprite.h"
#include "uvcoord_centipede_head_sprite.h"
#include "uvcoord_centipede_segments_sprite.h"
#include "uvcoord_mmx_shot_sprite.h"
#include "uvcoord_transpormaboss_sprite.h"
#include "uvcoord_gluteus_boss_sprite.h"

// ------------ Bezier Path Generator --------------

#include "bezier_wave.h"
#include "bezier_loop.h"
#include "bezier_stopnback.h"
#include "bezier_circle.h"


// ------------ Level --------------

#include "simap_level_01.h"
#include "simap_level_02.h"
#include "simap_level_03.h"
#include "simap_level_04.h"
#include "simap_level_05.h"

// ------------ Constants --------------

	
// ------------ Easy GL2D Sprite/Tile Sheets --------------

// Player
static glImage ViperSpriteImages[VIPER_SPRITE_NUM_IMAGES];
static glImage R9SpriteImages[R9_SPRITE_NUM_IMAGES];
static glImage VicSpriteImages[VIC_SPRITE_NUM_IMAGES];
static glImage MmxShotImages[MMX_SHOT_SPRITE_NUM_IMAGES];

// Bullets Explosions
static glImage ExplosionsImages[EXPLOSIONS_SPRITE_NUM_IMAGES];
static glImage BulletsImages[BULLETS_SPRITE_NUM_IMAGES];

// Enemies
static glImage PowerUpImages[4];
static glImage BomberUpImages[4];
static glImage ShipUpImages[4];

static glImage EnemiesImages[ENEMIES_SPRITE_NUM_IMAGES];
static glImage BlobImages[4];
static glImage CentipedeHeadImages[CENTIPEDE_HEAD_SPRITE_NUM_IMAGES];
static glImage CentipedeSegmentsImages[CENTIPEDE_SEGMENTS_SPRITE_NUM_IMAGES];
static glImage TentakillBossImages[8];
static glImage TranspormaBossImages[TRANSPORMABOSS_SPRITE_NUM_IMAGES];
static glImage GluteusBossImages[GLUTEUS_BOSS_SPRITE_NUM_IMAGES];
static glImage MonkatBossImages[1];


// Fonts
static glImage SiFontGLImages[FONT_SI_NUM_IMAGES];
static glImage SiFontGL16x16Images[64];


static glImage WarpTile[1];
static glImage Cockpit[1];
static glImage Helix[1];


static glImage Stage02BG[(256/16)*(64/16)];
static glImage Stage03BG[(128/16)*(256/16)];
static glImage Stage04BG[(128/16)*(256/16)];


static glImage HbLogoImage[1];
static glImage GbaTempLogoImage[1];

static glImage FlaresImages[4];

// Textures
static int Mode7TextureID;
static int OrganTextureID; 

// ------------ Palettes --------------

// Player
static int ViperSpriteTexPal;
static int MmxShotTexPal;


// Enemies
static int PowerUpTexPal;

static int BlobTexPal;
static int CentipedeHeadTexPal;
static int CentipedeSegmentsTexPal;

// Font Palettes

// Bullets and Explosions

// Texture Palettes


// --------------- Game Objects -------------------

// --------------- Enemy Containers -------------------
static PowerUpContainer PowerUps;
static BomberUpContainer BomberUps;
static ShipUpContainer ShipUps;
static BonusCarrierContainer BonusCarriers;

static PopcornContainer Popcorns;
static RotatorContainer Rotators;
static BoogerContainer Boogers;
static LooperContainer Loopers;
static PlatoContainer Platos;
static CentipedeContainer Centipedes;
static ElectricBlobContainer ElectricBlobs;
static WormContainer Worms;
static MahornContainer Mahorns;
static BumbleBeeContainer BumbleBees;
static PhoenixContainer Phoenixs;
static DiverContainer Divers;
static AbtikBawiContainer AbtikBawis;
static TictanicContainer Tictanics;
static AlienatorContainer Alienators;
static CannoneerContainer Cannoneers;
static LightBallContainer LightBalls;
static BatteryContainer Batteries;
static AmoebaContainer Amoebas;
static KalaveraContainer Kalaveras;
static FrisbeeContainer Frisbees;

// --------------- Bosses -------------------

static PredatorBoss Predator;
static TentakillBoss Tentakill;
static TranspormaBoss Transporma;
static MonkatBoss Monkat;
static GluteusBoss Gluteus;

// --------------- Bullet Rel -------------------

// Components
BulletArray Bullets;
TurretArray Turrets;

// --------------- Ships -------------------

Viper Vic;
R9 R9x;
VicViper Gradius;
Bomb Bombs;


// --------------- Level -------------------

static Level Lvl;

// --------------- Fonts -------------------

static GLFont SiFontGL;
static GLFont SiFontGL16x16;
static OamFont SubFont;
static OamFont SubFont2;


// --------------- Memory -------------------

MemoryFeedback Memory;


// --------------- Sub BG Stuff -------------------

// Plasma BG
static int SubBG3;

// PlayerStat
Engine::HighScore PlayerStat = { "CYAN", "BLUE AGILA"  , 0, 0 };

Engine::HighScore Scores[10] = 
{
	{ "ANYA"      , "RED AGILA"  , 400000, 0 },  // 0
	{ "DR.D"      , "BLUE BAGWIS", 300000, 1 },  // 1
	{ "MAGAN"     , "LOCO VIPER"  , 200000, 0 },  // 2
	{ "MORIAH"    , "BLUE BAGWIS", 100000, 1 },  // 3
	{ "URZA"      , "BLUE BAGWIS", 90000 , 1 },  // 4
	{ "MARCUS"    , "LOCO VIPER"  ,  80000, 0 },  // 5
	{ "MAZOR"     , "RED AGILA"  ,  70000, 0 },  // 6
	{ "J-PLAYER"  , "BLUE BAGWIS",  60000, 1 },  // 7
	{ "CALEB"     , "LOCO VIPER"  ,  50000, 0 },  // 8
	{ "LILYTTE"   , "RED AGILA"  ,  40000, 0 },  // 9
};


Engine::HighScore BRScores[10] = 
{
	{ "LILYTTE"   , "BLUE BAGWIS"  , 300000, 0 },  // 0
	{ "MARCUS"    , "LOCO VIPER"    , 200000, 1 },  // 1
	{ "MORIAH"    , "RED AGILA"    , 100000, 0 },  // 2
	{ "J-PLAYER"  , "BLUE BAGWIS"  ,  90000, 1 },  // 3
	{ "CALEB"     , "LOCO VIPER"    ,  80000, 1 },  // 4
	{ "DR.D"      , "BLUE BAGWIS"  ,  70000, 0 },  // 5
	{ "MAZOR"     , "RED AGILA"    ,  60000, 0 },  // 6
	{ "MAGAN"     , "BLUE BAGWIS"  ,  50000, 1 },  // 7
	{ "URZA"      , "LOCO VIPER"    ,  40000, 0 },  // 8
	{ "ANYA"      , "RED AGILA"    ,  30000, 0 },  // 9
};


const char *Stages[] =
{ 
	"  SMEGMA  ",
	"  AXILLA  ",
	"  PUDENDA ",
	"  AREOLA  ",
	"  GLUTEUS "
	
};

const char *Bosses[] =
{ 
	" VENGADOR ",
	"TRANSPORMA",
	"TENTAKILL ",
	" TAKNOM  ",
	" MAXIMUS "
	
};

static bool AutoBomb = false;
static int BossTimer = 0;
static int CurrentHighScore = 0;
static int NumContinues = 0;		
static int LoopNumber = 0;	
static int FirstScore = 0;
static bool CheatOn = false;
static bool IsDemo = false;
bool IsReplay = false;

// Mode Specific Globals
static int FADE_BGM = ( 60 * 135 ) * 1 + 128;   // main final
static int START_BOSS = FADE_BGM + (64*4);
static int STOP_SPAWING = FADE_BGM - (64*5);

static int BOSS_DEATH_FRAME = START_BOSS + ( 60 * 120 );
static int START_COUNTDOWN = BOSS_DEATH_FRAME - (60 * 10);


int YesOrNo( int Frame, int y, const char *text, int Active );
int LoadReplay();
void SaveReplay();
void PrintScores( Engine::HighScore Scores[], int ScoreMode, int Frame, int CurrentPlayerIndex, int ColOffset );
void LoadPlayerName( char *PlayerName );
void SavePlayerName( char *PlayerName );
void NoIntermission( PlayerShip &Ship );
int GetKeyIndex( int ControlButton  );

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
Engine::Engine()
{
}

Engine::~Engine()
{
}


//--------------------------------------
//	
//--------------------------------------
void Engine::InitHardware()
{
	
	// Final VRAM Bank settings
	//A+B+C -> Texture
	//D -> SUB Sprite
	//E -> Texure Palette
	//H+I -> Sub BG
	//consoleDemoInit();
	
	videoSetMode( MODE_5_3D );
	videoSetModeSub( MODE_5_2D );
	
	vramSetBankA( VRAM_A_TEXTURE );     
	vramSetBankB( VRAM_B_TEXTURE );     
	vramSetBankC( VRAM_C_TEXTURE );     
	vramSetBankD( VRAM_D_SUB_SPRITE );	// oam fonts et al
	
	vramSetBankE( VRAM_E_TEX_PALETTE );		// 64kb
	
	vramSetBankH( VRAM_H_SUB_BG );		// Our sub BG  
	vramSetBankI( VRAM_I_SUB_BG_0x06208000 ); // H + I makes 256*192
	
	fatInitDefault();
	
	
}


//--------------------------------------
//	
//--------------------------------------
void Engine::InitEverything()
{
	
	// Initialize GL2D
    glScreen2D();
   
	// 2D FX
	Fx2D::Instance()->Init();
	
	// 3D FX
	Fx3D::Instance()->InitShapes();
	Fx3D::Instance()->InitStars();
	
	// Palettes
	Palettes::Instance()->Init();
	
	// Sound
	Sound::Instance()->Init( (mm_addr)soundbank_bin );
	Sound::Instance()->SetMode( MM_MODE_A );
	
	// Button
	Button::Init();
	
	// Random number
	UTIL::InitRND(42);
	
	
	// Bg
	SubBG3 = bgInitSub( 3, BgType_Bmp8, BgSize_B8_256x256, 0, 0 );
	
	
}

void Engine::InitSubScreen()
{
	
	
	
	// fade to black instantly to hide the loaded BG
	s32 BGInterp = 1 << 12;
	Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Fx2D::Instance()->GetPlasmaPal(),
							Palettes::Instance()->GetBlack(),
							BGInterp,
							16,
							512 );
	
	
	
	// Load new palette for the gameplay sub bg
	//u8* Plaz = Fx2D::Instance()->PlasmaPolar();
	
	u8* Plaz = Fx2D::Instance()->PlasmaStencil(si_ds_titleBitmap);
	DC_FlushRange(Plaz, SCREEN_WIDTH * SCREEN_HEIGHT);
    dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	
	// Change pal
	// Only load the last half of the palette so that we could use
	// the sub for both text and plasma
	Fx2D::Instance()->LoadPal( 128, 255, plasma_palPal );
	Fx2D::Instance()->LoadPal( 0, 127, si_ds_titlePal );
	
	
}
//--------------------------------------
//	
//--------------------------------------
void Engine::LoadSounds()
{
	
	// BGMs
	Sound::Instance()->LoadBGM( MOD_LEVEL01 );
	Sound::Instance()->LoadBGM( MOD_LEVEL02 );
	Sound::Instance()->LoadBGM( MOD_LEVEL03 );
	Sound::Instance()->LoadBGM( MOD_LEVEL04 );
	Sound::Instance()->LoadBGM( MOD_LEVEL05 );
	Sound::Instance()->LoadBGM( MOD_BOSS );
	Sound::Instance()->LoadBGM( MOD_CREDITS );
	Sound::Instance()->LoadBGM( MOD_STORY );
	Sound::Instance()->LoadBGM( MOD_ENDING );
	Sound::Instance()->LoadBGM( MOD_MENU );
	
	// SFXs
	Sound::Instance()->LoadSFX( SFX_SHOT );
	Sound::Instance()->LoadSFX( SFX_BOMB );
	Sound::Instance()->LoadSFX( SFX_POWERUP );
	Sound::Instance()->LoadSFX( SFX_VIPER_ENTER );
	Sound::Instance()->LoadSFX( SFX_VIPER_EXPLODE );
	Sound::Instance()->LoadSFX( SFX_EXP_BIG );
	Sound::Instance()->LoadSFX( SFX_EXP_MED2 );
	Sound::Instance()->LoadSFX( SFX_EXP_MED1 );
	Sound::Instance()->LoadSFX( SFX_EXP_SMALL );
	Sound::Instance()->LoadSFX( SFX_ENEMY_HIT );
	Sound::Instance()->LoadSFX( SFX_BOSS_HIT );
	Sound::Instance()->LoadSFX( SFX_MENU_CHANGE );
	Sound::Instance()->LoadSFX( SFX_MENU_SELECT );
	Sound::Instance()->LoadSFX( SFX_MENU_OK );
	
	
}
	

//--------------------------------------
//	
//--------------------------------------
void Engine::LoadFonts()
{
	
	// Oam Fonts
	// Oam
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	SubFont.Init(&oamSub, (u8*)font_oamTiles, 8, 8, SpriteSize_8x8, 64, 0);
	SubFont2.Init(&oamSub, (u8*)font_oam_bigTiles, 16, 16, SpriteSize_16x16, 64, 64);
	SubFont.LoadPalette( font_oamPal, SPRITE_PALETTE_SUB, 512 );
	
	// GLfonts
	SiFontGL.Load( SiFontGLImages,
				   FONT_SI_NUM_IMAGES,
				   font_si_texcoords,
				   GL_RGB256,
				   TEXTURE_SIZE_64,		
				   TEXTURE_SIZE_128,		
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, 
				   (u8*)font_siBitmap, 
				   256,
				   viper_spritePal );	 	
	

	SiFontGL16x16.Load( SiFontGL16x16Images,
				   16,
				   16,
				   16,
				   1024,
				   GL_RGB256,
				   TEXTURE_SIZE_16,		
				   TEXTURE_SIZE_1024,		
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, 
				   (u8*)font_si_16x16Bitmap, 
				   256,
				   viper_spritePal );	 	

	
}


//--------------------------------------
//	
//--------------------------------------
void Engine::LoadImages()
{
	
	// ------------------ Player -------------------
	
	glLoadSpriteSet( ViperSpriteImages,				// pointer to glImage array
					 VIPER_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 viper_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_64,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)viper_spritePal,		// Load our 256 color enemies palette
					 (u8*)viper_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	glLoadSpriteSet( R9SpriteImages,				// pointer to glImage array
					 R9_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 r9_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_64,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)viper_spritePal,		// Load our 256 color enemies palette
					 (u8*)r9_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	glLoadSpriteSet( VicSpriteImages,				// pointer to glImage array
					 VIC_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 vic_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_64,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)viper_spritePal,		// Load our 256 color enemies palette
					 (u8*)vic_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	
	// ------------------ Bullet Rel and Explosions -------------------
	
	
	glLoadSpriteSet( BulletsImages,				// pointer to glImage array
					 BULLETS_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 bullets_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_128,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_64,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)viper_spritePal,		// Load our 256 color enemies palette
					 (u8*)bullets_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	
	glLoadSpriteSet( ExplosionsImages,				// pointer to glImage array
					 EXPLOSIONS_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 explosions_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_256,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)viper_spritePal,		// Load our 256 color enemies palette
					 (u8*)explosions_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	glLoadSpriteSet( MmxShotImages,				// pointer to glImage array
					 MMX_SHOT_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 mmx_shot_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB16,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_64,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 16,					// Length of the palette to use (256 colors)
					 (u16*)mmx_shot_spritePal,		// Load our 256 color enemies palette
					 (u8*)mmx_shot_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	// ------------------ Enemies -------------------

	glLoadTileSet( PowerUpImages,	// pointer to glImage array
				   16,				// sprite width
				   16,				// sprite height
				   16,				// bitmap image width
				   64,				// bitmap image height
				   GL_RGB16,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_16,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   16,					// Length of the palette to use (256 colors)
				   (u16*)powerup_spritePal,		// Load our 256 color enemies palette
				   (u8*)powerup_spriteBitmap // image data generated by GRIT
				 );
	
	glLoadTileSet( BomberUpImages,	// pointer to glImage array
				   16,				// sprite width
				   16,				// sprite height
				   16,				// bitmap image width
				   64,				// bitmap image height
				   GL_RGB16,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_16,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   16,					// Length of the palette to use (256 colors)
				   (u16*)bomberup_spritePal,		// Load our 256 color enemies palette
				   (u8*)bomberup_spriteBitmap // image data generated by GRIT
				 );
	
	glLoadTileSet( ShipUpImages,	// pointer to glImage array
				   16,				// sprite width
				   16,				// sprite height
				   16,				// bitmap image width
				   64,				// bitmap image height
				   GL_RGB16,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_16,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   16,					// Length of the palette to use (256 colors)
				   (u16*)shipup_spritePal,		// Load our 256 color enemies palette
				   (u8*)shipup_spriteBitmap // image data generated by GRIT
				 );
	
	glLoadSpriteSet( EnemiesImages,				// pointer to glImage array
					 ENEMIES_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 enemies_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_256,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)viper_spritePal,		// Load our 256 color enemies palette
					 (u8*)enemies_spriteBitmap	 	// image data generated by GRIT
				   );

	glLoadTileSet( BlobImages,		// pointer to glImage array
				   32,				// sprite width
				   32,				// sprite height
				   32,				// bitmap image width
				   128,				// bitmap image height
				   GL_RGB16,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   16,					// Length of the palette to use (256 colors)
				   (u16*)blob_spritePal,		// Load our 256 color enemies palette
				   (u8*)blob_spriteBitmap // image data generated by GRIT
				 );
	
	
	glLoadSpriteSet( CentipedeHeadImages,				// pointer to glImage array
					 CENTIPEDE_HEAD_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 centipede_head_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB16,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_64,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_512,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 16,					// Length of the palette to use (256 colors)
					 (u16*)centipede_head_spritePal,		// Load our 256 color enemies palette
					 (u8*)centipede_head_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	glLoadTileSet( TentakillBossImages,		// pointer to glImage array
				   32,				// sprite width
				   32,				// sprite height
				   256,				// bitmap image width
				   32,				// bitmap image height
				   GL_RGB256,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   256,					// Length of the palette to use (256 colors)
				   (u16*)tentakillboss_spritePal,		// Load our 256 color enemies palette
				   (u8*)tentakillboss_spriteBitmap // image data generated by GRIT
				 );

	glLoadSpriteSet( TranspormaBossImages,				// pointer to glImage array
					 TRANSPORMABOSS_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 transpormaboss_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_128,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)transpormaboss_spritePal,		// Load our 256 color enemies palette
					 (u8*)transpormaboss_spriteBitmap	 	// image data generated by GRIT
				   );
	
	glLoadSpriteSet( GluteusBossImages,				// pointer to glImage array
					 GLUTEUS_BOSS_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 gluteus_boss_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_128,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)gluteus_boss_spritePal,		// Load our 256 color enemies palette
					 (u8*)gluteus_boss_spriteBitmap	 	// image data generated by GRIT
				   );
	
	glLoadTileSet( MonkatBossImages,				// pointer to glImage array
				   64,					// sprite width
				   64,					// sprite height
				   64,					// bitmap width
				   64,					// bitmap height
				   GL_RGB16,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_64,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   16,					// Length of the palette to use (256 colors)
				   (u16*)monkatboss_spritePal,		// Load our 256 color tiles palette
				   (u8*)monkatboss_spriteBitmap		// image data generated by GRIT
				 );
	
	glLoadSpriteSet( CentipedeSegmentsImages,				// pointer to glImage array
					 CENTIPEDE_SEGMENTS_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 centipede_segments_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB16,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_32,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_512,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 16,					// Length of the palette to use (256 colors)
					 (u16*)centipede_segments_spritePal,		// Load our 256 color enemies palette
					 (u8*)centipede_segments_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	// ------------------ Misc -------------------
	
	glLoadTileSet( WarpTile,				// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   32,					// bitmap width
				   32,					// bitmap height
				   GL_RGB16,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_32,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   16,					// Length of the palette to use (256 colors)
				   (u16*)bluecrossPal,		// Load our 256 color tiles palette
				   (u8*)bluecrossBitmap		// image data generated by GRIT
				 );
	

	glLoadTileSet( Cockpit,				// pointer to glImage array
				   256,					// sprite width
				   128,					// sprite height
				   256,					// bitmap width
				   128,					// bitmap height
				   GL_RGB16,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   16,					// Length of the palette to use (256 colors)
				   (u16*)cockpitPal,		// Load our 256 color tiles palette
				   (u8*)cockpitBitmap		// image data generated by GRIT
				 );

	
	glLoadTileSet( Helix,				// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   32,					// bitmap width
				   32,					// bitmap height
				   GL_RGB16,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_32,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   16,					// Length of the palette to use (256 colors)
				   (u16*)sepiacirclePal,		// Load our 256 color tiles palette
				   (u8*)sepiacircleBitmap		// image data generated by GRIT
				 );
	
	
	
	// -------------------------Level---------------------------------
	// 2d
	glLoadTileSet( Stage02BG,		// pointer to glImage array
				   16,				// sprite width
				   16,				// sprite height
				   256,				// bitmap image width
				   64,				// bitmap image height
				   GL_RGB256,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   256,
				   (u16*)stage02bgPal,
				   (u8*)stage02bgBitmap // image data generated by GRIT
				 );
	
	glLoadTileSet( Stage03BG,		// pointer to glImage array
				   16,				// sprite width
				   16,				// sprite height
				   128,				// bitmap image width
				   256,				// bitmap image height
				   GL_RGB256,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_256,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   256,
				   (u16*)stage03bgPal,
				   (u8*)stage03bgBitmap // image data generated by GRIT
				 );
	
	glLoadTileSet( Stage04BG,		// pointer to glImage array
				   16,				// sprite width
				   16,				// sprite height
				   128,				// bitmap image width
				   256,				// bitmap image height
				   GL_RGB256,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_256,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   256,
				   (u16*)viper_spritePal,
				   (u8*)stage04bgBitmap // image data generated by GRIT
				 );
	
	
	// 3d
	
	
	// -------------------------particles---------------------------------
	glLoadTileSet( FlaresImages,		// pointer to glImage array
				   32,				// sprite width
				   32,				// sprite height
				   32,				// bitmap image width
				   128,				// bitmap image height
				   GL_RGB256,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   256,
				   (u16*)flares_spritePal,
				   (u8*)flares_spriteBitmap // image data generated by GRIT
				 );
	
}

	
//--------------------------------------
//	
//--------------------------------------
void Engine::Load3D()
{
	
	
	Mode7TextureID = Fx3D::Instance()->LoadGrid( GL_RGB256,
						 TEXTURE_SIZE_128, 
						 TEXTURE_SIZE_128, 
						 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T,
						 (u8*)mode7Bitmap,
						 256,
						 mode7Pal
					   );
	
	
	OrganTextureID = Fx3D::Instance()->LoadAscaris( GL_RGB256,
						 TEXTURE_SIZE_128, 
						 TEXTURE_SIZE_128, 
						 GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T,
						 (u8*)organBitmap,
						 256,
						 organPal
					   );

	Fx3D::Instance()->LoadShapes( GL_RGB256,
						 OrganTextureID );
	
}
	
	


//--------------------------------------
//	
//--------------------------------------
void Engine::GameDoSubScreen( int Frame,
							  int Stage,
							  const PlayerShip &Ship,
							  s32 &BGInterp, 
							  bool FadeOut,
							  bool RotatePal )
{
	
	
	
	SubFont.Clear(); // deactivate every oam per frame
	SubFont2.Clear(); // deactivate every oam per frame
	
	//void PrintSine(int x, int y, const char *text, int width_offset, int _height, int cycles, s32 start_angle);	
	
	SubFont2.Print( 0, 4, "HI-SCORE: " );
	SubFont2.PrintScore( 9*16, 4, CurrentHighScore, "0", 7 );
	
	SubFont2.Print( 0, 34, "   SCORE: " );
	SubFont2.PrintScore( 9*16, 34, Ship.GetScore(), "0", 7 );
	
	int h = (sinLerp(Frame*16) * 4) >> 12;
	
	SubFont.PrintCentered( 30, 34 + 20 * 2, "GAME MODE" );
	if( Button::Replay.Mode == 0 )
		SubFont2.PrintSine( 16 * 1 + 8, 34 + 20 * 3, "NORMAL ATTACK", 0, h, 3, Frame * 500 );
	else
		SubFont2.PrintSine( 16 * 0 + 8, 34 + 20 * 3, "   BOSS RUSH", 0, h, 3, Frame * 500 );
		
	
	// Draw
	oamUpdate(&oamSub);
	
	s32 SprInterp = BGInterp;
	if( !FadeOut )
	{
		// fade from black sub bg
		Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								16,
								512
							  );
		
		// fade from black sub OAM
		Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								viper_spritePal,
								SprInterp,
								32,
								512
							  );
		
	}
	else
	{
		// fade to black sub bg
		Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								-16,
								512
							  );
		
		// fade to black sub OAM
		Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								viper_spritePal,
								SprInterp,
								-16,
								512
							  );
		
	}
	
	
	// Animate the plazma!
	if( RotatePal )
	{
		Fx2D::Instance()->CyclePlasmaPal( 128, 255 );
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Engine::GameInitAllEntities( std::vector<PlayerShip*> &Players,
								  Bomb &Bombs,
								  BulletArray &Bullets,
								  TurretArray &Turrets,
								  std::vector<EntityContainer*> &Enemies,
								  std::vector<Boss*> &Bosses )
{

	// Ships
	Vic.Init( ViperSpriteTexPal, ViperSpriteImages );
	R9x.Init( ViperSpriteTexPal, R9SpriteImages );
	Gradius.Init( ViperSpriteTexPal, VicSpriteImages );
	
	
	Players.push_back( &Vic );
	Players.push_back( &R9x );
	Players.push_back( &Gradius );
	
	// Bosses
	Predator.Init( ViperSpriteTexPal, EnemiesImages, 0 );
	Transporma.Init( BlobTexPal, TranspormaBossImages, 1 );
	Tentakill.Init( BlobTexPal, TentakillBossImages, ViperSpriteTexPal, BulletsImages, 2 );
	Gluteus.Init( ViperSpriteTexPal, GluteusBossImages, 0 );
	Monkat.Init( ViperSpriteTexPal, MonkatBossImages, ViperSpriteTexPal, ExplosionsImages, 3 );
	
	Bosses.push_back( &Predator );
	Bosses.push_back( &Transporma );
	Bosses.push_back( &Tentakill );
	Bosses.push_back( &Monkat );
	Bosses.push_back( &Gluteus );
	
	
	// Enemy Containers
	
	PowerUps.Init( PowerUpTexPal, PowerUpImages );
	BomberUps.Init( PowerUpTexPal, BomberUpImages );
	ShipUps.Init( PowerUpTexPal, ShipUpImages );
	BonusCarriers.Init( ViperSpriteTexPal, EnemiesImages );
	
	Popcorns.Init( ViperSpriteTexPal, EnemiesImages );
	Rotators.Init( ViperSpriteTexPal, EnemiesImages );
	Boogers.Init( BlobTexPal, BlobImages );
	Loopers.Init( ViperSpriteTexPal, EnemiesImages );
	Platos.Init( ViperSpriteTexPal, EnemiesImages );
	Centipedes.Init( CentipedeHeadTexPal, CentipedeHeadImages,
					 CentipedeSegmentsTexPal, CentipedeSegmentsImages);
	ElectricBlobs.Init( ViperSpriteTexPal, EnemiesImages );
	Worms.Init( ViperSpriteTexPal, EnemiesImages );
	Mahorns.Init( ViperSpriteTexPal, EnemiesImages );
	BumbleBees.Init( ViperSpriteTexPal, EnemiesImages );
	Phoenixs.Init( ViperSpriteTexPal, EnemiesImages );
	Divers.Init( ViperSpriteTexPal, EnemiesImages );
	AbtikBawis.Init( ViperSpriteTexPal, EnemiesImages );
	Tictanics.Init( ViperSpriteTexPal, EnemiesImages );
	Alienators.Init( ViperSpriteTexPal, EnemiesImages );
	Cannoneers.Init( ViperSpriteTexPal, EnemiesImages );
	LightBalls.Init( ViperSpriteTexPal, EnemiesImages );
	Batteries.Init( ViperSpriteTexPal, EnemiesImages );
	Amoebas.Init( ViperSpriteTexPal, EnemiesImages );
	Kalaveras.Init( ViperSpriteTexPal, EnemiesImages );
	Frisbees.Init( ViperSpriteTexPal, EnemiesImages );
	
	
	
	// Order of drawing is from 1st to last push_back()
	Enemies.push_back( &Centipedes );
	Enemies.push_back( &Rotators );
	Enemies.push_back( &Boogers );
	Enemies.push_back( &Mahorns );
	Enemies.push_back( &BumbleBees );
	Enemies.push_back( &Alienators );
	Enemies.push_back( &Cannoneers );
	Enemies.push_back( &Phoenixs );
	Enemies.push_back( &Tictanics );
	Enemies.push_back( &LightBalls );
	Enemies.push_back( &Batteries );
	Enemies.push_back( &AbtikBawis );
	Enemies.push_back( &Loopers );
	Enemies.push_back( &Worms );
	Enemies.push_back( &ElectricBlobs );
	Enemies.push_back( &Frisbees );
	Enemies.push_back( &Platos );
	Enemies.push_back( &Popcorns );
	Enemies.push_back( &Amoebas );
	Enemies.push_back( &Kalaveras );
	Enemies.push_back( &Divers );
	Enemies.push_back( &BonusCarriers );
	Enemies.push_back( &PowerUps );
	Enemies.push_back( &BomberUps );
	Enemies.push_back( &ShipUps );
	
	// Bullet Rel
	Bullets.Init( ViperSpriteTexPal, BulletsImages, 512 );
	Turrets.Init( ViperSpriteTexPal, BulletsImages, 32 );
	Bombs.Init( MmxShotTexPal, MmxShotImages, FlaresImages, GluteusBossImages );

}

//--------------------------------------
//	
//--------------------------------------
int GetRevengeBulletParam( Bullet::Params &Bp, int Mode )
{
	int NumBullets = 0;
	switch( Mode )
	{
		case 1:
			Bp.Gx = 0;
			Bp.Gy = 0;
			Bp.Speed = floattof32(1.75);
			Bp.RotationDelta = 256;
			Bp.StopDelay = -20;
			Bp.Frame = 0;
			Bp.NumFrames = 4;
			Bp.BaseFrame = 20;
			Bp.Smart = false;
			Bp.Width = 12;
			Bp.Height = 12;
			Bp.Type = Bullet::NORMAL;
			Bp.AniMode = Bullet::DEFAULT;
			NumBullets = 1;
			break;
		case 2:
			Bp.Gx = 0;
			Bp.Gy = 0;
			Bp.Speed = floattof32(1.75);
			Bp.RotationDelta = 256;
			Bp.StopDelay = -20;
			Bp.Frame = 0;
			Bp.NumFrames = 3;
			Bp.BaseFrame = 34;
			Bp.Smart = false;
			Bp.Width = 8;
			Bp.Height = 8;
			Bp.Type = Bullet::NORMAL;
			Bp.AniMode = Bullet::ORIENTED;
			NumBullets = 3;
			break;
		case 3:
			Bp.Gx = 0;
			Bp.Gy = 0;
			Bp.Speed = floattof32(0.75)/8;
			Bp.RotationDelta = 256;
			Bp.StopDelay = -20;
			Bp.Frame = 0;
			Bp.NumFrames = 1;
			Bp.BaseFrame = 24;
			Bp.Smart = false;
			Bp.Width = 8;
			Bp.Height = 8;
			Bp.Type = Bullet::ACCEL;
			Bp.AniMode = Bullet::ORIENTED;
			NumBullets = 5;
			break;
		case 99:
			Bp.Gx = 0;
			Bp.Gy = 0;
			Bp.Speed = floattof32(0.75)/4;
			Bp.RotationDelta = 256;
			Bp.StopDelay = -20;
			Bp.Frame = 0;
			Bp.NumFrames = 3;
			Bp.BaseFrame = 25;
			Bp.Smart = false;
			Bp.Width = 8;
			Bp.Height = 8;
			Bp.Type = Bullet::ACCEL;
			Bp.AniMode = Bullet::ORIENTED;
			NumBullets = 25;
			break;
	}
	
	return NumBullets;
}

//--------------------------------------
//	
//--------------------------------------
void Engine::GameUpdateAll( int Frame,
							int KeyH,
							int KeyD,
							int Stage,
							bool &StageFinished,
							Level &Lvl,
							PlayerShip &Ship,
							Bomb &Bombs,
							BulletArray &Bullets,
							TurretArray &Turrets,
							PowerUpContainer &PowerUps,
							std::vector<EntityContainer*> &Enemies,
							Boss &Bossing )
{
  
	// Level
	Lvl.Update( Stage, Frame );
	// Player
	
	if( Ship.IsActive() ) 
	{
		if( Ship.GetControlType() == 0 )
			StageFinished = Ship.Update( KeyH, KeyD, Bombs );
		else
			StageFinished = Ship.Replay( Frame, Bombs, Stage + (LoopNumber * 5) );
		
		// Dont throw a bomb
		Ship.SetThrowBomb(false);
		
		// Limit Score
		if( CheatOn )
		{
			if( Ship.GetScore() > 50000 )
			{
				Ship.SetScore(50000);
			}
		}
		else
		{
			if( Ship.GetScore() > 9999999 )
			{
				Ship.SetScore(9999999);
			}
		}
		
		//if( FirstScore > Button::Replay.MaxScore )
		//{
		//	Button::Replay.MaxScore = FirstScore;
		//}
		
		if( !Ship.IsAlive() )
		{
			if( Ship.GetLives() > -1 )
			{
				Ship.DecNewLifeCounter();
				if( Ship.GetNewLifeCounter() == 0 )
				{
					Ship.NewLife();
				}
		
			}
			else
			{
				//GameOver = true;
				//StageFinished = true;
			}
		
		}
		
	}
	
	
	// Reset Targets
	Targets::Instance()->Reset();
	
	EntityContainer::CollisionPacket Ret;
	std::vector<EntityContainer*>::iterator iter;
		
	// Enemies	
	if( LoopNumber == 0 )
	{
		for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
		{
			bool IsDestroyed = false;
			int SprayMode = 0;
			(*iter)->UpdateEntities( Ship );
			Ret = (*iter)->Collide( Ship, Bombs.IsActive(), IsDestroyed, SprayMode );
			if( Ret.Collision > 0 )
			{
				for( int i = 0; i < Ret.Collision; i++)
				{
					PowerUps.Spawn( Ret.x, Ret.y );
				}
			}
		}
		
	}
	else
	{
		for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
		{
			bool IsDestroyed = false;
			int SprayMode = 0;
			(*iter)->UpdateEntities( Ship );
			Ret = (*iter)->Collide( Ship, Bombs.IsActive(), IsDestroyed, SprayMode );
			if( Ret.Collision > 0 )
			{
				for( int i = 0; i < Ret.Collision; i++)
				{
					PowerUps.Spawn( Ret.x, Ret.y );
				}
			}
			
			if( (IsDestroyed) && (SprayMode > 0) )
			{
				Bullet::Params Bp;
				int NumBullets = GetRevengeBulletParam( Bp, SprayMode );
				
				Bullets.SpawnBullets( Bp,
									  Ret.x << 12,
									  Ret.y << 12,
									  NumBullets,		// BulletsPerShot
									  BRAD_PI/16,		// SpreadAngle
									  0,
									  Ship.GetPosition() );
		
			}
			
		}
	}
	
	
	// boss
	if( Bossing.IsActive() )
	{
		
		bool BossDestroyed = false;
		
		Bossing.Update( Ship );
		s32 BossX = Bossing.GetX();
		s32 BossY = Bossing.GetY();
		
		Ret = Bossing.Collide( Ship, Bombs.IsActive(), BossDestroyed );
		if( Ret.Collision > 0 )
		{
			for( int i = 0; i < Ret.Collision; i++)
			{
				PowerUps.Spawn( Ret.x, Ret.y );
			}
		}
		
		if( BossDestroyed )
		{
			Bullets.KillEntities();
			Turrets.KillEntities();
			
			// revenge if loop 2
			if( LoopNumber > 0 )
			{

				Bullet::Params Bp;
				int NumBullets = GetRevengeBulletParam( Bp, 99 );
				for( int i = 0; i < 5; i++ )
				{
					int j = (i+1) & 3;
					Bp.BaseFrame = 25 + (j * 3);
					Bullets.SpawnBullets( Bp,
										  BossX,
										  BossY,
										  NumBullets*(i+1),		// BulletsPerShot
										  (i+1)*650,		// SpreadAngle
										  i * floattof32(0.15),
										  Ship.GetPosition() );
				}
				
			}
		}
		
		// Kill boss if over the frame limit
		if( Frame == BOSS_DEATH_FRAME ) 
		{
			Bullets.KillEntities();
			Turrets.KillEntities();
			
			Bullet::Params Bp;
			int NumBullets = GetRevengeBulletParam( Bp, 99 );
			for( int i = 0; i < 5; i++ )
			{
				int j = (i+1) & 3;
				Bp.BaseFrame = 25 + (j * 3);
				Bullets.SpawnBullets( Bp,
									  Bossing.GetX(),
									  Bossing.GetY(),
									  NumBullets*(i+1),		// BulletsPerShot
									  (i+1)*650,		// SpreadAngle
									  i * floattof32(0.15),
									  Ship.GetPosition() );
			}

			Bossing.ForceDestruction();
			
		}
		
	}
		
	// Set new player Targets for homing missiles
	Ship.SetTargets();
	
	// Only Shoot when there is no bomb
	if( !Bombs.IsActive() ) Turrets.UpdateEntities( Bullets );

	Bullets.UpdateEntities();
	
	// Kill Bullets on screen when bombing
	if( Bombs.IsActive() )
	{
		// bullets
		Bullets.KillEntities();
		
		// enemies
		for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
		{
			Ret = (*iter)->BombEntities( Ship, 1 );
			if( Ret.Collision > 0 )
			{
				for( int i = 0; i < Ret.Collision; i++)
				{
					PowerUps.Spawn( Ret.x, Ret.y );
				}
			}
		}
		
		// boss
		if( (Bossing.GetX() >> 12) < (256) )
			Bossing.BombMe( Ship, 1 );
		
	}

	
	// Collide Player with bullets
	Ret = Bullets.Collide( Ship );
	if( Ret.Collision > 0 )
	{
		for( int i = 0; i < Ret.Collision; i++)
		{
			PowerUps.Spawn( Ret.x, Ret.y );
		}
	}
	
	// Do the explosions
	Explosion::Instance()->Update();

	// Update bomb
	Bombs.Update( Ship.GetID() );
	
	// Blink enemies/boss when bomb is active
	if( Bombs.IsActive() )
	{
		
		if(( Frame & 3) == 0 )
		{
			for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
			{
				(*iter)->SetDrawMode( EN_WHITE );
			}
			
			Bossing.SetDrawMode( EN_WHITE );
			
		}
		
	}
	
	// HighScore
	if( Ship.GetScore() > CurrentHighScore )
		CurrentHighScore = Ship.GetScore();
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::GameDrawAll( int Frame,
						  int Stage,
						  bool FadeOut,
						  s32 &FadeInterp,
						  s32 &BGInterp,
						  s32 &FontInterp,
						  Level &Lvl,
						  const PlayerShip &Ship,
						  const Bomb &Bombs,
						  BulletArray &Bullets,
						  std::vector<EntityContainer*> &Enemies,
						  Boss &Bossing,
						  bool DrawAABB,
						  bool DrawDebug,
						  bool IsPaused,
						  bool PressStart,
						  bool GameOver,
						  int CanPressStartCounter,
						  int CanContinueCounter,
						  bool &PrintGameOver,
						  int &YorNActive )
{
	
	
	static bool PrintDemo = true;
	
	// Give the darn 3D layer a different ID so that bombs would be rendered fine
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	Lvl.Draw3D(Stage, Frame );		
	
	glBegin2D();
	
		glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		Lvl.Draw2D(Stage, Frame );
		
		// Same with all the GL2D sprites
		glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
		// Boss
		if( Bossing.IsActive() ) Bossing.Draw();
		
		// draw all enemies
		std::vector<EntityContainer*>::iterator iter;
		for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
		{
			(*iter)->DrawEntities();
			if( DrawAABB ) (*iter)->DrawAABB( RGB15(31,0,31) );
			
		}

		// Draw aura if invincible
		if( Ship.IsInvincible() || Bombs.IsActive() )
		{
			int auracolor = (abs(sinLerp(Frame*512))*31) >> 12;
			
			glColor(RGB15(auracolor+15,auracolor,31-auracolor));
			glSpriteRotateScale( (Ship.GetX() >> 12), 
								 (Ship.GetY() >> 12), 
								 0, 
								 abs(sinLerp(Frame * 700)), 
								 GL_FLIP_NONE, 
								 &FlaresImages[0] );
			
			glColor(RGB15(auracolor,31-auracolor,auracolor*2));
			glSpriteRotateScale( (Ship.GetX() >> 12), 
								 (Ship.GetY() >> 12), 
								 0, 
								 2048 + abs(sinLerp(Frame * 400)/2), 
								 GL_FLIP_NONE, 
								 &FlaresImages[3] );
			
		}
		
		glColor(RGB15(31,31,31));
		if( Ship.GetID() < 2 )
		{
			Ship.Draw();
			Ship.DrawShots();
		}
		else
		{
			Ship.DrawShots();
			Ship.Draw();
		}
		
		Bullets.DrawEntities();
		
		if( DrawAABB ) Bullets.DrawAABB( RGB15(31,31,31) );
		
		
		Explosion::Instance()->Draw();
		
		if( DrawAABB ) Ship.DrawAABB( RGB15(0,31,0) );
		
		if( DrawAABB ) Bossing.DrawAABB( RGB15(0,0,31) );
		
		glEnable( GL_BLEND );
		
		// Bombs are drawn translucently
		Bombs.Draw( Ship.GetID() );
		
		// Restore to non-translucent
		glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		
		if( PressStart )
		{
			glColor( RGB15(31,31,31) );
			SiFontGL16x16.PrintCentered( 0, 70, "PRESS START");
		}
		
		
		// Draw Hud
		glColor( RGB15(31,31,31) );
		SiFontGL.Print( 0, 0, "SHIPS:");
		SiFontGL.Print( 128, 0, "BOMBS:");
		
		int ShipLeft = Ship.GetLives();
		int bombs = Ship.GetBombs();
		
		if( ShipLeft >= 0 )
		{
			if( ShipLeft < 9 )
			{
				SiFontGL.PrintAscii( 8*6, 0, 65+31, ShipLeft );
			}
			else
			{
				SiFontGL.PrintAscii( 8*6, 0, 65+31, 1);  // ship
				SiFontGL.PrintAscii( 8*7, 0, 78+31, 1);  // "x"
				SiFontGL.Print( 8*8, 0, ShipLeft );      // number
			}
		}
		if( bombs >= 0 )
		{
			if( bombs < 9 )
			{
				SiFontGL.PrintAscii( 128+8*6, 0, 66+31, bombs );
			}
			else
			{
				SiFontGL.PrintAscii( 128+8*6, 0, 66+31, 1 );
				SiFontGL.PrintAscii( 128+8*7, 0, 78+31, 1);
				SiFontGL.Print( 128+8*8, 0, bombs );
			}
		}
		
		// Loop-Stage
		SiFontGL.Print( 226, 192-10, LoopNumber+1 );
		SiFontGL.Print( 236, 192-10, "-" );
		SiFontGL.Print( 246, 192-10, (Stage % 5) + 1 );
		
		// LockOn
		int LockFrame = 0;
		if( (Frame > START_BOSS) && (Frame < START_BOSS + 512) )
		{
			 if( (Frame & 3) == 0 )
			{
				LockFrame++;
				if( LockFrame > 2 ) LockFrame = 0;
			}
			
			if( (Frame & 7) )
				glColor(RGB15(31,31,31));
			else
				glColor(RGB15(0,0,0));

			glSprite( (Bossing.GetX() >> 12) - 16, 
					  (Bossing.GetY() >> 12) - 16, 
					  GL_FLIP_NONE, 
					  &GluteusBossImages[LockFrame+1] );
			
			glColor(RGB15(31,31,31));
			
		}
		
		// Enter da impakto!
		if( Frame < 512 )
		{
				
			glColor( RGB15(31,31,31) );
		
			if( Frame < 256 )
			{
				if( Frame == 1 ) FontInterp = 0;
				
				int color = abs(sinLerp(Frame * 200) * 31) >> 12;
				glColor(RGB15(31,color,color));
				
				FontInterp += 64;
				if( FontInterp > 4096 )  FontInterp = 4096;
				int x = UTIL::Lerpf32( 300 << 12, 48 << 12, FontInterp ) >> 12;
				int y = UTIL::Lerpf32( 200 << 12, 76 << 12, FontInterp ) >> 12;
				int spacing = UTIL::Lerpf32( 7 << 12, 0 << 12, FontInterp );
				SiFontGL16x16.PrintSpaced( x, y, Stages[Stage], 0, -spacing , -spacing );
			}
			else
			{
				
				int color = abs(sinLerp(Frame * 200) * 31) >> 12;
				glColor(RGB15(color,31,color));
				
				if( Frame == 256 ) FontInterp = 0;
				FontInterp += 64;
				if( FontInterp > 4096 )  FontInterp = 4096;
				int x = UTIL::Lerpf32( 48 << 12, -200 << 12,FontInterp ) >> 12;
				int y = UTIL::Lerpf32( 76 << 12, -16 << 12,FontInterp ) >> 12;
				int spacing = UTIL::Lerpf32( 0 << 12, 7 << 12, FontInterp );
				SiFontGL16x16.PrintSpaced( x, y, Stages[Stage], 0, -spacing , spacing );					
			}

			
		}
		
		
		glColor( RGB15(31,31,31) );
		
		// Boss warning
		if( (Frame >= FADE_BGM) && (FADE_BGM + 512) )
		{
			if( Frame == FADE_BGM )
			{
				FontInterp = 0;
			}
			
			if( Frame < (FADE_BGM + 256) )
			{
				
				int color = abs(sinLerp(Frame * 200) * 31) >> 12;
				glColor(RGB15(color,color,31-color));
				
				FontInterp += 64;
				if( FontInterp > 4096 )  FontInterp = 4096;
				int x = UTIL::Lerpf32( 300 << 12, 48 << 12, FontInterp ) >> 12;
				int y = UTIL::Lerpf32( 200 << 12, 76 << 12, FontInterp ) >> 12;
				int spacing = UTIL::Lerpf32( 7 << 12, 0 << 12, FontInterp );
				SiFontGL16x16.PrintSpaced( x, y, Bosses[Stage], 0, -spacing , spacing );
			}
			else
			{
				int color = abs(sinLerp(Frame * 200) * 31) >> 12;
				glColor(RGB15(color,color,31-color));
				
				if( Frame == (FADE_BGM + 256) ) FontInterp = 0;
				FontInterp += 64;
				if( FontInterp > 4096 )  FontInterp = 4096;
				int x = UTIL::Lerpf32( 48 << 12, -200 << 12,FontInterp ) >> 12;
				int y = UTIL::Lerpf32( 76 << 12, -16 << 12,FontInterp ) >> 12;
				int spacing = UTIL::Lerpf32( 0 << 12, 7 << 12, FontInterp );
				SiFontGL16x16.PrintSpaced( x, y, Bosses[Stage], 0, -spacing , -spacing );					
			}
		}
		
		
		if( GameOver && (CanPressStartCounter <= 0))
		{
			if( PrintGameOver )
			{
			glColor( RGB15(0,31,0) );
			
				SiFontGL16x16.PrintCentered( 0, 76, "GAME OVER");
			}
			else
			{
				if( CanContinueCounter > 0 )
				{
					glColor( RGB15(31,31,31) );
					SiFontGL.PrintCentered( 0, 66, "PRESS START");
					SiFontGL.PrintCentered( 0, 86, "TO CONTINUE");
				}
				else
				{
					glColor( RGB15(31,31,0) );
					SiFontGL.PrintCentered( 0, 46, "PRESS START");
					SiFontGL.PrintCentered( 0, 66, "TO RETURN");
					SiFontGL.PrintCentered( 0, 86, "TO TITLE");
				}
				
			}
			
			if( (Frame & 31) == 0 ) PrintGameOver = !PrintGameOver;
			
			glColor( RGB15(31,31,31) );
			if( CanContinueCounter >= 0 )
				SiFontGL16x16.PrintCentered( 0, 106, CanContinueCounter/60);
			 
		}
		
		if( IsDemo )
		{
			glColor( RGB15(31,31,31) );
			if( (Frame & 63) == 0 ) PrintDemo = !PrintDemo;
			if( PrintDemo ) 
				SiFontGL.Print( 0, 192-10, "DEMO");
			else
				SiFontGL.Print( 0, 192-10, "PRESS START TO END");
		}
		else
		{
			if( IsReplay )
			{
				glColor( RGB15(31,31,31) );
				if( (Frame & 63) == 0 ) PrintDemo = !PrintDemo;
				if( PrintDemo ) 
					SiFontGL.Print( 0, 192-10, "REPLAY");
			}
		}
		
		//SiFontGL.Print( 0, 192-10, "FRAME:", Frame );
		//SiFontGL.Print( 0, 192-20, "SECONDS:", Frame/60 );
		//SiFontGL.Print( 0, 192-20, "SHIP AUTOBOMB:", Ship.IsAutoBomb() );
		//SiFontGL.Print( 0, 192-10, "BUTTON AUTOBOMB:", Button::Replay.AutoBomb );
		
		
		
		if( !IsPaused )
		{
			if( (!GameOver) && (Bossing.IsActive()) )
			{
				if( Frame >= START_COUNTDOWN )
				{
					BossTimer++;
					if( BossTimer < 60*10 )
						SiFontGL16x16.PrintCentered( 0, 60, (60*10-BossTimer)/60 );
				
				}
			}
		}
		else // paused
		{
			YorNActive = YesOrNo( Frame, 120, "RETURN TO MENU?", YorNActive );
		}
		
		
		// Fade
		if( FadeOut )
		{
			int R = 0;
			int G = 0;
			int B = 0;
			
			int Color1 = RGB15(R,G,B);
			int Color2 = RGB15(B,R,G);
			int Color3 = RGB15(G,B,R);
			int Color4 = RGB15(R,G,B);
	
			Fx3D::Instance()->Fade( Color1,
									Color2,
									Color3,
									Color4,
									FadeInterp,
									16 );
	

		}
	
		// Draw debugs
		if( DrawDebug )
		{
			GameDrawDebug( Frame,
						   Stage,
						   Ship,
						   Bombs,
						   Bullets,
						   Enemies,
						   Bossing );
		}
		
		
	
	glEnd2D();

	glFlush(0);                    

	
	swiWaitForVBlank();
	
	GameDoSubScreen( Frame, Stage, Ship, BGInterp, FadeOut, !DrawDebug );
	
	
	
}

void Engine::GameDrawDebug( int Frame,
							int Stage,
							const PlayerShip &Ship,
							const Bomb &Bombs,
							BulletArray &Bullets,
							std::vector<EntityContainer*> &Enemies,
							Boss &Bossing )
{

//	const char *EnemyNames[] =
//	{ 
//		"ROTATORS   = ",
//		"POPCORNS   = ",
//		"BOOGERS    = ",
//		"LOOPERS    = ",
//		"PLATOS     = ",
//		"CENTIPEDES = ",
//		"POWERUPS   = ",
//		"BONUSCR    = ",
//		"FILLER     = "
//	};
	
	

	// calculate the amount of 
	// memory uploaded to VRAM in KB
	int TextureSize = viper_spriteBitmapLen +
					  r9_spriteBitmapLen +
					  vic_spriteBitmapLen +
					  bullets_spriteBitmapLen +
					  explosions_spriteBitmapLen +
					  mmx_shot_spriteBitmapLen +
					  powerup_spriteBitmapLen +
					  bomberup_spriteBitmapLen +
					  shipup_spriteBitmapLen +
					  mode7BitmapLen +
					  organBitmapLen +
					  stage02bgBitmapLen +
					  stage03bgBitmapLen +
					  hblogoBitmapLen +
					  gbatemplogoBitmapLen +
					  flares_spriteBitmapLen +
					  enemies_spriteBitmapLen +
					  centipede_segments_spriteBitmapLen +
					  centipede_head_spriteBitmapLen +
					  blob_spriteBitmapLen +
					  tentakillboss_spriteBitmapLen +
					  transpormaboss_spriteBitmapLen +
					  gluteus_boss_spriteBitmapLen +
					  monkatboss_spriteBitmapLen +
					  font_si_16x16BitmapLen +
					  sepiacircleBitmapLen +
					  bluecrossBitmapLen +
					  cockpitBitmapLen +
					  font_siBitmapLen;


	SiFontGL.Print( 00, 1 * 10, "FRAMES = ", Frame );				  
	SiFontGL.Print( 00, 2 * 10, "TOTAL TEXTURE SIZE(KB) = ", TextureSize / 1024 );				  
	SiFontGL.Print( 00, 3 * 10, "MEM USED = ", Memory.GetMemUsed()/1024 );				  
	SiFontGL.Print( 00, 4 * 10, "MEM LEFT = ", Memory.GetMemFree()/1024 );				  
	SiFontGL.Print( 00, 5 * 10, "BULLETS = ", Bullets.NumEntities() );				  
	SiFontGL.Print( 00, 6 * 10, "TURRETS = ", Turrets.NumEntities() );				  
	SiFontGL.Print( 00, 7 * 10, "STAGE  = ", Stage );				  
	SiFontGL.Print( 00, 8 * 10, "MODE = ", Button::Replay.Mode );				  
	SiFontGL.Print( 00, 9 * 10, "MAXSCORE = ", Button::Replay.MaxScore );				  
	SiFontGL.Print( 00, 10 * 10, "FIRST SCORE = ", FirstScore );				  
	SiFontGL.Print( 00, 11 * 10, "REPLAY STAGE = ", LoopNumber * 5 + Stage );				  
	
	
	SiFontGL.Print( 00, 12 * 10, "SHIP = ", Button::Replay.ShipType );	
	
//	int i = 0;
//	int column = 0;
//	int row = 9 * 10;
//	
//	std::vector<EntityContainer*>::iterator iter;
//	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
//	{
//		SiFontGL.Print( column, row, EnemyNames[i], (*iter)->NumEntities() );				  
//		i++;
//		row += 10;
//		if(i == 10)
//		{
//			column = 128;
//			row = 40;
//		}
//	}
	
}
							
//--------------------------------------
//	
//--------------------------------------
int  Engine::GamePause( int Frame,
					    int Stage,
					    bool FadeOut,
						s32 &FadeInterp,
						s32 &BGInterp,
						s32 FontInterp,
						Level &Lvl,
					    const PlayerShip &Ship,
						BulletArray &Bullets,
				        const Bomb &Bombs,
				        std::vector<EntityContainer*> &Enemies,
						Boss &Bossing )
{
  
	Sound::Instance()->SetMasterVolume( 256 );
	
	int Active = 0;      // NO
	
	bool PrintGameOver = false;
	bool IsPaused = true;
	bool PressStart = true;
	int frames = 0;
	while( IsPaused )
	{
		frames++;
		scanKeys();
		int KD = keysDown();

		if( (KD & KEY_START) || (KD & KEY_A) ) 
		{
			IsPaused = false;
			
		}
		
		if( (KD & KEY_RIGHT) )
		{
			Active = (Active+1) & 1;
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		if( (KD & KEY_LEFT) )
		{
			Active = (Active-1) & 1;
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		if( (frames & 31) == 0 )PressStart = !PressStart;
		
		
		GlobalManager::Instance()->ResetAll();
			
		GameDrawAll( Frame,
					 Stage,
					 FadeOut,
					 FadeInterp,
					 BGInterp,
					 FontInterp,
					 Lvl,
					 Ship,
				     Bombs,
				     Bullets,
				     Enemies,
					 Bossing,
					 false,
					 false,
					 IsPaused,
					 PressStart,
					 false,
					 60*3,
					 60*3,
					 PrintGameOver,
					 Active );
		

	}
	
	Sound::Instance()->SetMasterVolume( 1024 );
	
	return Active;		

}

//--------------------------------------
//	
//--------------------------------------
void Engine::SetMode( int &GameMode )
{
	
	//Change Mode Depending on the Choice
	if( GameMode == 0 )  // Normal
	{
		FADE_BGM = ( 60 * 135 ) * 1 + 128;   // main final
		START_BOSS = FADE_BGM + (64*4);
		STOP_SPAWING = FADE_BGM - (64*5);

		BOSS_DEATH_FRAME = START_BOSS + ( 60 * 120 );
		START_COUNTDOWN = BOSS_DEATH_FRAME - (60 * 10);
	}
	else				// Boss Rush
	{
		FADE_BGM = 512*1+128;       // for boss
		START_BOSS = FADE_BGM + (64*4);
		STOP_SPAWING = FADE_BGM - (64*5);

		BOSS_DEATH_FRAME = START_BOSS + ( 60 * 120 );
		START_COUNTDOWN = BOSS_DEATH_FRAME - (60 * 10);
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
int Engine::MainMenu( int &GameMode, bool &IsReplay )
{
	
	int Choice = 1;
	int PlayerChoice = -1;
	
	
	// Load high score table and replay if present
	LoadHiScore();
	
	// Hide SubFonts
	SubFont.Clear();
	oamUpdate(&oamSub);
	IsDemo = false;
	
	while( (Choice > 1) || (PlayerChoice == -1) )
	{
		
		Choice = Menu();
		if( IsDemo ) break;
		
		switch( Choice )
		{
			case 0:   // Start
				GameMode = 0;
				PlayerChoice = SelectPlayer();
				break;
			case 1:		// Boss rush
				GameMode = 1;
				PlayerChoice = SelectPlayer();
				break;
			case 2:   // Replay
				if( LoadReplay() < 2)   //2 == exit
				{
					IsReplay = true;
					GameMode = Button::Replay.Mode;
					SetMode( GameMode );
					return -1; 
				}
				break;
			case 3:		// Saveload replay
				SaveReplay();
				break;
			case 4:   // Records
				PrintHighScore();
				break;
			case 5:   // Options
				Options();
				break;
			case 6:   // Credits
				Credits();
				break;
			case 7:   // End
				//Exit();
				exit(0);
				break;
			
		}
	} 
	
	SetMode( GameMode );
	
	return PlayerChoice;
	
	
}

//--------------------------------------
//	
//--------------------------------------
int Engine::Menu()
{
	
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	const int START_DEMO = 60 * 20;
	
	struct MenuPos
	{
		s32 x;
		s32 x1;
		s32 x2;
		s32 y;
		s32 Interp;
		s32 Speed;
		int Spacing;
	};

	const int START_MENU = 200;
	
	
	const char *Menu[] =
	{ 
		"START",
		"BOSS RUSH",
		"LOAD REPLAY",
		"SAVE REPLAY",
		"RECORDS",
		"OPTIONS",
		"CREDITS",
		"EXIT"
	};

	MenuPos MenPos[8];

	for( int i = 0; i < 8; i++ )
	{
		MenPos[i].x1 = 255 << 12;
		MenPos[i].x2 = (50 + i * 10) << 12;
		MenPos[i].y = (20 + i * 20) << 12;
		MenPos[i].Interp = 0;
		int Steps = 60 * 1 + (i * 20);
		MenPos[i].Speed = 4096/Steps;
	}
	
	

	u8* Plaz = Fx2D::Instance()->PlasmaPolar5();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal3Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_MENU, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	s32 SoundInterp = 1 << 12;
	
	// fade from black sub bg
	s32 BGInterp = 0;
	int CockPos = 192+126;
	int Active = 0;
	int Frame = 0;
	int AnimFrame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	
	Sound::Instance()->PlaySFX( E_SFX_BOMB );
	
	while( FinDelay > 0 )
	{
		Frame++;
		AnimFrame++;
		
		if( Frame > START_MENU ) 
		{
			scanKeys();
			int KeyD = keysDown();
			
			if( !Finished )
			{
				if( (KeyD & KEY_UP) )
				{
					Active--;
					if( Active < 0 ) Active = 7;
					AnimFrame = 0;
					Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );				
				}
				
				if( (KeyD & KEY_DOWN) )
				{
					Active++;
					if( Active > 7 ) Active = 0;
					AnimFrame = 0;
					Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
				}
				
				if( (KeyD & KEY_START) || (KeyD & KEY_A) )
				{
					if( !Finished )
					{
						BGInterp = 0;
						Finished = true;
						Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
					}
				}
				
				if( Frame == START_DEMO )
				{
					if( !Finished )
					{
						BGInterp = 0;
						Finished = true;
						IsDemo = true;
						//Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
					}
					
				}
				
			}
		}
			
		
		s32 Animator = sinLerp(AnimFrame * 256) * 2; 
	
		if( Frame <= START_MENU ) 
		{
			for( int i = 0; i < 8; i++ )
			{	
				MenPos[i].Interp += MenPos[i].Speed;
				if( MenPos[i].Interp > 4096 )  MenPos[i].Interp = 4096;
				MenPos[i].x = UTIL::Lerpf32( MenPos[i].x1, 
											 MenPos[i].x2, 
											 MenPos[i].Interp );
				MenPos[i].Spacing = UTIL::Lerpf32( 7 << 12, 0 << 12, MenPos[i].Interp );
			}		
		}
		

		if( CockPos > 192 ) CockPos--;
		
		
		GlobalManager::Instance()->ResetAll();
		
		Fx3D::Instance()->UpdateStars();
		Fx3D::Instance()->DrawParts();
		
		glBegin2D();
			
			int random = qran_range( -2,2 );
			
			// left hud
			glBoxFilledGradient( 73, (CockPos - 70) + random,
								 73 + 10, (CockPos - 70 + 10) + random,
								 RGB15(qran_range(0,31),qran_range(0,31),qran_range(0,31)),
								 RGB15(qran_range(0,31),qran_range(0,31),qran_range(0,31)),
								 RGB15(qran_range(0,31),qran_range(0,31),qran_range(0,31)),
								 RGB15(qran_range(0,31),qran_range(0,31),qran_range(0,31)) );
			
			// bottom hud
			int color = (abs(sinLerp(Frame*1200))*31) >> 12;
			glBoxFilledGradient( 80, (CockPos - 55) + random,
								 80 + 75, (CockPos - 55 + 12) + random,
								 RGB15(color,31,color),
								 RGB15(31,31,color),
								 RGB15(color,31,31),
								 RGB15(color,31,color) );
			
			// right hud
			color = (abs(sinLerp(Frame*512))*31) >> 12;
			glBoxFilledGradient( 145, (CockPos - 80) + random,
								 145 + 20, (CockPos - 80 + 24) + random,
								 RGB15(color,color,color),
								 RGB15(color,color,color),
								 RGB15(color,color,color),
								 RGB15(color,color,color) );
			
			glColor( RGB15(31,31,31) );
			glSprite( 0, (CockPos - 126) + random, GL_FLIP_NONE, &Cockpit[0] );
			
			glColor(RGB15(color,31-color,color));
			glSpriteRotateScaleXY( SCREEN_WIDTH/2-2, (CockPos - 76), Frame*140, sinLerp(Frame*120) * 1, sinLerp(Frame*210) * 1,  GL_FLIP_NONE, Helix );
			glColor(RGB15(31-color,color,color));
			glSpriteRotateScaleXY( SCREEN_WIDTH/2-2, (CockPos - 76), -Frame*140, sinLerp(Frame*210) * 1, sinLerp(Frame*120) * 1,  GL_FLIP_NONE, Helix );
			
			glColor( RGB15(31,31,0) );
			if( Frame <= START_MENU ) 
			{
				for( int i = 0; i < 8; i++ )
				{
					SiFontGL.PrintSpaced( MenPos[i].x >> 12,
										  MenPos[i].y >> 12, 
										  Menu[i], 
										  0,
										  MenPos[i].Spacing,
										  MenPos[i].Spacing*2 );
				}
			}
			else
			{
				for( int i = 0; i < 8; i++ )
				{
					if( i != Active )
					{
						glColor( RGB15(31,31,0) );
						SiFontGL.PrintSpaced( MenPos[i].x >> 12,
											  MenPos[i].y >> 12, 
											  Menu[i], 
											  0,
											  MenPos[i].Spacing,
											  MenPos[i].Spacing*2 );
					}
					else
					{
						glColor( RGB15(31,0,31) );
						SiFontGL.PrintSpaced( MenPos[i].x >> 12,
											  MenPos[i].y >> 12, 
											  Menu[i], 
											  0,
											  abs(Animator),
											  0 );
					}
				}
			}

	
			glColor( RGB15(31,31,31) );
			if( Frame > START_MENU )
			{
				SiFontGL.PrintAscii( (MenPos[Active].x2 >> 12) - 15,
									 MenPos[Active].y >> 12,
									 65+31, 
									 1 );
			}
			
			// Version
			glColor( RGB15(31,31,31) );
			SiFontGL.Print( 165, 0, "V 2.0 FINAL");
		
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
		
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
	
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 1, "SPACE" );
		SubFont2.PrintCentered( 0, 16 * 3, "IMPAKTO" );
		SubFont2.PrintCentered( 0, 16 * 5, "D S" );
		
		SubFont.PrintCentered( 0, 16 * 7, "MODARIFFIC.COM" );
		SubFont.PrintCentered( 0, 16 * 9, "ROCKSOFTONLINE.COM" );
		SubFont.PrintCentered( 0, 16 * 11, "GBATEMP.NET" );
		
		// Draw
		oamUpdate(&oamSub);
		
		
		s32 SprInterp = BGInterp;
	
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
				
		
		}
		else
		{
			FinDelay--;
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
		
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		
	
	}
	
	//Sound::Instance()->StopBGM();
	
	return Active;

}

//--------------------------------------
//	
//--------------------------------------
int Engine::Replay( PlayerShip &Ship, int &GameMode, int &Stage )
{
	
	GameMode = Button::Replay.Mode;
	Stage = Button::Replay.Stage;
	Ship.SetID( Button::Replay.ShipType );
	Ship.SetLevel( Button::Replay.ShipLevel );
	Ship.SetLives( Button::Replay.ShipLives );
	Ship.SetBombs( Button::Replay.ShipBombs );
	Ship.SetScore( Button::Replay.Score );
	Ship.SetAutoBomb( Button::Replay.AutoBomb );
	
	return Ship.GetID();
	
}

//--------------------------------------
//	
//--------------------------------------
int Engine::TextEntry(int x, int y, int width, const char *filler, int length,char *entry, GLFont &Font, GLFont &TitleFont)
{	
	
	int maxlen = length;
	int k , kh;
	int char_val = 65;
	int pos = 0;
	char asc[2];
	const char *eraser = " ";
	const int MAXCOOLDOWN = 8;
	
	
	
	LoadPlayerName(entry);
	
	int start = strlen(entry)-1;
	if( start < 0 ) start = 0;
	
	if( start != 0 ) pos = strlen(entry)-1;
	
	if( pos > 0 )
	{
		char_val = entry[pos];
	}
	
	for (int i = start; i < maxlen; i++)
	{
		entry[i] = *filler;
	}
	
	entry[maxlen] = '\0';
	
	s32 BGInterp = 0;


	// Hide SubFonts
	SubFont.Clear();
	SubFont2.Clear();
	oamUpdate(&oamSub);
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar5();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal4Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	int CoolDown = MAXCOOLDOWN;
	int Frames = 0;
	
	do
	{
		Frames++;
		scanKeys();
		k = keysDown();
		kh = keysHeld();
		
		if( !(kh & KEY_UP)  && !(kh & KEY_DOWN) )
		{
			CoolDown = 0;
		}
		
		if( kh & KEY_DOWN )
		{
			CoolDown--;
			if( CoolDown < 0 )
			{
				CoolDown = MAXCOOLDOWN;
				char_val -= 1;
				if (char_val < 48)
					char_val = 95;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
		}
		
		if( kh & KEY_UP )
		{
			CoolDown--;
			if( CoolDown < 0 )
			{
				CoolDown = MAXCOOLDOWN;
				char_val += 1;
				if (char_val > 95)
					char_val = 48;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
		}
	
		if( (k & KEY_RIGHT) || (k & KEY_A) )
		{
			if ( pos < maxlen-1)
			{
				
				entry[pos] = char_val; 
				pos++; 
			}
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		else if( (k & KEY_LEFT)  || (k & KEY_B) )
		{
			if (pos > 0)
			{
				entry[pos] = *filler;
				pos--;
			}
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}

		entry[pos] = *eraser;
		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
		
			glColor(RGB15(16,0,0));
			Fx3D::Instance()->WarpScreen( Frames, Helix );
			
			glColor(RGB15(31,31,31));
			TitleFont.PrintCentered(x, 10, "ENTER YOUR NAME");
			TitleFont.PrintCentered(x, 40, "PRESS START TO EXIT");
			
			Font.Print(x, y, entry);
			
			asc[0] = char_val;
			asc[1] = '\0';
			
			glColor(RGB15(0,31,0));
			Font.Print(x+(pos*width), y,  asc);
			
			int c = abs(sinLerp(Frames)*31);
			glColor(RGB15(c,c,c));
			Font.Print(x+(pos*width), y-10,  "-");
			Font.Print(x+(pos*width), y+10,  "-");
			
			
	
		glEnd2D();
		
		glFlush(0);

		swiWaitForVBlank();
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "YOU HAVE JUST" );
		SubFont2.PrintCentered( 0, 16 * 6, "PWNED" );
		SubFont2.PrintCentered( 0, 16 * 9, "SOMEONE'S SCORE!" );
		
		// Draw
		oamUpdate(&oamSub);
	
		Fx2D::Instance()->CyclePlasmaPal();
		Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512
						  );
		
		s32 SprInterp = BGInterp;
		// fade from black sub OAM
		Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								viper_spritePal,
								SprInterp,
								16,
								512
							  );
		

	}
	
	while (!(k & KEY_START));
	
	Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
	
	entry[pos] = char_val;
	entry[pos+1] = '\0';
	
	SavePlayerName(entry);
	
	return 1;
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::PrintHighScore( HighScore &PlayerStat, int GameMode )
{
	
	
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	
	switch( PlayerStat.ShipID )
	{
		case 0:
			strcpy(PlayerStat.Ship, "RED AGILA");
			break;
		case 1:
			strcpy(PlayerStat.Ship, "BLUE BAGWIS");
			break;
		case 2:
			strcpy(PlayerStat.Ship, "LOCO VIPER");
			break;	
	}
	
	// Read HighScore from "SpaceImpaktoDS.his" here
	
	int CurrentPlayerIndex = 11;
	// Insert Player Stat
	
	if( GameMode == 0 )    // Normal
	{
		for( int i = 9; i >= 0; i--)
		{
			int MyScore = PlayerStat.Score;
			if( i > 0)
			{
				if( MyScore > Scores[i].Score )
				{
					if(MyScore <= Scores[i-1].Score)
					{
						// Move entries down
						for( int j = 9; j >= i; j-- )
						{
							Scores[j] = Scores[j-1];
						}
						
						// Insert
						Scores[i] = PlayerStat;
						CurrentPlayerIndex = i;
					}
				}
			}
			else
			{
				if( MyScore > Scores[i].Score )
				{
					for( int j = 9; j >= i; j-- )
					{
						Scores[j] = Scores[j-1];
					}					
					Scores[i] = PlayerStat;
					CurrentPlayerIndex = i;
				}
			}
		}
	}
	else      // Boss Rush
	{
		for( int i = 9; i >= 0; i--)
		{
			int MyScore = PlayerStat.Score;
			if( i > 0)
			{
				if( MyScore > BRScores[i].Score )
				{
					if(MyScore <= BRScores[i-1].Score)
					{
						// Move entries down
						for( int j = 9; j >= i; j-- )
						{
							BRScores[j] = BRScores[j-1];
						}
						
						// Insert
						BRScores[i] = PlayerStat;
						CurrentPlayerIndex = i;
					}
				}
			}
			else
			{
				if( MyScore > BRScores[i].Score )
				{
					for( int j = 9; j >= i; j-- )
					{
						BRScores[j] = BRScores[j-1];
					}					
					BRScores[i] = PlayerStat;
					CurrentPlayerIndex = i;
				}
			}
		}		
	}    // end if gamemode
	
	s32 SoundInterp = 1 << 12;
	s32 BGInterp = 0;


	// Hide SubFonts
	SubFont.Clear(); // deactivate every oam per frame
	SubFont2.Clear(); // deactivate every oam per frame
	
	oamUpdate(&oamSub);
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar5();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal5Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	
	int Frame = 0;
	int Finished = false;
	
	int FinDelay = FADE_FRAMES;
	
	while( FinDelay > 0 )
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
		
		if( (KeyD & KEY_START) || (KeyD & KEY_B) || (KeyD & KEY_A) )
		{
			if( !Finished )
			{
				BGInterp = 0;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
				Finished = true;
			}
		
		}
		
		
		GlobalManager::Instance()->ResetAll();
	
		glBegin2D();
	
			glColor(RGB15(16,0,12));
			Fx3D::Instance()->WarpScreen( Frame, Helix );
			
			glColor(RGB15(31,31,0));
			SiFontGL.PrintCentered( 0, 0, "HALL OF ACES" );
	
			// Print Em
			if( GameMode == 0 )
				PrintScores( Scores, GameMode, Frame, CurrentPlayerIndex, 0 );
			else
				PrintScores( BRScores, GameMode, Frame, CurrentPlayerIndex, 0 );
				
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
		
		glEnd2D();

		glFlush(0);                    
		
		swiWaitForVBlank();
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "MEISTER" );
		SubFont2.PrintCentered( 0, 16 * 8, "SUPERPLAYERS" );
		
		// Draw
		oamUpdate(&oamSub);
		
		s32 SprInterp = BGInterp;
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
		
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		
	}
	
	//Sound::Instance()->StopBGM();
	
	
}


//--------------------------------------
//	
//--------------------------------------
void Engine::PrintHighScore()
{
	
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	// Read HighScore from "SpaceImpaktoDS.his" here
	
	
	int Frame = 0;
	int Finished = false;
	
	s32 SoundInterp = 1 << 12;
	s32 BGInterp = 0;
	
	int ScrollColumn = 0;
	int ScrollDirection = -1;
	int ScrollCountDown = 60 * 8;
	

	// Hide SubFonts
	SubFont.Clear();
	SubFont2.Clear();
	oamUpdate(&oamSub);
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar5();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal5Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	
	int FinDelay = FADE_FRAMES;
	
	while( FinDelay > 0 )
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
		
		if( (KeyD & KEY_START) || (KeyD & KEY_B) || (KeyD & KEY_A) )
		{
			if( !Finished )
			{
				BGInterp = 0;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
				Finished = true;
			}
		
		}
		
		if( Frame > 60 * 6)
		{
			ScrollColumn += ScrollDirection;
			if( ScrollColumn < -256 )
			{
				if( ScrollCountDown > 0 )
				{
					ScrollCountDown--;
					ScrollDirection = 0;
				}
				else //ScrollCountDown == 0
				{
					ScrollCountDown = 60 * 6;
					ScrollDirection = 1;
				}
			}
			
			if( ScrollColumn > 0 )
			{
				if( ScrollCountDown > 0 )
				{
					ScrollCountDown--;
					ScrollDirection = 0;
				}
				else //ScrollCountDown == 0
				{
					ScrollCountDown = 60 * 6;
					ScrollDirection = -1;
				}
			}
		}
	
		
		GlobalManager::Instance()->ResetAll();
	
		glBegin2D();
	
			glColor( RGB15(0,16,0) );
			Fx3D::Instance()->WarpScreenPolar( Frame, Helix );
			
			
			glColor(RGB15(31,31,0));
			SiFontGL.PrintCentered( 0, 0, "HALL OF ACES" );
			
			// Print Em
			// Normal
			PrintScores( Scores, 0, Frame, 99, ScrollColumn + 4 );
			
			// Boss Rush
			PrintScores( BRScores, 1, Frame, 99, ScrollColumn + 256 + 6);
			
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
	
		glEnd2D();

		glFlush(0);                    
		
		swiWaitForVBlank();
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "MEISTER" );
		SubFont2.PrintCentered( 0, 16 * 8, "SUPERPLAYERS" );
		
		// Draw
		oamUpdate(&oamSub);
		
		s32 SprInterp = BGInterp;
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
		
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		
	}
	
	//Sound::Instance()->StopBGM();
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::Options()
{
	
	KeyConfig();
	
	// Save ConfigFile
	strcpy( Button::Config.Name, PlayerStat.Name );
	Button::Config.AutoBomb = AutoBomb;
	for( int i = 0; i < 8; i++ )
		Button::Config.Control[i] = Button::Control[i];
		
	Button::SaveConfigFile();
}

//--------------------------------------
//	
//--------------------------------------
void Engine::Credits()
{

	const int MAX_TEXT = 86;
	const int DONE = 60 * 52;
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const char *Text[] =
	{ 
		"SPACE IMPAKTO DS",
		" ",
		"    1ST PLACE DS CATEGORY",
		"   GBATEMP HOMEBREW BOUNTY",
		"            2011",
		" ",
		"PROGRAMMING:",
		"   RICHARD ERIC M. LOPE BSN RN",
		"   (RELMINATOR)",
		"   HTTP://REL.PHATCODE.NET",
		" ",
		"DESIGN:",
		"   ANYA THERESE B. LOPE",
		"   RELMINATOR",
		" ",
		"GRAPHICS:",
		"   ADIGUN A. POLACK",
		"   RELMINATOR",
		"   MARC RUSSEL",
		"   VATOLOCO (ICON)",
		"   TAITO",
		"   IREM",
		"   KONAMI",
		" ",
		"MUSIC:",
		"   VGMUSIC.COM",
		"   WAVEOFDARKNESS",
		"   FIREMARIO",
		" ",
		"TOOLS:",
		"   DEVKITPRO",
		"   LIBNDS",
		"   EASY GL2D",
		"   MAXMOD",
		" ",
		"ADDITIONAL CODE:",
		"   LIBNDS EXAMPLES",
		"   JASPER VIJN (CEARN) ",
		" ",
		"BETA TESTERS:",
		"   URZA",
		"   MAZOR",
		"   ORC",
		"   JURASSIC PLAYER",
		"   MARCUS CLARK",
		"   CALEB CLARK",
		"   VATOLOCO",
		" ",
		"GREETZ:",
		"   GOD",
		"   DR_D",
		"   WINTERMUTE",
		"   PLASMA",
		"   HELLFIRE",
		"   CEARN",
		"   STEVEN",
		"   IMMIBIS",
		"   ZEROMUS",
		"   CYAN",
		"   RYDIAN",
		"   L_O_J",
		"   V1CTOR",
		"   MOTORHERP",
		"   HEADSPIN",
		"   DISCOSTEW",
		"   FLASH",
		"   JINROH",
		"   ELHOBBS",
		"   DWEDIT",
		"   SVERX",
		"   X-OUT",
		"   SHOCKWAVE",
		"   ALL THE GUYS AT:",
		"     DEVKITPRO.ORG",
		"     GBADEV.ORG",
		"     GBATEMP.NET",
		"     NINTENDOMAX.COM",
		"     DBFINTERACTIVE.COM",
		"     FREEBASIC.NET",
		"     SHMUP-DEV.COM",
		"     SHMUPS.COM",
		"     SYMBIANIZE.COM",
		"     GREATFLASH.CO.UK",
		"     ROCKSOFTONLINE.COM",
		"     BLITZED: #DSDEV",
		"     GBATEMP: #GBATEMP.NET",
		" "
	};
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar2();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal4Pal );
	
	Sound::Instance()->PlayBGM( E_BGM_CREDITS, MM_PLAY_ONCE );
	Sound::Instance()->SetMasterVolume( 512 );
	
	s32 SoundInterp = 1 << 12;
	
	// fade from black sub bg
	s32 BGInterp = 0;
	s32 Scroll_X = SCREEN_HEIGHT << 12;
	int Frame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	
	while( FinDelay > 0 )
	{
		Frame++;
		if( (Frame == DONE) && (!Finished) )
		{
			Finished = true;
			BGInterp = 0;
		}
		
		Scroll_X -= floattof32(0.45);
		
		
		scanKeys();
		int KeyD = keysDown();
	
		if( (KeyD & KEY_START) || (KeyD & KEY_A) || (KeyD & KEY_B) )
		{
			if( !Finished ) BGInterp = 0;
			Finished = true;
		}
		
		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
			int Row = Scroll_X >> 12;
			
			for( int i = 0; i < MAX_TEXT; i++ )
			{
				if( *Text[i] == 32 )
				{
					glColor( RGB15(31,31,31) );
				}
				else
				{
					glColor( RGB15(31,31,0) );
				}
				SiFontGL.Print( 10, Row, Text[i] );
				Row += 16;
			}
			
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "THANK YOU" );
		SubFont2.PrintCentered( 0, 16 * 6, "SALAMAT" );
		SubFont2.PrintCentered( 0, 16 * 9, "ARIGATO" );
		
		// Draw
		oamUpdate(&oamSub);
	
		
		s32 SprInterp = BGInterp;
	
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			
		
		}
		
	}
	
	//Sound::Instance()->StopBGM();
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::Ending( int LoopNumber )
{

	const int MAX_TEXT = 21;
	const int DONE = 64 * 38;
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const char *Text[] =
	{ 
		"CONGRATULATIONS",
		" ",
		"YOU ARE THE",
		"MESSIAH ",
		"OF",
		"EARTH.",
		" ",
		"TIME TO GO HOME",
		"AND",
		"DRINK SOME", 
		"COFFEE",
		"TO CALM",
		"THE NERVES.",
		" ",
		"SEE YOU IN...",
		" ",
		"FRANTIC JOURNEY",
		"DS",
		" ",
		" "
		" ",
		" "
		
	};
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar4();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal4Pal );
	
	Sound::Instance()->PlayBGM( E_BGM_ENDING, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	s32 SoundInterp = 1 << 12;
	
	// fade from black sub bg
	s32 BGInterp = 0;
	int Scroll_X = SCREEN_HEIGHT;
	int Frame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	
	while( FinDelay > 0 )
	{
		Frame++;
		if( (Frame == DONE) && (!Finished) )
		{
			Finished = true;
			BGInterp = 0;
		}
		
		if( (Frame % 3) == 0) Scroll_X--;
		
		//scanKeys();
		//int KeyD = keysDown();
	
		//if( (KeyD & KEY_START) || (KeyD & KEY_A) || (KeyD & KEY_B) )
		//{
		//	if( !Finished ) BGInterp = 0;
		//	Finished = true;
		//}
		
		
		GlobalManager::Instance()->ResetAll();
		
		glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		Fx3D::Instance()->UpdateStars();
		Fx3D::Instance()->DrawParts();
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
			int Row = Scroll_X;
			
			for( int i = 0; i < MAX_TEXT; i++ )
			{
				if( i < (MAX_TEXT - 1) )
				{
					SiFontGL16x16.PrintCentered( 10, Row, Text[i] );
					Row += 32;
				}
				else
				{
					glColor( RGB15(31,31,0) );
					if( LoopNumber == 1 )
					{
						SiFontGL16x16.PrintCentered( 10, Row, "ATTACK WAVE 2" );
					}
					else
					{
						SiFontGL16x16.PrintCentered( 10, Row, "GAME OVER" );
					}
					glColor( RGB15(31,31,31) );
				}
			}
			
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "ANG LUPET" );
		SubFont2.PrintCentered( 0, 16 * 6, "NG KAMAY" );
		SubFont2.PrintCentered( 0, 16 * 9, "MO!" );
		
		// Draw
		oamUpdate(&oamSub);
	
		
		s32 SprInterp = BGInterp;
	
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			
		
		}
		
	}
	
	//Sound::Instance()->StopBGM();
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::Exit()
{
	s32 BGInterp = 1 << 12;
	int Frame = 0;
	int Finished = false;
	
	while(!Finished)
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
	
		if( (KeyD & KEY_START) || (KeyD & KEY_A) )
		{
			Finished = true;
		}
		
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			SiFontGL.PrintCentered( 0, 80, "EXIT" );
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
	
		Fx2D::Instance()->CyclePlasmaPal();
		Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512
						  );
			
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::Splash()
{
	const int DONE = 64 * 5;
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	glLoadTileSet( HbLogoImage,				// pointer to glImage array
				   128,					// sprite width
				   128,					// sprite height
				   128,					// bitmap width
				   128,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_128,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   256,					// Length of the palette to use (256 colors)
				   (u16*)hblogoPal,		// Load our 256 color tiles palette
				   (u8*)hblogoBitmap		// image data generated by GRIT
				 );
	
	glLoadTileSet( GbaTempLogoImage,				// pointer to glImage array
				   256,					// sprite width
				   32,					// sprite height
				   256,					// bitmap width
				   32,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   256,					// Length of the palette to use (256 colors)
				   (u16*)gbatemplogoPal,		// Load our 256 color tiles palette
				   (u8*)gbatemplogoBitmap		// image data generated by GRIT
				 );
	
	// fade to black instantly to hide the loaded BG
	s32 BGInterp = 1 << 12;
	Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Fx2D::Instance()->GetPlasmaPal(),
							Palettes::Instance()->GetBlack(),
							BGInterp,
							16,
							512 );
	
	
	
	// Load new palette for the gameplay sub bg
	//u8* Plaz = Fx2D::Instance()->PlasmaPolar();
	
	//u8* Plaz = Fx2D::Instance()->PlasmaStencil(logoBitmap);
	DC_FlushRange(logoBitmap, SCREEN_WIDTH * SCREEN_HEIGHT);
    dmaCopy( (u8*)logoBitmap, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	
	
	// Change pal
	// Only load the last half of the palette so that we could use
	// the sub for both text and plasma
	//Fx2D::Instance()->LoadPal( 128, 255, plasma_palPal );
	Fx2D::Instance()->LoadPal( logoPal );
	
	
	// fade from black sub bg
	BGInterp = 0;
	int Frame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	s32 MaxRotate = BRAD_PI * 6;
	s32 Rotation = MaxRotate;
	s32 Scale = 0;
	s32 AniInterp = 0;
	s32 Color = 0;
	
	while( FinDelay > 0 )
	{
		Frame++;
		
		Rotation = UTIL::Lerpf32( MaxRotate, 0, AniInterp );
		Scale = UTIL::Lerpf32( 0, 1 << 12, AniInterp );
		Color = UTIL::Lerpf32( 0, 31 << 12, AniInterp );
		
		
		if( (Frame == DONE) && (!Finished) )
		{
			Finished = true;
			BGInterp = 0;
		}
		

		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
			glColor(RGB15(31,31,31));
			
			glSpriteRotateScale( 128,
								 190 - 64,
								 Rotation,
								 Scale,
								 GL_FLIP_NONE, 
								 HbLogoImage );
			
			glSpriteScaleXY( 0, 10, Scale, 1 << 12,GL_FLIP_NONE, GbaTempLogoImage );
			
			
			glColor( RGB15(Color >> 12, Color >> 12, Color >> 12 ) );
			SiFontGL.Print( 20, 70, "1ST");
			SiFontGL.PrintVertical( 30, 90, "P L A C E");
			
			SiFontGL.Print( 210, 70, "D S");
			SiFontGL.PrintVertical( 220, 90, "CATEGORY ^");
			
			
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
			
			
			
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
		
		if( !Finished )
		{
			AniInterp = BGInterp;
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
		
		}
		else
		{
			FinDelay--;
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
		
		}
	
	
	}
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::Story()
{

	const int MAX_TEXT = 24;
	const int DONE = 64 * 26;
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const char *Text[] =
	{ 
		"THE YEAR WAS",
		"2339.",
		" ",
		"THEY CAME WITHOUT WARNING.",
		" ",
		"THEY CALL THEMSELVES...",
		" ",
		"- THE HAMMER OF GODS -",
		" ",
		"THEY DESTROYED THE PLANET'S",
		"DEFENSES WITH EASE.",
		" ",
		"YET, BECAUSE OF THEIR ARROGANCE,",
		"THEY GAVE US HOPE.",
		" ",
		"THREE SHIPS AGAINST THOUSANDS.",
		" ",
		"THIS IS THE STORY OF A PILOT",
		"KNOWN ONLY AS... ",
		" ",
		" ",
		"CODENAME:",
		" ",
		"SPACE IMPAKTO",
		" "
	};
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar3();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal6Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	s32 SoundInterp = 1 << 12;
	
	// fade from black sub bg
	s32 BGInterp = 0;
	
	int Scroll_X = SCREEN_HEIGHT-20;
	int Frame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	
	
	while( FinDelay > 0 )
	{
		Frame++;
		if( (Frame == DONE) && (!Finished) )
		{
			Finished = true;
			BGInterp = 0;
		}
		
		if( (Frame % 3) == 0) Scroll_X--;
		
		scanKeys();
		int KeyD = keysDown();
	
		if( (KeyD & KEY_START) || (KeyD & KEY_A) )
		{
			if( !Finished ) BGInterp = 0;
			Finished = true;
		}
		
		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			int Row = Scroll_X;
			glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
			for( int i = 0; i < MAX_TEXT; i++ )
			{
				if( i < 23 )
					SiFontGL.PrintCentered( 0, Row, Text[i] );
				else
					SiFontGL16x16.PrintCentered( 0, Row, Text[i] );
			
				Row += 16;
			}
			
			for( int i = 0; i < (192/8); i++ )
			{
				s32 Alpha = abs(cosLerp(i * 768)) * 30;
				glPolyFmt( POLY_ALPHA((Alpha >> 12) + 1) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
				glBoxFilled( 0, i * 8, 256, i * 8 + 7, RGB15(0,0,0) );
			}
			
			glColor( RGB15(0,31,31) );
			glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
			SiFontGL.PrintCentered( 0, 0, "CAPTAIN'S LOG - CIROCCO JONES" );
			SiFontGL.PrintCentered( 0, 10, "AD 2340" );
			
			
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
		
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
	
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "HYPERION" );
		SubFont2.PrintCentered( 0, 16 * 6, "ARCHIVES" );
		SubFont2.PrintCentered( 0, 16 * 9, "STARDATE 1337" );
		
		// Draw
		oamUpdate(&oamSub);
	
		
		s32 SprInterp = BGInterp;
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
		
		}
		else
		{
			FinDelay--;
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
		
		}
	
	}
	
	//Sound::Instance()->StopBGM();
	
}

//--------------------------------------
//	
//--------------------------------------
int Engine::SelectPlayer()
{
	
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const int XRADIUS = 50;
	const int YRADIUS = 75;
	const int CHOICES = 6;
	const s32 BRAD_STEPS = (BRAD_PI*2)/CHOICES;
	
	IsDemo = false;
	
	const char *Text[] =
	{ 
		"SHIP:",
		"   f RED AGILA",
		" ",
		"PILOT CODENAME:",
		"   f SPACE IMPAKTO",
		" ",
		"ARMAMENT:",
		"   f WAVE PLASMA",
		"   f T-BLADE",
		" ",
		"SPEED:",
		"   f NORMAL"
	};
	
	const char *Text2[] =
	{ 
		"SHIP:",
		"   f BLUE BAGWIS",
		" ",
		"PILOT CODENAME:",
		"   f LORD AZKAL",
		" ",
		"ARMAMENT:",
		"   f WIDE LASER",
		"   f H-MISSILE",
		" ",
		"SPEED:",
		"   f SLOW"
	};
	
	const char *Text3[] =
	{ 
		"SHIP:",
		"   f LOCO VIPER",
		" ",
		"PILOT CODENAME:",
		"   f VATO GUAPO",
		" ",
		"ARMAMENT:",
		"   f BARYON VULCAN",
		"   f OPTION / S-W",
		" ",
		"SPEED:",
		"   f HIGH"
	};
	
	int DrawFrame = 2;
	int DrawDirection = 1;
	
	s32 CurrentAngle = 0;
	s32 NextAngle = 0;
	
	// fade from black sub bg
	s32 BGInterp = 0;
	bool Animate = false;
	int Active = 0;
	int Frame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	bool GoBack = false;
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar4();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal5Pal );
	
	CheatOn = false;
	
	while( FinDelay > 0 )
	{
		Frame++;
		if( (Frame & 7) == 0 )
		{
			DrawFrame += DrawDirection;
			if( DrawFrame < 1 ) DrawDirection = -DrawDirection;
			if( DrawFrame > 3 ) DrawDirection = -DrawDirection;
		}
		
		scanKeys();
		int KeyD = keysDown();
		int KeyH = keysHeld();
		
		if( !Animate && !Finished )
		{
			if( (KeyD & KEY_UP) || (KeyD & KEY_LEFT) )
			{
				Active++;
				if( Active > 5 ) Active = 0;
				Animate = true;
				NextAngle -= BRAD_STEPS;
				Sound::Instance()->PlaySFX( E_SFX_MENU_SELECT );
			}
			
			if( (KeyD & KEY_DOWN) || (KeyD & KEY_RIGHT) )
			{
				Active--;
				if( Active < 0 ) Active = 5;
				Animate = true;
				NextAngle += BRAD_STEPS;
				Sound::Instance()->PlaySFX( E_SFX_MENU_SELECT );
			}
		}
		
		if( (KeyD & KEY_START) || (KeyD & KEY_A) )
		{
			if( (KeyH & KEY_R) ) 
				CheatOn = true;
		
			if( !Finished ) 
			{
				BGInterp = 0;
				GoBack = false;
				Finished = true;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
		}
		
		
		if( (KeyD & KEY_B) )
		{
			if( !Finished ) 
			{
				BGInterp = 0;
				GoBack = true;
				Finished = true;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
		}
		
		if( Animate )
		{
			if( CurrentAngle < NextAngle )
			{
				CurrentAngle += 256;
				if( CurrentAngle >= NextAngle )
				{
					CurrentAngle = NextAngle;
					Animate = false;
				}
			}
			else
			{
				CurrentAngle -= 256;
				if( CurrentAngle <= NextAngle )
				{
					CurrentAngle = NextAngle;
					Animate = false;
				}
			}
		}
		
		
		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			Fx3D::Instance()->WarpScreen( Frame, WarpTile );
			
			glColor( RGB15(31,31,31) );
			int Counter = 0;
			for( int i = CurrentAngle; i < (BRAD_PI*2 + (CurrentAngle-256)); i += BRAD_STEPS )
			{
				int x = cosLerp(i) * XRADIUS;
				int y = sinLerp(i) * YRADIUS;
				int ShipActive = abs(Counter) % 3;
				if( ShipActive == 0 )
				{
					if( Counter == Active )
					{
						glSprite(  (x >> 12) - 5, (y >> 12) + 80,
									GL_FLIP_NONE, &ViperSpriteImages[DrawFrame] );
					}
					else
					{
						glSprite( (x >> 12) - 5, (y >> 12) + 80,
								  GL_FLIP_NONE, &ViperSpriteImages[2] );
					}
				}
				else if( ShipActive == 1 )
				{
					if( Counter == Active )
					{
						glSprite(  (x >> 12) - 5, (y >> 12) + 80,
									GL_FLIP_NONE, &R9SpriteImages[DrawFrame] );
					}
					else
					{
						glSprite( (x >> 12) - 5, (y >> 12) + 80,
								  GL_FLIP_NONE, &R9SpriteImages[2] );
					}
					
				}
				
				else
				{
					if( Counter == Active )
					{
						glSprite(  (x >> 12) - 5, (y >> 12) + 76,
									GL_FLIP_NONE, &VicSpriteImages[8+DrawFrame] );
					}
					else
					{
						glSprite( (x >> 12) - 5, (y >> 12) + 76,
								  GL_FLIP_NONE, &VicSpriteImages[10] );
					}
					
				}
				
				//SiFontGL.Print( (x >> 12) - 15, (y >> 12) + 80, Counter );
				Counter++;
			}
		
			SiFontGL.PrintCentered( 0, 180, "sPLEASE SELECT BATTLESHIPr" );
			
			glColor( RGB15(-Frame*20,Frame,Frame*120) );
			SiFontGL.Print( 35, 62, "______" );
			
			SiFontGL.Print( 35, 95, "______" );
			
			
			//SiFontGL.PrintCentered( 0, 170, "abcdefghijklmnopqrstuvwxyz!@#$%^&*()" );
			
			for( int i = 0; i < 12; i++)
			{
				if( *Text[i] == 32 )
					glColor( RGB15(31,31,0) );
				else
					glColor( RGB15(31,31,31) );
				
				int ShipActive = abs(Active) % 3;
				switch( ShipActive )
				{
					case 0:
						SiFontGL.Print( 110, 20 + i * 10, Text[i] );
						break;
					case 1:
						SiFontGL.Print( 110, 20 + i * 10, Text2[i] );
						break;
					case 2:
						SiFontGL.Print( 110, 20 + i * 10, Text3[i] );
						break;
						
						
				}
				
			}	
		
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();

		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "SCRAMBLE" );
		SubFont2.PrintCentered( 0, 16 * 6, "FOR" );
		SubFont2.PrintCentered( 0, 16 * 9, "SURVIVAL" );
		
		// Draw
		oamUpdate(&oamSub);
	
		s32 SprInterp = BGInterp;
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );

			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		
	}
	
	if( !GoBack )
	{
		return ( abs(Active) % 3 );
	}
	else
	{
		return -1;
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Engine::PrintKeyMapping( int Keys[3], const char *MenuText[], const char *Text[],
							  int RowCoords[], int StartRow, int StartColumn,
							  int ColumnOffset )
{
	
	enum  
	{
		FIRE = 0,
		BOMB,
		SLOW
	};
	
	
			
		// Fire
		glBoxFilled( ColumnOffset * 8 + StartColumn*8 + (Keys[FIRE] * 16) - 1, 
					 (StartRow * 10) + (0 * 10) - 1,
					 ColumnOffset * 8 + StartColumn*8 + (Keys[FIRE] * 16) + 8, 
					 (StartRow * 10) + (0 * 10) + 9,
					 RGB15(24,0,0) );

		// Bomb
		glBoxFilled( ColumnOffset * 8 + StartColumn*8 + (Keys[BOMB] * 16) - 1, 
					 (StartRow * 10) + (1 * 10) - 1,
					 ColumnOffset * 8 + StartColumn*8 + (Keys[BOMB] * 16) + 8, 
					 (StartRow * 10) + (1 * 10) + 9,
					 RGB15(24,0,0) );

		// Slow
		glBoxFilled( ColumnOffset * 8 + StartColumn*8 + (Keys[SLOW] * 16) - 1, 
					 (StartRow * 10) + (2 * 10) - 1,
					 ColumnOffset * 8 + StartColumn*8 + (Keys[SLOW] * 16) + 8, 
					 (StartRow * 10) + (2 * 10) + 9,
					 RGB15(24,0,0) );


		// Main key config menu
		glColor( RGB15(31,31,0) );
		for( int i = 0; i< 4; i++)
		{
			SiFontGL.Print(  20, RowCoords[i], MenuText[i] );
		}
		
		// Controls Menu
		glColor( RGB15(31,31,31) );
		for( int i = 0; i < 3; i++ )
		{
			SiFontGL.Print( ColumnOffset * 8 , (StartRow * 10) + i * 10, Text[i] );
			SiFontGL.Print( ColumnOffset * 8 + StartColumn* 8, (StartRow * 10) + i * 10, Text[3] );
		}
		
		

	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::EditControls( int Keys[3], int Choices[6], const char *Text[], 
						   const char *MenuText[], int RowCoords[] )
{
	
	
	enum  
	{
		FIRE = 0,
		BOMB,
		SLOW
	};
	
	
	int Table[3][6];
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 6; j++)
		{
			Table[i][j] = 0;
		}
	}
	
	Table[FIRE][1] = 1;
	Table[BOMB][0] = 1;
	Table[SLOW][5] = 1;
	
	int Frame = 0;
	int Finished = false;
	
	int ActiveRow = 0;
	int ActiveColumn = 0;
	
	int ColumnOffset = 5;
	int StartRow = 7;
	int StartColumn = 12;

	s32 BGInterp = 1 << 12;
	
	while(!Finished)
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
		
		if( (KeyD & KEY_START) || (KeyD & KEY_B) )
		{
			Finished = true;
		}
		
		if( (KeyD & KEY_UP) )
		{
			ActiveRow--;
			if( ActiveRow < 0 ) ActiveRow = 2;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_DOWN) )
		{
			ActiveRow++;
			if( ActiveRow > 2 ) ActiveRow = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_LEFT) )
		{
			ActiveColumn--;			
			if( ActiveColumn < 0 ) ActiveColumn = 5;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_RIGHT) )
		{
			ActiveColumn++;
			if( ActiveColumn > 5 ) ActiveColumn = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_A) )
		{
			
			int RowU = (ActiveRow+1) % 3;
			int RowD = (ActiveRow-1);
			if( RowD < 0 ) RowD = 2;
			
			if( (Table[RowU][ActiveColumn] == 0) && (Table[RowD][ActiveColumn] == 0) )
			{
				Table[ActiveRow][Keys[ActiveRow]] = 0;
				Table[ActiveRow][ActiveColumn] = 1;
				Keys[ActiveRow] = ActiveColumn;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
		}
		
		
		Fx2D::Instance()->CyclePlasmaPal();
		Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512
						  );
	
		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
		
		SiFontGL16x16.PrintCentered( 0, 10, "EDIT CONTROLS" );
		
		PrintKeyMapping( Keys, MenuText, Text, RowCoords, StartRow, StartColumn, ColumnOffset );
		
		glPolyFmt( POLY_ALPHA(8) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		
		// Cursor
		glBoxFilled( ColumnOffset * 8 + StartColumn*8 + (ActiveColumn * 16) - 2, 
			   (StartRow * 10) + (ActiveRow * 10) - 2,
			   ColumnOffset * 8 + StartColumn*8 + (ActiveColumn * 16) + 9, 
			   (StartRow * 10) + (ActiveRow * 10) + 10,
			   RGB15(31,0,31) );
		
		glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
		SiFontGL.PrintCentered( 0, 170, "PRESS A TO CHANGE CONTROLS" );
		SiFontGL.PrintCentered( 0, 182, "START OR B TO GO BACK" );
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
	
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
void Engine::KeyConfig()
{
	
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	enum  
	{
		FIRE = 0,
		BOMB,
		SLOW
	};
	
	const char *Text[] =
	{ 
		"FIRE     : ",
		"BOMB     : ",
		"SLOWDOWN : ",
		"A B X Y L R "
	};
	
	const char *MenuText[] =
	{ 
		"CONTROLS: ",
		"AUTOBOMB: ",
		"DEFAULT ",
		"EXIT "
	};
	
	int RowCoords[] = 
	{
		50, 110, 130, 150
	};
	
	int Choices[6];
	Choices[0] = KEY_A;
	Choices[1] = KEY_B;
	Choices[2] = KEY_X;
	Choices[3] = KEY_Y;
	Choices[4] = KEY_L;
	Choices[5] = KEY_R;
	
	int Keys[3];
	
	if( Button::LoadConfigFile() )
	{
		Keys[FIRE] = GetKeyIndex( Button::Config.Control[Button::CONTROL_FIRE] );
		Keys[BOMB] = GetKeyIndex( Button::Config.Control[Button::CONTROL_BOMB] );
		Keys[SLOW] = GetKeyIndex( Button::Config.Control[Button::CONTROL_SLOWDOWN] );
	}
	else
	{
		Keys[FIRE] = 1;
		Keys[BOMB] = 0;
		Keys[SLOW] = 5;	
	}
	
	int Frame = 0;
	int Finished = false;
	
	int ActiveRow = 0;
	
	int ColumnOffset = 5;
	int StartRow = 7;
	int StartColumn = 12;
	
	s32 SoundInterp = 1 << 12;
	s32 BGInterp = 0;
	int FinDelay = FADE_FRAMES;
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar6();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal2Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	while( FinDelay > 0 )
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
		
		if( (KeyD & KEY_START)  || (KeyD & KEY_B) )
		{
			if( !Finished ) 
			{
				BGInterp = 0;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
				Finished = true;
			}
		}
		
		if( (KeyD & KEY_UP) )
		{
			
			ActiveRow--;
			if( ActiveRow < 0 ) ActiveRow = 3;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_DOWN) )
		{
			ActiveRow++;
			if( ActiveRow > 3 ) ActiveRow = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
	
		if( (KeyD & KEY_A) )
		{
			switch( ActiveRow )
			{
				case 0:
					EditControls( Keys, Choices, Text, MenuText, RowCoords );
					break;
				case 1:
					AutoBomb = !AutoBomb;
					break;
				case 2:
					Keys[FIRE] = 1;
					Keys[BOMB] = 0;
					Keys[SLOW] = 5;
					AutoBomb = false;
					break;
				case 3:
					if( !Finished )
					{
						Finished = true;
						BGInterp = 0;
					}
					//break;
			}
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
			
			SiFontGL16x16.PrintCentered( 0, 10, "KEY CONFIG" );
		
			glColor( RGB15(31,31,31) );
			PrintKeyMapping( Keys, MenuText, Text, RowCoords, StartRow, StartColumn, ColumnOffset );
		
			// Cursor
			SiFontGL.PrintAscii(  10, RowCoords[ActiveRow], 65+31, 1 );
			
			// Instructions
			SiFontGL.PrintCentered( 0, 170, "PRESS A TO EDIT BUTTONS" );
			SiFontGL.PrintCentered( 0, 182, "START OR B TO GO BACK" );
			
			// AutoBomb
			if(AutoBomb)
			{
				SiFontGL.Print( 9 * 11, RowCoords[1], "ON" );
			}
			else
			{
				SiFontGL.Print( 9 * 11, RowCoords[1], "OFF" );
			}
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				s32 Finterp = BGInterp;
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										Finterp,
										FADE_STEPS );
		
			}
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
		
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "PHATCODE.NET" );
		SubFont2.PrintCentered( 0, 16 * 8, "REL.PHATCODE.NET" );
		
		// Draw
		oamUpdate(&oamSub);
		
		s32 SprInterp = BGInterp;
	
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
			
		
		}
		
	}
	
	
	Button::Control[Button::CONTROL_FIRE] = 0;
	Button::Control[Button::CONTROL_BOMB] = 0;
	Button::Control[Button::CONTROL_SLOWDOWN] = 0;
	Button::Control[Button::CONTROL_RESERVE] = 0;
	
	Button::Control[Button::CONTROL_FIRE] = Choices[Keys[FIRE]];
	Button::Control[Button::CONTROL_BOMB] = Choices[Keys[BOMB]];
	Button::Control[Button::CONTROL_SLOWDOWN] = Choices[Keys[SLOW]];
	
	//Sound::Instance()->StopBGM();
	
	

}



//--------------------------------------
//	
//--------------------------------------
void Engine::Intermission( int Stage,
						   PlayerShip &Ship )
{

	const int DONE = 60 * 10;
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const char *StagesX[] =
	{ 
		"SMEGMA",
		"AXILLA",
		"PUDENDA",
		"AREOLA",
		"GLUTEUS"
		
	};

	const char *BossesX[] =
	{ 
		"VENGADOR",
		"TRANSPORMA",
		"TENTAKILL",
		"TAKNOM",
		"MAXIMUS"
		
	};

	
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar6();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal6Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	s32 SoundInterp = 1 << 12;
	
	// fade from black sub bg
	s32 BGInterp = 0;
	
	int Frame = 0;
	int Finished = false;
	int FinDelay = FADE_FRAMES;
	
	int ShipLeft = Ship.GetLives();
	int BombsLeft = Ship.GetBombs();
	int Total = ShipLeft * 20000 + BombsLeft * 10000;
	int Score = Ship.GetScore();
			
	while( FinDelay > 0 )
	{
		Frame++;
		if( (Frame >= DONE) && (!Finished) )
		{
			Finished = true;
			BGInterp = 0;
			SoundInterp = 1 << 12;
		}
		
		scanKeys();
		int KD = keysDown();

		
		if( (KD & KEY_START) || (KD & KEY_A) || (KD & KEY_B) )
		{
			if( Frame < (60*5) )
			{
				Frame += 59;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
		}
		
				
		GlobalManager::Instance()->ResetAll();
		
		glBegin2D();
			
			glColor( RGB15(0,0,31) );
			Fx3D::Instance()->WarpScreen( Frame, Helix );
			glColor( RGB15(31,31,31) );
			
			if( Frame >= 60 )
			{
				SiFontGL.PrintCentered( 0, 32, "SHIPS LEFT" );
				if( Frame == (60 * 1)) Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
			if( Frame >= (60*2) )
			{
				SiFontGL.PrintAscii( 8+8*4, 32+16, 65+31, 1);  // ship
				SiFontGL.Print( 8+8*6, 32+16, "=>" );  
				SiFontGL.Print( 8+8*9, 32+16, ShipLeft );      // number
				SiFontGL.PrintAscii( 8+8*11, 32+16, 78+31, 1 ); // ""
				SiFontGL.Print( 8+8*13, 32+16, "20,000 = ", ShipLeft * 20000 );  
				if( Frame == (60 * 2)) Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
			if( Frame >= (60*3) )
			{
				SiFontGL.PrintCentered( 0, 32+40, "BOMBS LEFT" );
				if( Frame == (60 * 3)) Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
			
			if( Frame >= (60*4) )
			{
				SiFontGL.PrintAscii( 12+8*4, 32+40+16, 66+31, 1);  // bombs
				SiFontGL.Print( 12+8*6, 32+40+16, "=>" );  
				SiFontGL.Print( 12+8*9, 32+40+16, BombsLeft );      // number
				SiFontGL.PrintAscii( 12+8*11, 32+40+16, 78+31, 1 ); // ""
				SiFontGL.Print( 12+8*13, 32+40+16, "10,000 = ", BombsLeft * 10000 );  
				if( Frame == (60 * 4)) Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
			if( Frame >= (60*5) )
			{
				SiFontGL16x16.PrintCentered( 0, 116,"SCORE");  
				if( Frame == (60 * 5)) Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
			}
			
			if( Frame >= (60*6) )
			{	
				if( Frame == (60*6) )
				{
					Ship.AddToScore(Total);
					if( Ship.GetScore() > 9999999 )
					{
						Ship.SetScore(9999999);
					}		
					Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
				}
				SiFontGL.PrintScore( 8*3, 132 + 24, Score, "0", 7 );
				SiFontGL.Print( 8*10, 132 + 24, " + ");
				SiFontGL.PrintScore( 8*13, 132 + 24, Total, "0", 7 );
				SiFontGL.Print( 8*20, 132 + 24, " = ");
				int color = abs(sinLerp(Frame * 200) * 31) >> 12;
				glColor(RGB15(color,31-color,color));
				SiFontGL.PrintScore( 8*23, 132 + 24, Ship.GetScore(), "0", 7 );
				glColor(RGB15(31,31,31));
				
			}
				
			
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										BGInterp,
										FADE_STEPS );
		
			}
		
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
	
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 1, "STATUS REPORT" );
		SubFont2.PrintCentered( 0, 16 * 3, StagesX[Stage] );
		SubFont2.PrintCentered( 0, 16 * 6, "X" );
		SubFont2.PrintCentered( 0, 16 * 9, BossesX[Stage] );
		
		// Draw
		oamUpdate(&oamSub);
	
		
		s32 SprInterp = BGInterp;
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
		
		}
		else
		{
			FinDelay--;
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
			
		}
	
	}


	//Sound::Instance()->StopBGM();
	
	
}

//------------------------------------------------------------------------------
//	Misc
//------------------------------------------------------------------------------
int YesOrNo( int Frame, int y, const char *text, int Active )
{

	static int frame = 0;
	const int XCoord[] =  { 16*4+6*16+8, 16*3+8 };
	
	
	frame++;
	
	
	glColor( RGB15(31,31,31) );
	SiFontGL.PrintCentered( 0, y, text);
	
	
	SiFontGL16x16.PrintCentered( 0, y+24, "YES    NO");
	int color = abs(sinLerp(frame * 200) * 31) >> 12;
				
	if( Active == 0 )
	{
		glColor(RGB15(31-color,color,color));
		SiFontGL16x16.Print( XCoord[Active], y+24,  "NO" );
		glColor( RGB15(0,31,0) );
		SiFontGL16x16.Print( XCoord[Active]-16, y+24,  "<  >" );
	}
	else
	{
		glColor(RGB15(31-color,color,color));
		SiFontGL16x16.Print( XCoord[Active], y+24,  "YES" );
		glColor( RGB15(0,31,0) );
		SiFontGL16x16.Print( XCoord[Active]-16, y+24,  "<   >" );
	}
	
	glColor( RGB15(31,31,31) );
	
	
	return Active;
	
}


//--------------------------------------
//	
//--------------------------------------
void PrintCurrentReplay( int Frame, int y )
{
	
	int color = abs(sinLerp(Frame * 200) * 31) >> 12;
	
	glColor( RGB15(31,color,color) );
	SiFontGL.Print( 50, y + 0, "SHIP : " );
	SiFontGL.Print( 50, y + 14, "MODE : " );
	SiFontGL.Print( 50, y + 26, "MAXSCORE : " );
	
	glColor( RGB15(31,31,31) );
	
	// real time read header stuff
	if( Button::CanReplay )
	{
		if( Button::Replay.ShipType == 0)
			SiFontGL.Print( 50 + 8 * 7, y + 0, "RED AGILA" );
		else if ( Button::Replay.ShipType == 1)
			SiFontGL.Print( 50 + 8 * 7, y + 0, "BLUE BAGWIS" );
		else
			SiFontGL.Print( 50 + 8 * 7, y + 0, "LOCO VIPER" );
			
		
		if( Button::Replay.Mode == 0)
			SiFontGL.Print( 50 + 8 * 7, y + 14, "NORMAL" );
		else
			SiFontGL.Print( 50 + 8 * 7, y + 14, "BOSS RUSH" );
			

		SiFontGL.PrintScore( 50 + 8 * 11, y + 26, Button::Replay.MaxScore, "0", 7 );
	}
	else
	{
		SiFontGL.Print( 50 + 8 * 7, y + 0, "N/A" );
		SiFontGL.Print( 50 + 8 * 7, y + 14, "N/A" );
		SiFontGL.Print( 50 + 8 * 11, y + 26, "N/A" );
	}
	
	glColor( RGB15(31,31,31) );
	
}

//--------------------------------------
//	
//--------------------------------------
int PrintReplay( int Frame, int y, int Active, Button::ReplayType &Rep )
{

	const int XCoord[] =  { 60, 120, 180 };
	
	
	int color = abs(sinLerp(Frame * 200) * 31) >> 12;
	
	for( int i = 0; i < 3; i++ )
	{
		SiFontGL16x16.Print( XCoord[i], y+20,  i+1 );
	}
	
	glColor( RGB15(color,31,color) );
	
	SiFontGL16x16.Print( XCoord[Active]-16, y+20,  "< >" );
	
	glColor( RGB15(color,color,31) );
	SiFontGL.Print( 50, y + 40, "SHIP : " );
	SiFontGL.Print( 50, y + 54, "MODE : " );
	SiFontGL.Print( 50, y + 66, "MAXSCORE : " );
	
	glColor( RGB15(31,31,31) );
	
	// real time read header stuff
	if( Button::ReadReplayHeader( Active, Rep ) )
	{
		if( Rep.ShipType == 0 )
			SiFontGL.Print( 50 + 8 * 7, y + 40, "RED AGILA" );
		else if( Rep.ShipType == 1 )
			SiFontGL.Print( 50 + 8 * 7, y + 40, "BLUE BAGWIS" );
		else
			SiFontGL.Print( 50 + 8 * 7, y + 40, "LOCO VIPER" );
			
		if( Rep.Mode == 0)
			SiFontGL.Print( 50 + 8 * 7, y + 54, "NORMAL" );
		else
			SiFontGL.Print( 50 + 8 * 7, y + 54, "BOSS RUSH" );
			

		SiFontGL.PrintScore( 50 + 8 * 11, y + 66, Rep.MaxScore, "0", 7 );
	}
	else
	{
		SiFontGL.Print( 50 + 8 * 7, y + 40, "N/A" );
		SiFontGL.Print( 50 + 8 * 7, y + 54, "N/A" );
		SiFontGL.Print( 50 + 8 * 11, y + 66, "N/A" );
	}
	
	glColor( RGB15(31,31,31) );
	
	
	return Active;
	
}

//--------------------------------------
//	
//--------------------------------------
int LoadReplay()
{
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const int MAX_ITEMS = 3;
	
	const char *MenuText[] =
	{ 
		"REPLAY LAST GAME",
		"LOAD REPLAY",
		"EXIT "
	};
	
	int RowCoords[] = 
	{
		10, 20, 140
	};
	
	
	Button::ReplayType Rep;
	
	int Res = 2;
	int Frame = 0;
	int Finished = false;
	
	int ActiveRow = 0;		 // Row active	includes exit
	
	int Active = 0;			 // slot
	
	
	s32 SoundInterp = 1 << 12;
	s32 BGInterp = 0;
	int FinDelay = FADE_FRAMES;
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar5();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal6Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	while( FinDelay > 0 )
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
		
		if( (KeyD & KEY_START)  || (KeyD & KEY_B) )
		{
			if( !Finished ) 
			{
				BGInterp = 0;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
				Finished = true;
				Res = 2;
			}
		}
		
		if( (KeyD & KEY_UP) )
		{
			
			ActiveRow--;
			if( ActiveRow < 0 ) ActiveRow = MAX_ITEMS - 1;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_DOWN) )
		{
			ActiveRow++;
			if( ActiveRow > (MAX_ITEMS - 1) ) ActiveRow = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
			
		}
	
		if( (KeyD & KEY_RIGHT) )
		{
			Active++;
			if( Active > 2) Active = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		if( (KeyD & KEY_LEFT) )
		{
			Active--;
			if( Active < 0) Active = 2;
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		if( (KeyD & KEY_A) )
		{
			switch( ActiveRow )
			{
				case 0:				// Load Active if there is one
					if( Button::CanReplay  )
					{
						if( !Finished ) 
						{
							BGInterp = 0;
							Finished = true;
							Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
							Res = 0;
						}					
					}
					break;				
				case 1:				// Load
					if( Button::LoadReplay( Active ) )
					{
						if( !Finished ) 
						{
							BGInterp = 0;
							Finished = true;
							Button::CanReplay = true;
							Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
							Res = 1;
						}
					}
					break;
				case 2:
					if( !Finished ) Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
					BGInterp = 0;
					Finished = true;
					Res = 2;
					break;
			}
			
		}
						
			
		GlobalManager::Instance()->ResetAll();
		
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			
			SiFontGL.PrintCentered( 0, 0, "LOAD REPLAY" );
		
			// Main key config menu
			glColor( RGB15(31,31,0) );
			for( int i = 0; i < (MAX_ITEMS); i++)
			{
				SiFontGL.Print(  20, RowCoords[i], MenuText[i] );
			}
			
			// Controls Menu
			glColor( RGB15(31,31,31) );
		
			// Cursor
			SiFontGL.PrintAscii(  10, RowCoords[ActiveRow], 65+31, 1 );
			
			// Instructions
			SiFontGL.PrintCentered( 0, 158, "[RIGHT]/[LEFT] TO CHANGE SLOTS" );
			SiFontGL.PrintCentered( 0, 170, "[A] TO CHOOSE MODE" );
			SiFontGL.PrintCentered( 0, 182, "[START]/[B] TO GO BACK" );
		
			// Print replay
			PrintReplay( Frame, 60, Active, Rep );
			
			glColor( RGB15(31,0,31) );
			SiFontGL.PrintCentered( 0, 30, "ACTIVE REPLAY" );
			PrintCurrentReplay( Frame, 42 );
			
		
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				s32 Finterp = BGInterp;
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										Finterp,
										FADE_STEPS );
		
			}
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
		
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "GBATEMP.NET" );
		SubFont2.PrintCentered( 0, 16 * 8, "MODARIFFIC.COM" );
		
		// Draw
		oamUpdate(&oamSub);
		
		s32 SprInterp = BGInterp;
	
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			
		
		}
		
	}
	
	
	
	//Sound::Instance()->StopBGM();
	
	return Res;
}


//--------------------------------------
//	
//--------------------------------------
void SaveReplay()
{
	const int FADE_FRAMES = 60*3;
	const int FADE_STEPS = 4096/FADE_FRAMES;
	
	const int MAX_ITEMS = 2;
	
	const char *MenuText[] =
	{ 
		"SAVE REPLAY",
		"EXIT "
	};
	
	int RowCoords[] = 
	{
		15, 140
	};
	
	
	Button::ReplayType Rep;
	
	int Frame = 0;
	int Finished = false;
	
	int ActiveRow = 0;		 // Row active	includes exit
	
	int Active = 0;			 // slot
	
	
	s32 SoundInterp = 1 << 12;
	s32 BGInterp = 0;
	int FinDelay = FADE_FRAMES;
	
	u8* Plaz = Fx2D::Instance()->PlasmaPolar5();
	
	DC_FlushRange( Plaz, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( Plaz, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( plasma_pal6Pal );
	
	
	Sound::Instance()->PlayBGM( E_BGM_STORY, MM_PLAY_LOOP );
	Sound::Instance()->SetMasterVolume( 512 );
	
	while( FinDelay > 0 )
	{
		Frame++;
		
		scanKeys();
		int KeyD = keysDown();
		
		if( (KeyD & KEY_START)  || (KeyD & KEY_B) )
		{
			if( !Finished ) 
			{
				BGInterp = 0;
				Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
				Finished = true;
			}
		}
		
		if( (KeyD & KEY_UP) )
		{
			
			ActiveRow--;
			if( ActiveRow < 0 ) ActiveRow = (MAX_ITEMS - 1);
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
		}
		
		if( (KeyD & KEY_DOWN) )
		{
			ActiveRow++;
			if( ActiveRow > (MAX_ITEMS - 1) ) ActiveRow = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_CHANGE );
			
		}
	
		if( (KeyD & KEY_RIGHT) )
		{
			Active++;
			if( Active > 2) Active = 0;
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		if( (KeyD & KEY_LEFT) )
		{
			Active--;
			if( Active < 0) Active = 2;
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
		
		if( (KeyD & KEY_A) )
		{
			switch( ActiveRow )
			{
				case 0:				// Save
					Button::SaveReplay( Active );
					break;
				case 1:
					BGInterp = 0;
					Finished = true;
					break;
			}
			Sound::Instance()->PlaySFX( E_SFX_MENU_OK );
		}
						
			
		GlobalManager::Instance()->ResetAll();
		
		
		glBegin2D();
			
			glColor( RGB15(31,31,31) );
			
			SiFontGL.PrintCentered( 0, 0, "SAVE REPLAY" );
		
			// Main key config menu
			glColor( RGB15(31,31,0) );
			for( int i = 0; i < (MAX_ITEMS); i++)
			{
				SiFontGL.Print(  20, RowCoords[i], MenuText[i] );
			}
			
			// Controls Menu
			glColor( RGB15(31,31,31) );
		
			// Cursor
			SiFontGL.PrintAscii(  10, RowCoords[ActiveRow], 65+31, 1 );
			
			// Instructions
			SiFontGL.PrintCentered( 0, 158, "[RIGHT]/[LEFT] TO CHANGE SLOTS" );
			SiFontGL.PrintCentered( 0, 170, "[A] TO CHOOSE MODE" );
			SiFontGL.PrintCentered( 0, 182, "[START]/[B] TO GO BACK" );
		
			// Print replay
			PrintReplay( Frame, 60, Active, Rep );
			
			glColor( RGB15(31,0,31) );
			SiFontGL.PrintCentered( 0, 28, "ACTIVE REPLAY" );
			PrintCurrentReplay( Frame, 40 );
			
		
			// Fade
			if( Finished )
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				int Color1 = RGB15(R,G,B);
				int Color2 = RGB15(B,R,G);
				int Color3 = RGB15(G,B,R);
				int Color4 = RGB15(R,G,B);
		
				s32 Finterp = BGInterp;
				Fx3D::Instance()->Fade( Color1,
										Color2,
										Color3,
										Color4,
										Finterp,
										FADE_STEPS );
		
			}
			
		glEnd2D();

		glFlush(0);                    

		swiWaitForVBlank();
		
		
		SubFont.Clear(); // deactivate every oam per frame
		SubFont2.Clear(); // deactivate every oam per frame
	
		SubFont2.PrintCentered( 0, 16 * 3, "DEVKITPRO.ORG" );
		SubFont2.PrintCentered( 0, 16 * 8, "GBADEV.ORG" );
		
		// Draw
		oamUpdate(&oamSub);
		
		s32 SprInterp = BGInterp;
	
		if( !Finished )
		{
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Palettes::Instance()->GetBlack(),
								Fx2D::Instance()->GetPlasmaPal(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			
			// fade from black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									Palettes::Instance()->GetBlack(),
									viper_spritePal,
									SprInterp,
									FADE_STEPS,
									512
								  );
			
		}
		else
		{
			FinDelay--;
			Fx2D::Instance()->CyclePlasmaPal();
			Fx2D::Instance()->Fade( BG_PALETTE_SUB,
								Fx2D::Instance()->GetPlasmaPal(),
								Palettes::Instance()->GetBlack(),
								BGInterp,
								FADE_STEPS,
								512
							  );
			// fade to black sub OAM
			Fx2D::Instance()->Fade( SPRITE_PALETTE_SUB,
									viper_spritePal,
									Palettes::Instance()->GetBlack(),
									SprInterp,
									FADE_STEPS,
									512
								  );
			
			Sound::Instance()->FadeBGM( SoundInterp, -FADE_STEPS );
			
		
		}
		
	}
	
	
	
	//Sound::Instance()->StopBGM();
	

}

//--------------------------------------
//	
//--------------------------------------
void PrintScores( Engine::HighScore Scores[], int ScoreMode, int Frame, int CurrentPlayerIndex, int ColOffset )
{
	
	//ScoreMode 0 = normal, 1 = bossrush
	
	int Columns[3] = { 5, 95-8, 195-8 };
	
	const char *Title[] = 
	{
		" NAME",
		"  SHIP",
		" SCORE"
	};
	
	
		glColor(RGB15(16,31,12));
		if( ScoreMode == 0 )
			SiFontGL.Print( ColOffset + 8 * 11 + 4, 18, " NORMAL  " );
		else
			SiFontGL.Print( ColOffset + 8 * 10 + 4, 18, "BOSS RUSH" );
			
		glColor(RGB15(0,31,31));
		for( int i = 0; i < 3; i++ )
		{
			SiFontGL.Print( ColOffset + Columns[i], 40, Title[i] );
		}
		
		for( int i = 0; i < 10; i++ )
		{
			if( i == CurrentPlayerIndex )
			{
				glColor(RGB15(31,31,0));
				SiFontGL.PrintSine(ColOffset + Columns[0], 55 + i * 14, Scores[i].Name, 0, 2, 3, Frame * 512);
			}
			else
			{
				glColor(RGB15(31,31,31));
				SiFontGL.Print( ColOffset + Columns[0], 55 + i * 14, Scores[i].Name );
			}
			
			glColor(RGB15(31,31,0));
			SiFontGL.Print( ColOffset + Columns[1], 55 + i * 14, Scores[i].Ship );
			glColor(RGB15(31,31,31));
			SiFontGL.PrintScore( ColOffset + Columns[2], 55 + i * 14, Scores[i].Score, " ", 7 );
	
		}
		
		
	
}

//--------------------------------------
//	
//--------------------------------------
void SavePlayerName( char *PlayerName )
{
		
	FILE *NameFile;
	
	char name[10];
	
	strcpy( name, PlayerName );
	
	NameFile = fopen( "space_impakto_ds.cfg", "wb" );
	
	if( NameFile )
	{
		fwrite( &name, sizeof(name), 1, NameFile );
		fclose( NameFile );
	} 
}

//--------------------------------------
//	
//--------------------------------------
void LoadPlayerName( char *PlayerName )
{
		
	FILE *NameFile;
	
	char name[10] = "";
	NameFile = fopen( "space_impakto_ds.cfg", "rb" );
	
	if( NameFile )
	{
		fread( &name, sizeof(name), 1, NameFile );
		fclose( NameFile );
		strcpy( PlayerName, name );
	} 
	/*else
	{
		const char *filler = "-";
		for (int i = 0; i < 10; i++)
		{
			PlayerName[i] = *filler;
		}		
	}*/
	
	
}


//--------------------------------------
//	No Intermission for demomode
//--------------------------------------
void NoIntermission( PlayerShip &Ship )
{

	
	int ShipLeft = Ship.GetLives();
	int BombsLeft = Ship.GetBombs();
	int Total = ShipLeft * 20000 + BombsLeft * 10000;
	
	Ship.AddToScore(Total);
	if( Ship.GetScore() > 9999999 )
	{
		Ship.SetScore(9999999);
	}		
			
}


//--------------------------------------
//	
//--------------------------------------
int GetKeyIndex( int ControlButton  )
{
	
	switch( ControlButton )
	{
		case KEY_A:
			return 0;
			break;
		case KEY_B:
			return 1;
			break;
		case KEY_X:
			return 2;
			break;
		case KEY_Y:
			return 3;
			break;
		case KEY_L:
			return 4;
			break;
		case KEY_R:
			return 5;
			break;
	}
	
	return -1;
}
	

//------------------------------------------------------------------------------
//	ENEMY SPAWNERS
//------------------------------------------------------------------------------
void SpawnLoopersWave( bool mirrored, TurretArray &Turrets )
{
	for( int i = 0; i < 8; i++  )
	{
		if( !mirrored )
			Loopers.Spawn( bezier_loop_coord, bezier_loop_table, 
						  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
						  BEZIER_LOOP_TOTAL_DISTANCE, i * 16, i & 3, Turrets );
		else
			Loopers.Spawn( bezier_loop_coord_mirrored, bezier_loop_table, 
						  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
						  BEZIER_LOOP_TOTAL_DISTANCE, i * 16, i & 3, Turrets );
	}				
					
}

void SpawnPlatosWave( bool mirrored, TurretArray &Turrets )
{
	for( int i = 0; i < 8; i++  )
	{
		if( !mirrored )
			Platos.Spawn( bezier_wave_coord, bezier_wave_table, 
						  BEZIER_WAVE_NUM_ELEMENTS, BEZIER_WAVE_RATE, 
						  BEZIER_WAVE_TOTAL_DISTANCE, i * 12, i & 3, Turrets );						  
		else
			Platos.Spawn( bezier_wave_coord_mirrored, bezier_wave_table, 
						  BEZIER_WAVE_NUM_ELEMENTS, BEZIER_WAVE_RATE, 
						  BEZIER_WAVE_TOTAL_DISTANCE, i * 12, i & 3, Turrets );

	}				
					
}

void SpawnFrisbeesWave( bool mirrored, TurretArray &Turrets )
{
	for( int i = 0; i < 8; i++  )
	{
		if( !mirrored )
			Frisbees.Spawn( bezier_circle_coord, bezier_circle_table, 
						  BEZIER_CIRCLE_NUM_ELEMENTS, BEZIER_CIRCLE_RATE, 
						  BEZIER_CIRCLE_TOTAL_DISTANCE, i * 12, i & 3, Turrets );						  
		else
			Frisbees.Spawn( bezier_circle_coord_mirrored, bezier_circle_table, 
						  BEZIER_CIRCLE_NUM_ELEMENTS, BEZIER_CIRCLE_RATE, 
						  BEZIER_CIRCLE_TOTAL_DISTANCE, i * 12, i & 3, Turrets );

	}				
					
}

void SpawnRandomElectricBlobs( int frame, TurretArray &Turrets)
{
	if( (frame & 127) == 0 )
		ElectricBlobs.Spawn(UTIL::RND_RANGE(10,180), UTIL::RND_RANGE(0, 12121), Turrets );					
}

void SpawnRandomWorms( int frame, TurretArray &Turrets)
{
	if( (frame & 127) == 0 )
		Worms.Spawn(UTIL::RND_RANGE(10,180), UTIL::RND_RANGE(0, 12121), Turrets );
}

void SpawnRandomMahorns( int frame, TurretArray &Turrets)
{
	if( (frame & 511) == 0 )
	{
		int randy = UTIL::RND_RANGE(0,2);
		switch(randy)
		{
			case 0:
				Mahorns.Spawn(false, Turrets );
				break;
			case 1:
				Mahorns.Spawn(true, Turrets );
				break;
			case 2:
				Mahorns.Spawn(false, Turrets );
				Mahorns.Spawn(true, Turrets );
		}
	}
}


void SpawnRandomBumbleBees( int frame, TurretArray &Turrets)
{
	if( (frame & 511) == 0 )
	{
		int randy = UTIL::RND_RANGE(0,2);
		switch(randy)
		{
			case 0:
				BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), false, Turrets );
				break;
			case 1:
				BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), true, Turrets );
				break;
			case 2:
				BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), false, Turrets );
				BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), true, Turrets );
		}
	}
}


void SpawnRandomPhoenixs( int frame, TurretArray &Turrets)
{
	if( (frame & 511) == 0 )
	{
		int randy = UTIL::RND_RANGE(0,2);
		switch(randy)
		{
			case 0:
				Phoenixs.Spawn(200, -40, false, Turrets );				
				break;
			case 1:
				Phoenixs.Spawn(200, 192+40, true, Turrets );
				break;
			case 2:
				Phoenixs.Spawn(200, -40, false, Turrets );
				Phoenixs.Spawn(200, 192+40, true, Turrets );
		}
	}
}

void SpawnRandomDivers( int frame, TurretArray &Turrets )
{
	if( (frame & 31) == 0 )
		Divers.Spawn( UTIL::RND_RANGE(300, 460), UTIL::RND_RANGE(16, 192-16), Turrets );
}

void SpawnRandomAbtikBawis( int frame, TurretArray &Turrets )
{
	if( (frame & 63) == 0 )
	{
		bool normal = UTIL::RND_RANGE(0, 512) > 256;
		int tdelay = UTIL::RND_RANGE(0, 512);
		int yoffset = UTIL::RND_RANGE(40, 80);
		if( normal )
			AbtikBawis.Spawn( bezier_stopnback_coord, bezier_loop_table, 
					  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
					  BEZIER_LOOP_TOTAL_DISTANCE, tdelay, -yoffset, Turrets );
		else
			AbtikBawis.Spawn( bezier_stopnback_coord_mirrored, bezier_loop_table, 
				  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
				  BEZIER_LOOP_TOTAL_DISTANCE, tdelay, yoffset, Turrets );
	}
	
}

void SpawnRandomEverything( int frame, TurretArray &Turrets )
{
	//002 = Diver
	//003 = Kalavera
	//004 = Amoeba
	//005 = Popcorn
	//006 = Plato top
	//007 = Plato Below
	//008 = Frisbee top
	//009 = frisbee below
	
	if( (frame & 511) == 0 )
	{
		int Enemy = UTIL::RND_RANGE(0, 9);
		int y = UTIL::RND_RANGE(16, 192-16);
		switch( Enemy )
		{
			case 1:
				Divers.Spawn( UTIL::RND_RANGE(300, 460), UTIL::RND_RANGE(16, 192-16), Turrets );
				break;
			case 2:
				Kalaveras.Spawn( y, y, Turrets );
				break;
			case 3:		//
				Amoebas.Spawn( y, UTIL::RND_RANGE(0,191), Turrets );				
				break;
			case 4:		//
				Popcorns.Spawn( y, y, Turrets );				
				break;
			case 5:		//
				Platos.Spawn( bezier_wave_coord, bezier_wave_table, 
							  BEZIER_WAVE_NUM_ELEMENTS, BEZIER_WAVE_RATE, 
							  BEZIER_WAVE_TOTAL_DISTANCE, 0, 0, Turrets );						  						
				break;
			case 6:		//
				Platos.Spawn( bezier_wave_coord_mirrored, bezier_wave_table, 
							  BEZIER_WAVE_NUM_ELEMENTS, BEZIER_WAVE_RATE, 
							  BEZIER_WAVE_TOTAL_DISTANCE, 0, 0, Turrets );
				break;
			case 7:		//
				Frisbees.Spawn( bezier_circle_coord, bezier_circle_table, 
							  BEZIER_CIRCLE_NUM_ELEMENTS, BEZIER_CIRCLE_RATE, 
							  BEZIER_CIRCLE_TOTAL_DISTANCE, 0, 0, Turrets );						  
				break;
			case 8:		//
				Frisbees.Spawn( bezier_circle_coord_mirrored, bezier_circle_table, 
							  BEZIER_CIRCLE_NUM_ELEMENTS, BEZIER_CIRCLE_RATE, 
							  BEZIER_CIRCLE_TOTAL_DISTANCE, 0, 0, Turrets );
				break;
			default:
			Divers.Spawn( UTIL::RND_RANGE(300, 460), UTIL::RND_RANGE(16, 192-16), Turrets );
			break;

		}
	}
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void GameEnemySequenceStage1( int Frame, const PlayerShip &Ship, TurretArray &Turrets )
{
	/*
	 * Loopers
	 * Platos
	 * AbtikBawis
	 * BumbleBees
	 * Divers
	 * ElectricBlobs
	 * Mahorns
	 * Phoenixs
	 * Worms
	 * Centipedes
	 */
	 
	const int SPAWN_DOUBLE_PLATOS = 60 * 3;
	const int SPAWN_DOUBLE_LOOPERS = 60 * 7;
	const int SPAWN_BUMBLE_BEE = 60 * 12;
	const int SPAWN_MAHORN_UP = 60 * 30;
	const int SPAWN_PHOENIX = 60 * 45;
	const int SPAWN_CENTIPEDE = 60 * 100;
	const int SPAWN_MAHORN_DOWN = 60 * 130;
	const int SPAWN_MAHORN_BOTH = 60 * 140;
	const int SPAWN_DOUBLE_BBS = 60 * 150;
	const int SPAWN_DOUBLE_PHS = 60 * 160;
	

	const int START_DIVER = 60 * 8;
	const int END_DIVER = 60 * 25;
	const int START_WORM = 60 * 35;
	const int END_WORM = 60 * 46;
	const int START_E_BLOBS = 60 * 55;
	const int END_E_BLOBS = 60 * 75;
	const int START_ABTIKBAWI = 60 * 78;
	const int END_ABTIKBAWI = 60 * 90;
	const int START_DIVER2 = 60 * 115;
	const int END_DIVER2 = 60 * 135;
	
	if( Ship.GetLevel() < 4 && ((Frame & 1023) == 0) )
		BonusCarriers.Spawn( 300, UTIL::RND_RANGE(10, 180) );

	if( (Frame >= START_DIVER) && (Frame <= END_DIVER) )
	{
		SpawnRandomDivers( Frame, Turrets );
	}
	
	if( (Frame >= START_WORM) && (Frame <= END_WORM) )
	{
		SpawnRandomWorms( Frame, Turrets );
	}
	
	if( (Frame >= START_E_BLOBS) && (Frame <= END_E_BLOBS) )
	{
		SpawnRandomElectricBlobs( Frame, Turrets );
	}
	
	if( (Frame >= START_ABTIKBAWI) && (Frame <= END_ABTIKBAWI) )
	{
		SpawnRandomAbtikBawis( Frame, Turrets );
	}
	
	if( (Frame >= START_DIVER2) && (Frame <= END_DIVER2) )
	{
		SpawnRandomDivers( Frame, Turrets );
	}
	
	switch( Frame )
	{
		case SPAWN_DOUBLE_PLATOS:
			SpawnPlatosWave( false, Turrets );
			SpawnPlatosWave( true, Turrets );
			break;
		case SPAWN_DOUBLE_LOOPERS:
			SpawnLoopersWave( false, Turrets );
			SpawnLoopersWave( true, Turrets );
			break;
		case SPAWN_BUMBLE_BEE:
			BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), false, Turrets );
			break;
		case SPAWN_MAHORN_UP:
			Mahorns.Spawn(false, Turrets );				
			break;
		case SPAWN_PHOENIX:
			Phoenixs.Spawn(300, UTIL::RND_RANGE(16, 192-16), true, Turrets );
			break;
		case SPAWN_CENTIPEDE:
			Centipedes.Spawn( 300, UTIL::RND_RANGE(16, 192-16), Turrets );
			break;
		case SPAWN_MAHORN_DOWN:
			Mahorns.Spawn(true, Turrets );
			break;
		case SPAWN_MAHORN_BOTH:
			Mahorns.Spawn(false, Turrets );
			Mahorns.Spawn(true, Turrets );
			break;
		case SPAWN_DOUBLE_BBS:
			BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), false, Turrets );
			BumbleBees.Spawn(300, UTIL::RND_RANGE(16, 192-16), true, Turrets );
			break;
		case SPAWN_DOUBLE_PHS:
			Phoenixs.Spawn(300, UTIL::RND_RANGE(16, 192-16), false, Turrets );
			Phoenixs.Spawn(300, UTIL::RND_RANGE(16, 192-16), true, Turrets );
			break;
		
	}
}

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void GameEnemySequence( int Frame, const PlayerShip &Ship, TurretArray &Turrets, Level &Lvl )
{
	
//	Enemies.push_back( &Centipedes );
//	Enemies.push_back( &Rotators );
//	Enemies.push_back( &Boogers );
//	Enemies.push_back( &Mahorns );
//	Enemies.push_back( &BumbleBees );
//	Enemies.push_back( &Alienators );
//	Enemies.push_back( &Cannoneers );
//	Enemies.push_back( &Phoenixs );
//	Enemies.push_back( &Tictanics );
//	Enemies.push_back( &LightBalls );
//	Enemies.push_back( &Batteries );
//	Enemies.push_back( &AbtikBawis );
//	Enemies.push_back( &Loopers );
//	Enemies.push_back( &Worms );
//	Enemies.push_back( &ElectricBlobs );
//	Enemies.push_back( &Frisbees );
//	Enemies.push_back( &Platos );
//	Enemies.push_back( &Popcorns );
//	Enemies.push_back( &Amoebas );
//	Enemies.push_back( &Kalaveras );
//	Enemies.push_back( &Divers );
//	Enemies.push_back( &BonusCarriers );
//	Enemies.push_back( &PowerUps );
	
	if( Ship.GetLevel() < 4 && ((Frame & 1023) == 0) )
		BonusCarriers.Spawn( 300, UTIL::RND_RANGE(10, 180) );
	
	for( int i = 0; i < 12; i++)
	{
		int EnemyID = Lvl.GetEnemyID(Frame, i );
		if( EnemyID > 0 )
		{
			int y = i * 16 + 8;
			switch( EnemyID )
			{
				case 1:		//
					BonusCarriers.Spawn( 300, y );
					break;
				case 2:		//
					Divers.Spawn( 300, y, Turrets );
					break;
				case 3:		//
					Kalaveras.Spawn( y, y, Turrets );
					break;
				case 4:		//
					Amoebas.Spawn( y, UTIL::RND_RANGE(0,191), Turrets );				
					break;
				case 5:		//
					Popcorns.Spawn( y, y, Turrets );				
					break;
				case 6:		//
					Platos.Spawn( bezier_wave_coord, bezier_wave_table, 
								  BEZIER_WAVE_NUM_ELEMENTS, BEZIER_WAVE_RATE, 
								  BEZIER_WAVE_TOTAL_DISTANCE, 0, 0, Turrets );						  						
					break;
				case 7:		//
					Platos.Spawn( bezier_wave_coord_mirrored, bezier_wave_table, 
								  BEZIER_WAVE_NUM_ELEMENTS, BEZIER_WAVE_RATE, 
								  BEZIER_WAVE_TOTAL_DISTANCE, 0, 0, Turrets );
					break;
				case 8:		//
					Frisbees.Spawn( bezier_circle_coord, bezier_circle_table, 
								  BEZIER_CIRCLE_NUM_ELEMENTS, BEZIER_CIRCLE_RATE, 
								  BEZIER_CIRCLE_TOTAL_DISTANCE, 0, 0, Turrets );						  
					break;
				case 9:		//
					Frisbees.Spawn( bezier_circle_coord_mirrored, bezier_circle_table, 
								  BEZIER_CIRCLE_NUM_ELEMENTS, BEZIER_CIRCLE_RATE, 
								  BEZIER_CIRCLE_TOTAL_DISTANCE, 0, 0, Turrets );
					break;
				case 10:		//
					ElectricBlobs.Spawn( y, UTIL::RND_RANGE(0, 12121), Turrets );
					break;
				case 11:		//
					Worms.Spawn( y, UTIL::RND_RANGE(0, 12121), Turrets );
					break;
				case 12:		//
					Loopers.Spawn( bezier_loop_coord, bezier_loop_table, 
								  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
								  BEZIER_LOOP_TOTAL_DISTANCE, 0, 0, Turrets );
					break;
				case 13:		//
					Loopers.Spawn( bezier_loop_coord_mirrored, bezier_loop_table, 
								  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
								  BEZIER_LOOP_TOTAL_DISTANCE, 0, 0, Turrets );				
					break;
				case 14:		//
					AbtikBawis.Spawn( bezier_stopnback_coord, bezier_loop_table, 
							  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
							  BEZIER_LOOP_TOTAL_DISTANCE, UTIL::RND_RANGE(0, 512), -UTIL::RND_RANGE(40, 80), Turrets );
					break;
				case 15:		//
					AbtikBawis.Spawn( bezier_stopnback_coord_mirrored, bezier_loop_table, 
						  BEZIER_LOOP_NUM_ELEMENTS, BEZIER_LOOP_RATE, 
						  BEZIER_LOOP_TOTAL_DISTANCE, UTIL::RND_RANGE(0, 512), UTIL::RND_RANGE(40, 80), Turrets );				
					break;
				case 16:		//
					Batteries.Spawn(false, Turrets );				
					break;
				case 17:		//
					Batteries.Spawn(true, Turrets );				
					break;
				case 18:		//
					LightBalls.Spawn(false, Turrets );				
					break;
				case 19:		//
					LightBalls.Spawn(true, Turrets );				
					break;
				case 20:		//
					Tictanics.Spawn(200, -40, false, Turrets );				
					break;
				case 21:		//
					Tictanics.Spawn(200, 196+40, true, Turrets );				
					break;
				case 22:		//
					Phoenixs.Spawn(300, y, false, Turrets );
					break;
				case 23:		//
					Phoenixs.Spawn(300, y, true, Turrets );
					break;
				case 24:		//
					Cannoneers.Spawn(50, -40, false, Turrets );
					break;
				case 25:		//
					Cannoneers.Spawn(50, 196+40, true, Turrets );				
					break;
				case 26:		//
					Alienators.Spawn(50, -40, false, Turrets );				
					break;
				case 27:		//
					Alienators.Spawn(50, 196+40, true, Turrets );			
					break;
				case 28:		//
					BumbleBees.Spawn(300, y, false, Turrets );
					break;
				case 29:		//
					BumbleBees.Spawn(300, y, true, Turrets );
					break;
				case 30:		//
					Mahorns.Spawn(false, Turrets );				
					break;
				case 31:		//
					Mahorns.Spawn(true, Turrets );				
					break;
				case 32:		//
					Boogers.Spawn(300, y, false, Turrets );
					break;
				case 33:		//
					Boogers.Spawn(300, y, true, Turrets );
					break;
				case 34:		//
					Rotators.Spawn(350, y, false,Turrets );
					break;
				case 35:		//
					Rotators.Spawn(350, y, true,Turrets );
					break;
				case 36:		//
					Centipedes.Spawn( 300, y, Turrets );
					break;
				case 37:		//
					
					break;
				case 38:		//
				
					break;
				case 39:		//
					
					break;
					// ==================== Waves ==============================
				case 128:
					SpawnPlatosWave( false, Turrets );
					break;
				case 129:
					SpawnPlatosWave( true, Turrets );
					break;
				case 130:
					SpawnPlatosWave( false, Turrets );
					SpawnPlatosWave( true, Turrets );
					break;
				case 131:
					SpawnFrisbeesWave( false, Turrets );
					break;
				case 132:
					SpawnFrisbeesWave( true, Turrets );
					break;
				case 133:
					SpawnFrisbeesWave( false, Turrets );
					SpawnFrisbeesWave( true, Turrets );
					break;
				case 134:
					SpawnLoopersWave( false, Turrets );
					break;
				case 135:
					SpawnLoopersWave( true, Turrets );
					break;
				case 136:
					SpawnLoopersWave( false, Turrets );
					SpawnLoopersWave( true, Turrets );
					break;
				case 137:
					break;
				case 138:
					break;
				case 139:
					break;
				case 140:
					break;
				case 141:
					break;
				case 142:
					break;
				case 143:
					break;
				case 144:
					break;
				case 145:
					break;
				case 146:
					break;
				case 147:
					break;
				case 148:
					break;
				case 149:
					break;
				case 150:
					break;
				case 151:
					break;
				
			}
		}
	}
		
}

//--------------------------------------
//	
//--------------------------------------
void Engine::SaveHiScore()
{

			
	
	FILE *HiScoreFile = fopen( "space_impakto_ds.his", "wb" );
	if( HiScoreFile )
	{
		fwrite( Scores, sizeof(Engine::HighScore), 10, HiScoreFile );
		fclose( HiScoreFile );
	} 
	
	FILE *HiScoreFileBR = fopen( "space_impakto_ds_br.his", "wb" );
	if( HiScoreFileBR )
	{
		fwrite( BRScores, sizeof(Engine::HighScore), 10, HiScoreFileBR );
		fclose( HiScoreFileBR );
	} 
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::LoadHiScore()
{

	FILE *HiScoreFile = fopen( "space_impakto_ds.his", "rb" );
	if( HiScoreFile ) 
	{
		fread( Scores, sizeof(Engine::HighScore), 10, HiScoreFile );
		fclose( HiScoreFile );
	} 
	
	
	FILE *HiScoreFileBR = fopen( "space_impakto_ds_br.his", "rb" );
	if( HiScoreFileBR )
	{
		fread( BRScores, sizeof(Engine::HighScore), 10, HiScoreFileBR );
		fclose( HiScoreFileBR );
	} 
	
	// Set Current HS to top score
	CurrentHighScore = Scores[0].Score;
	
}

//--------------------------------------
//	
//--------------------------------------
void Engine::PlayGame()
{
  
	
	
	InitHardware();
	InitEverything();
	
	LoadSounds();
	LoadFonts();
	LoadImages();
	Load3D();
	
	// Singletons
	GlobalManager::Instance()->Init();
	Targets::Instance()->Init();
	Palettes::Instance()->Init();
	Explosion::Instance()->Init( ViperSpriteTexPal, ExplosionsImages );
	
	
	std::vector<PlayerShip*> Players;
	
	std::vector<EntityContainer*> Enemies;
	
	std::vector<Boss*> Bosses;
	
	GameInitAllEntities( Players,
					     Bombs,
					     Bullets,
					     Turrets,
						 Enemies,
						 Bosses );
	
	
	
	
	int Frame = 0;
	bool IsPaused = false;
	bool StageFinished = false;
	bool PlayerExited = false;
	bool DrawAABB = !true;
	bool DrawDebug = !true;
	int GameMode = 0;
	int Stage = 0;
	s32 FadeInterp = 0;
	s32 BGInterp = 0;
	s32 SoundInterp = 1 << 12;
	s32 FontInterp = 0;
	bool FadeOut = false;
	int CurrentBoss = 0;
	bool GameActive = true;
	bool GameOver = false;
	int CanContinueCounter = 60 * 13;
	int CanPressStartCounter = 60 * 3;
	bool PrintGameOver = false;
	Button::CanReplay = false;
	bool HasContinued = false;
	bool ForceExited = false;
	int YorNactive = 0;  // not used here
	
	
	Splash();
	
	lcdSwap();
	
	// Do the story screen 
	Story();
	
	lcdSwap();
	
				
	Button::LoadReplay(3);
	
	// temp
	// consoleDemoInit();   

while( 1 )
{

Stage = 0;
CurrentBoss = 0;
GameActive = true;
GameOver = false;
CanContinueCounter = 60 * 13;
CanPressStartCounter = 60 * 3;
PrintGameOver = false;
HasContinued = false;
NumContinues = 0;	
LoopNumber = 0;
//LoopNumber = 1;    // temp
	
// Select Player
IsReplay = false;
//int CurrentShip = 2;  // temp
//GameMode = 1;	//temp
//SetMode( GameMode );  // temp
	
//int CurrentShip = SelectPlayer();   // temp!!!!
//if( CurrentShip < 0 ) CurrentShip = 0; //temp!!!


// Load ConfigFile if there is one
if( Button::LoadConfigFile() )
{
	strcpy( PlayerStat.Name, Button::Config.Name  );
	AutoBomb = Button::Config.AutoBomb;
	for( int i = 0; i < 8; i++ )
		Button::Control[i] = Button::Config.Control[i];
}		
	

CheatOn = false;
int CurrentShip = MainMenu( GameMode, IsReplay );
Players[CurrentShip]->SetControlType( 0 );
Players[CurrentShip]->SetLives( 2 );
Players[CurrentShip]->SetScore( 0 );
Players[CurrentShip]->SetBombs( 2 );

Players[CurrentShip]->NewGame();

ForceExited = false;    // if has exited via pause. Here because I want to disable replay when emergency exited


// Save replay variables when not in replay mode
// Also clear buttons
if( !IsDemo )
{
	if( !IsReplay )
	{
		Button::Clear();
		Button::Replay.Mode = GameMode;
		Button::Replay.Stage = Stage;
		Button::Replay.ShipType = Players[CurrentShip]->GetID();
		Button::Replay.ShipLevel = Players[CurrentShip]->GetLevel();
		Button::Replay.ShipLives = Players[CurrentShip]->GetLives();
		Button::Replay.ShipBombs = Players[CurrentShip]->GetBombs();
		Button::Replay.Score = Players[CurrentShip]->GetScore();
		Button::Replay.MaxScore = 0;
		Button::Replay.AutoBomb = AutoBomb;
		Players[CurrentShip]->SetAutoBomb( AutoBomb );
		if( CheatOn ) 
		{
			Players[CurrentShip]->SetLives(99);
			Players[CurrentShip]->SetBombs(99);
			Players[CurrentShip]->SetAutoBomb(true);
			Button::Replay.AutoBomb = true;
		}
	}
	else
	{
		
	
		CurrentShip = (int)Replay( *Players[CurrentShip], GameMode, Stage );
		Players[CurrentShip]->SetControlType( 1 );   // Set control type to replay
		Players[CurrentShip]->LoadReplay( Button::Replay );
		Button::Replay.MaxScore = 0;
		//Players[CurrentShip]->SetAutoBomb( Button::Replay.AutoBomb );
	}
}
else   // demo mode	
{

	IsReplay = true;
	
	Button::LoadDemo( demo_bin );
	
	CurrentShip = (int)Replay( *Players[CurrentShip], GameMode, Stage );
	Players[CurrentShip]->SetControlType( 1 );   // Set control type to replay
	
	
	Players[CurrentShip]->LoadReplay( Button::Replay );
	Button::Replay.MaxScore = 0;
	
	Players[CurrentShip]->SetAutoBomb(true);
	Button::Replay.AutoBomb = true;
	SetMode( GameMode );
}



// Set Fadeout vars here to hide some artifacts while loading the plasma
FadeOut = false;
FadeInterp = 0;

// temp
//Stage = 3;
//CurrentBoss = 3;

FirstScore = 0;
PlayerStat.Score = 0;

while( GameActive )
{
	
	Frame = 0;
	StageFinished = false;
	PlayerExited = false;
	SoundInterp = 1 << 12;
	GameOver = false;
	GameActive = true;
	FadeOut = false;
	FadeInterp = 0;
	HasContinued = false;
	
	Button::CanReplay = true;
	
	InitSubScreen();
	
	UTIL::ResetRND();
	
	Players[CurrentShip]->NewStage();
	// temp
	//Players[CurrentShip]->SetLives(8);
	//Players[CurrentShip]->SetBombs(8);
	
	// Kill everything that moves( or doesn't )
	Players[CurrentShip]->KillArmament();
	Bullets.KillEntities();
	Turrets.KillEntities();
	Explosion::Instance()->KillEntities();
	Bombs.Deactivate();
	
	std::vector<EntityContainer*>::iterator iter;
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->KillAllEntities();
	}
	
	// Load Levels here
	switch( Stage )
	{
		case 0:
			Lvl.LoadEnemySequence( simap_level_01_map );
			Sound::Instance()->PlayBGM( E_BGM_LEVEL01, MM_PLAY_LOOP );
			Sound::Instance()->SetMasterVolume( 1024 );
			break;
		case 1:
			Lvl.Init( ViperSpriteTexPal, Stage02BG, 2 );
			Lvl.LoadEnemySequence( simap_level_02_map );
			Sound::Instance()->PlayBGM( E_BGM_LEVEL02, MM_PLAY_LOOP );
			Sound::Instance()->SetMasterVolume( 1024 );
			break;
		case 2:
			Lvl.Init( ViperSpriteTexPal, Stage03BG, 1 );
			Lvl.LoadEnemySequence( simap_level_03_map );
			Sound::Instance()->PlayBGM( E_BGM_LEVEL03, MM_PLAY_LOOP );
			Sound::Instance()->SetMasterVolume( 1024 );
			break;
		case 3:
			Lvl.Init( ViperSpriteTexPal, Stage04BG, 3 );
			Lvl.LoadEnemySequence( simap_level_04_map );
			Sound::Instance()->PlayBGM( E_BGM_LEVEL04, MM_PLAY_LOOP );
			Sound::Instance()->SetMasterVolume( 1024 );
			break;
		case 4:
			Lvl.LoadEnemySequence( simap_level_05_map );
			Sound::Instance()->PlayBGM( E_BGM_LEVEL05, MM_PLAY_LOOP );
			Sound::Instance()->SetMasterVolume( 1024 );
			break;
		default:
			Lvl.LoadEnemySequence( simap_level_01_map );
			Sound::Instance()->PlayBGM( E_BGM_LEVEL01, MM_PLAY_LOOP );
			Sound::Instance()->SetMasterVolume( 1024 );
			
	}
	
	
	Frame = 0;
	BossTimer = 0;
	UTIL::ResetRND();
	while( !StageFinished )
	{
	
		Frame++;
	
		
		if( Frame == 128 )
		{
			UTIL::RND_RANGE(32,192-32);  // For RND fix
			if( (GameMode == 0) && ((LoopNumber * 5 + Stage) > 0) ) ShipUps.Spawn( 300, 96 );  // don't spawn when boss rush
		}
		
		if( ( Frame < STOP_SPAWING ) )
		{
			GameEnemySequence( Frame, *Players[CurrentShip], Turrets, Lvl );			
		}
		else if( Frame == STOP_SPAWING )
		{
			if( !Players[CurrentShip]->IsAutoBomb() )
				BomberUps.Spawn( 300, UTIL::RND_RANGE(32,192-32) );  // don't spawn when boss rush
				
			if( IsDemo )
				BomberUps.Spawn( 300, UTIL::RND_RANGE(32,192-32) );  // spawn for demo
				
		}
		else		// Boss
		{
			if( Players[CurrentShip]->GetLevel() < 4 && 
			    ((Frame & 1023) == 0) && 
				Bosses[CurrentBoss]->IsAlive() )
				BonusCarriers.Spawn( 300, UTIL::RND_RANGE(10, 180) );
			
			if( (LoopNumber > 0) && Bosses[CurrentBoss]->IsAlive() )  // Spawn random popcorns when in loop 2
			{
				SpawnRandomEverything( Frame, Turrets );
			}
		}
		
		// Boss
		if( Frame == START_BOSS )
		{
			SoundInterp = 1 << 12;
			Sound::Instance()->SetMasterVolume( 1024 );
			Sound::Instance()->PlayBGM( E_BGM_BOSS, MM_PLAY_LOOP );
			Bosses[CurrentBoss]->Spawn( Turrets );
		}
		
		
		
		scanKeys();
		int KeyH = keysHeld();
		int KeyD = keysDown();
	
		//if( (KeyD & KEY_L) )
		//{
			//DrawDebug = !DrawDebug;
			//DrawAABB = !DrawAABB;
			//Bosses[CurrentBoss]->ForceDestruction();
			//Frame = BOSS_DEATH_FRAME;
		//}
		
		
		if( !GameOver )
		{
			if( !IsDemo )
			{
				if(KeyD & KEY_START)
					IsPaused = true;
			}
			else
			{
				if(KeyD & KEY_START)     // return to title when in demo mode
				{
					GameActive = false;
					StageFinished = true;
					HasContinued = true;
					ForceExited = true;
				}
			}
		}
		else
		{
			// Don't let player continue in replay mode
			if( IsReplay ) 
			{
				if( CanContinueCounter > 0 )
					CanContinueCounter = 0;
				
				CanContinueCounter--;
				if( CanContinueCounter < -(60 * 6) )   // force end of stage after 6 secs
				{
					GameActive = false;
					StageFinished = true;
				}
				
			}
				
			if( CanContinueCounter > 0 )
			{
				
				CanContinueCounter--;
				if (KeyD & KEY_A)				// Arcade style fast countdown
				{
					CanContinueCounter -= 60;
					if( CanPressStartCounter > 0 )
					{
						CanPressStartCounter-= 60;
					}
				}
				
				if( CanPressStartCounter > 0 )
				{
					CanPressStartCounter--;
				}
				else
				{
					if (KeyD & KEY_START)
					{
						if( FirstScore == 0 )
							FirstScore = Players[CurrentShip]->GetScore();
						Players[CurrentShip]->NewGame();
						GameOver = false;
						CanPressStartCounter = 60 * 3;
						CanContinueCounter = 60 * 13;
						HasContinued = true;
						NumContinues++;
					}
					
				}
			}
			else	//Return to title
			{
				Sound::Instance()->FadeBGM( SoundInterp, -16 );
				if (KeyD & KEY_START)
				{
					if( !IsReplay )
					{
						Button::Replay.Mode = GameMode;
						Button::SaveReplay(3);     // Save replay if gameovered and not in replay mode
					}
					
					if( FirstScore == 0 )
							FirstScore = Players[CurrentShip]->GetScore();
					GameActive = false;
					StageFinished = true;
					HasContinued = true;
				}
			}
		}
			
		// If lives are spent then game over
		if( Players[CurrentShip]->GetLives() == -1 )
		{
			GameOver = true;
			//Button::CanReplay = false;   //  can't replay if died in the middle
		}
		
		if( (Frame > FADE_BGM) && (Frame < START_BOSS) )
		{
			Sound::Instance()->FadeBGM( SoundInterp, -16 );
		}
		
		if( (!Bosses[CurrentBoss]->IsAlive()) && (Frame > START_BOSS) )
		{
			Bosses[CurrentBoss]->DecDestroyedCounter();
			//Bullets.KillEntities();
			Sound::Instance()->FadeBGM( SoundInterp,-16 );
		}
		
		// Bugz next level wont load when you die at boss explosion
		// and you continue after a while
		// otherwise it works
		if( (Bosses[CurrentBoss]->GetDestroyedCounter() < 0) )
		{
			if( (!Players[CurrentShip]->IsLocked()) )   // only end the stage when player is active
			{
				Sound::Instance()->PlaySFX( E_SFX_VIPER_ENTER );
				Players[CurrentShip]->SetMode( PlayerShip::E_EXIT );
				Players[CurrentShip]->SetState( PlayerShip::E_LOCKED );
			}
		}
	
		if( PlayerExited )
		{
			FadeOut = true;
			if(!HasContinued)
				Button::CanReplay = true;
		}
		
		if( ( FadeInterp >= (1 << 12) ) && (Bosses[CurrentBoss]->GetDestroyedCounter() <= -60 * 7 ) )
		{
			StageFinished = true;
			Bosses[CurrentBoss]->SetDestroyedCounter( 1 << 12 );
		}
		
		GlobalManager::Instance()->ResetAll();
		
		
		UTIL::BitClearAll( Button::Buttons );
		
		GameUpdateAll( Frame,
					   KeyH,
					   KeyD,
					   Stage,
					   PlayerExited,
					   Lvl,
					   *Players[CurrentShip],
					   Bombs,
					   Bullets,
					   Turrets,
					   PowerUps,
					   Enemies,
					   *Bosses[CurrentBoss] );
		
		
		// Record keypad state
		if( !IsReplay )
		{
			Button::KeyPressed[LoopNumber * 5 + Stage][Frame] = Button::Buttons;
		}
		
								
		GameDrawAll( Frame,
					 Stage,
					 FadeOut,
					 FadeInterp,
					 BGInterp,
					 FontInterp,
					 Lvl,
					 *Players[CurrentShip],
				     Bombs,
				     Bullets,
				     Enemies,
					 *Bosses[CurrentBoss],
					 DrawAABB,
					 DrawDebug,
					 IsPaused,
					 false,
					 GameOver,
					 CanPressStartCounter,
					 CanContinueCounter,
					 PrintGameOver,
					 YorNactive );
   		
		
		
		if( IsPaused )
		{
			int Result = GamePause( Frame,
								    Stage,
								    FadeOut,
								    FadeInterp,
								    BGInterp,
								    FontInterp,
								    Lvl,
								    *Players[CurrentShip],
								    Bullets,
								    Bombs,
								    Enemies,
								    *Bosses[CurrentBoss] );
			IsPaused = false;
			FadeOut = false;
			FadeInterp = 0;
			
			if( Result == 1)
			{
				ForceExited = true;
				if( !IsReplay )
				{
					Button::CanReplay = false;     // Cannot replay if forced exit from pause
				}
				else
				{
					Button::CanReplay = true;
				}
				GameActive = false;
				StageFinished = true;
			}
					 
		}
		
		// Draw debug
		//consoleClear();
		//iprintf("\x1b[1;1HEnemyColumn = %i         ", Lvl.GetEnemyColumn() );
					
//		if( DrawDebug )
//		{	
//			iprintf("\x1b[1;1HFrames = %i         ", Frame);
//			iprintf("\x1b[2;1HMemUsed = %i         ", Memory.GetMemUsed()/1024);
//			iprintf("\x1b[3;1HMemFree = %i         ", Memory.GetMemFree()/1024);
//				
//			iprintf("\x1b[5;1HMaxBullets = %i         ", Bullets.NumEntities());
//			iprintf("\x1b[6;1HNumTurrets = %i         ", Turrets.NumEntities());
//			iprintf("\x1b[7;1HBossDestroyedCounter = %i         ", Bosses[CurrentBoss]->GetDestroyedCounter());
//			iprintf("\x1b[8;1HStage = %i         ", Stage);
//			iprintf("\x1b[9;1HFadeInterp = %i         ", FadeInterp);
//			
//			iprintf("\x1b[11;1HScore = %i         ", Players[CurrentShip]->GetScore());
//			iprintf("\x1b[12;1HLives  = %i         ", Players[CurrentShip]->GetLives() );
//			iprintf("\x1b[13;1HLevel  = %i         ", Players[CurrentShip]->GetLevel() );
//			iprintf("\x1b[14;1HActive  = %i         ", Players[CurrentShip]->IsActive() );
//			iprintf("\x1b[15;1HNumTargets  = %i         ", Targets::Instance()->GetNumTargets() );
//			iprintf("\x1b[16;1HBombs  = %i         ", Players[CurrentShip]->GetBombs() );
//			iprintf("\x1b[17;1HBossHP = %i         ", Bosses[CurrentBoss]->GetHp());
//			
//			iprintf("\x1b[19;1HGameActive = %i         ", GameActive);
//			iprintf("\x1b[20;1HGameOver = %i         ", GameOver);
//			iprintf("\x1b[21;1HIsReplay = %i         ", IsReplay);
//		}
		
		
	} // loop ( StageFinished )
	
	if( Players[CurrentShip]->GetLives() > -1 )
	{
		if( !IsDemo  )
		{
			if( !IsReplay )
			{
				if( !CheatOn )
				{
					if( !ForceExited )
					{
						Intermission( Stage, *Players[CurrentShip] );
					}
				}
			}
			else
			{
				NoIntermission( *Players[CurrentShip] );
			}
		}
		else
		{
			NoIntermission( *Players[CurrentShip] );
		}
	}
	
	Bosses[CurrentBoss]->SetDestroyedCounter( 1 << 12 );
	Bosses[CurrentBoss]->Kill();
	Stage++;
	if( Stage > 4 )
	{
		if( (CanContinueCounter > 0) && (!ForceExited) )   // >0 boss killed, <=0 Gameovered
		{
			//Button::SaveReplay();
			LoopNumber++;
			lcdSwap();
			Ending(LoopNumber);
			lcdSwap();
			if( LoopNumber > 1 ) GameActive = false;   //GameOver if more than 2 loops
		}
		Stage = 0;	
		// if( IsReplay ) GameActive = false;     // limit replay to 1 loop
 	}
	
	CurrentBoss++;
	if( CurrentBoss > 4 ) CurrentBoss = 0;
	//GameActive = false;   // Temp only a single stage so far
	
	if( !GameActive )
	{
		// Set PlayerStat if not replay mode and score is higher than lowest
		if( !IsReplay && !ForceExited)
		{
			if( FirstScore == 0 )    // For when you can 1 cc the whole loop
				FirstScore = Players[CurrentShip]->GetScore();
			
			Button::Replay.MaxScore = FirstScore;    // save  maxscore
			
			// Save the highest score for this multi continue game
			if( PlayerStat.Score < FirstScore ) 
			{
				PlayerStat.Score = FirstScore;
				FirstScore = 0;
			}
	
			for( int i = 0; i < 10; i++ )
			{
				if( PlayerStat.Score > Scores[i].Score )
				{
					PlayerStat.ShipID = Players[CurrentShip]->GetID();
					TextEntry( 16 * 3, 90, 16, "-", 10, PlayerStat.Name, SiFontGL16x16, SiFontGL );
					PrintHighScore(PlayerStat, GameMode);
					SaveHiScore();
					break;
				}
			}
		}
	}

} // loop ( GameActive )
} // loop ( while(1) )
	
}


