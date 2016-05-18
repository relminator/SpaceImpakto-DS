/******************************************************************************
*******************************************************************************

	Space Impakto DS
	Relminator
	http://rel.betterwebber.com


	VicViper class


*******************************************************************************
******************************************************************************/

#include "PlayerShip.h"

#ifndef VIC_H
#define VIC_H


#define MAX_VIPER_OPTIONS 2
#define MAX_VIPER_LINKS 32

class VicViper : public PlayerShip
{
public:
	
	struct Option
	{
		s32 x;
		s32 y;
		int Frame;
	};
	virtual void Init( int Palette, glImage* const Sprites );
	virtual bool Update( int KeyH, int KeyD, Bomb &Bomber  );
	virtual bool Replay( int Frame, Bomb &Bomber, int Stage );
	virtual void Draw() const;
	virtual void DrawShots() const;
	virtual void DrawAABB( int color ) const;
	
private:
	
	void SpawnNormalShot();
	void SpawnMissile();
	void Animate();
	void UpdateOptions();
	
	int NumOptions;
	Option Options[MAX_VIPER_OPTIONS];
	Vector2df32 Links[MAX_VIPER_LINKS];
};






#endif // VIC_H
