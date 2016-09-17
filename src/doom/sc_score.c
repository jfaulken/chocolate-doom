#include "sc_score.h"
#include "p_mobj.h"
#include <stdint.h>

void SC_BeginNewRecord( boolean is_nightmare ) {}
void SC_FinalizeRecord( char * player_name ) {}
void SC_OnBeginMap( int maxkills, int maxitems, int maxsecrets ) {}
void SC_OnEndMap() {}
void SC_OnGetAmmo( ammotype_t ammo, int amount, int new_value ) {}
void SC_OnGetArmor( int amount, int new_value ) {} 
void SC_OnGetBackpack() {}
void SC_OnGetHealth( int amount, int new_value ) {}
void SC_OnGetKey( int type ) {} // it_redcard etc
void SC_OnGetPowerup( int type ) {} // pw_invisibility etc
void SC_OnGetWeapon( weapontype_t weapon, boolean was_dropped ) {}
void SC_OnMappedWall( boolean is_boundary, boolean is_secret ) {}
void SC_OnMobjDamaged( mobj_t * target, mobj_t * inflictor, mobj_t * source, int damage ) {}
void SC_OnMobjKilled( mobj_t * source, mobj_t * target ) {}
void SC_OnTouchSecretSector() {}

