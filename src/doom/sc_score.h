#ifndef _SC_SCORE_
#define _SC_SCORE_

#include "doomdef.h"
#include "m_fixed.h"
typedef struct mobj_s mobj_t;

#define SC_MAX_NAME_LEN 10
#define SC_RECORD_VERSION 1

typedef struct
{
	char name[SC_MAX_NAME_LEN + 1];
	unsigned score;
	unsigned duration_sec;
} sc_record_t;

#define SC_NUM_RECORDS 10
extern sc_record_t sc_records[SC_NUM_RECORDS];
extern const unsigned * const sc_current_score;

void SC_Init();
const sc_record_t * SC_GetHighScore();
void SC_BeginNewRecord( boolean is_nightmare );
int SC_FinalizeRecord( char * player_name ); // return placement on leaderboard or -1
void SC_OnNextMap( int maxkills, int maxitems, int maxsecrets );
void SC_OnGetAmmo( ammotype_t ammo, int amount );
void SC_OnGetArmor( int amount );
void SC_OnGetBackpack();
void SC_OnGetHealth( int amount );
void SC_OnGetKey( int type ); // it_redcard etc
void SC_OnGetPowerup( int type ); // pw_invisibility etc
void SC_OnGetWeapon( weapontype_t weapon, boolean was_dropped );
void SC_OnMappedWall( boolean is_boundary, boolean is_secret );
void SC_OnMobjDamaged( mobj_t * target, mobj_t * inflictor, mobj_t * source, int damage, fixed_t thrust );
void SC_OnMobjKilled( mobj_t * target, mobj_t * inflictor, mobj_t * source );
void SC_OnTouchSecretSector();

#endif