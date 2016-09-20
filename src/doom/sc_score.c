#include "sc_score.h"
#include "p_mobj.h"
#include "m_misc.h"
#include "i_system.h"
#include <stdlib.h>
#include <assert.h>

#define SC_RECORD_FILENAME "arcade_records.txt"

#define SSCANF_FORMAT_STRING_LEN_(S) "%" #S "s"
#define SSCANF_FORMAT_STRING_LEN(S) SSCANF_FORMAT_STRING_LEN_(S)

struct
{
	boolean is_nightmare;
	int maxkills, maxitems, maxsecrets;
	unsigned score;
} static s_state;

const unsigned * const sc_current_score = &s_state.score;
sc_record_t sc_records[SC_NUM_RECORDS];

static sc_record_t sc_default_records[SC_NUM_RECORDS] = {
	{ "JAZZY", 9, 0 },
	{ "FUZZY", 8, 0 },
	{ "MUZZY", 7, 0 },
	{ "WHIZZ", 6, 0 },
	{ "FEZZY", 5, 0 },
	{ "FIZZY", 4, 0 },
	{ "ABUZZ", 3, 0 },
	{ "ZUZIM", 2, 0 },
	{ "SCUZZ", 1, 0 },
	{ "DIZZY", 0, 0 },
};

void SC_Init()
{
	memset( &s_state, 0, sizeof( s_state ) );
	memset( &sc_records[0], 0, sizeof( sc_records ) );

	FILE * f = fopen( SC_RECORD_FILENAME, "rb" );
	if( !f )
	{
		memcpy( sc_records, sc_default_records, sizeof( sc_records ) );
		return;
	}

	char line[128];

	// check file version 
	// TODO migration
	fgets( line, sizeof( line ), f );
	int fileversion = atoi( line );
	if( fileversion != SC_RECORD_VERSION )
	{
		fclose( f );
		I_Error( SC_RECORD_FILENAME " is version %i, expected version %i\n", 
			fileversion, SC_RECORD_VERSION );
	}

	for( int i = 0; i < SC_NUM_RECORDS; ++i )
	{
		sc_record_t * r = &sc_records[i];
		fgets( line, sizeof( line ), f );
		if( sscanf( line, SSCANF_FORMAT_STRING_LEN( SC_MAX_NAME_LEN ) " %u %u", 
			r->name, &r->score, &r->duration_sec ) != 3 )
		{
			fclose( f );
			I_Error( SC_RECORD_FILENAME " line %i parse error\n", i );
		}
		r->name[SC_MAX_NAME_LEN] = 0;
	}

	fclose( f );
}

const sc_record_t * SC_GetHighScore()
{
	// TODO OPT
	static sc_record_t result;
	sc_record_t * high = &sc_records[0];
	if ( s_state.score > high->score )
	{
		result.score = s_state.score;
		result.duration_sec = 0; // TODO TIME
		result.name[0] = 'Y';
		result.name[1] = 'O';
		result.name[2] = 'U';
		result.name[3] = 0;
	}
	else
	{
		result = *high;
	}
	return &result;
}

void SC_BeginNewRecord( boolean is_nightmare )
{
	memset( &s_state, 0, sizeof( s_state ) );
	s_state.is_nightmare = is_nightmare;
	s_state.score = 0;
}

int SC_FinalizeRecord( char * player_name )
{
	// find the rank
	int rank = 0;
	for( ; rank < SC_NUM_RECORDS; ++rank )
	{
		if( s_state.score > sc_records[rank].score )
		{
			break;
		}
	}

	if( rank == SC_NUM_RECORDS )
	{
		// did not place
		return -1;
	}

	// insert a record
	memmove( &sc_records[rank + 1], &sc_records[rank], 
		sizeof( sc_record_t ) * ( SC_NUM_RECORDS - rank - 1 ) );
	sc_records[rank].score = s_state.score;
	sc_records[rank].duration_sec = 0; // TODO
	M_StringCopy( sc_records[rank].name, player_name, SC_MAX_NAME_LEN );

	// save
	FILE * f = fopen( SC_RECORD_FILENAME, "wb" );
	if( !f )
	{
		I_Error( SC_RECORD_FILENAME " failed to open for write" );
	}
	fprintf( f, "%i\n", SC_RECORD_VERSION );
	for( int i = 0; i < SC_NUM_RECORDS; ++i )
	{
		sc_record_t * r = &sc_records[i];
		fprintf( f, "%s %u %u\n", r->name, r->score, r->duration_sec );
	}
	fclose( f );

	return rank;
}

void SC_OnNextMap( int maxkills, int maxitems, int maxsecrets )
{
	s_state.maxkills = maxkills;
	s_state.maxitems = maxitems;
	s_state.maxsecrets = maxsecrets;
}

void SC_OnGetAmmo( ammotype_t ammo, int amount )
{
	//s_state.score += amount;
}

void SC_OnGetArmor( int amount )
{
	//s_state.score += amount;
}

void SC_OnGetBackpack()
{
	//s_state.score += 500;
}

void SC_OnGetHealth( int amount )
{
	//s_state.score += amount;
}

void SC_OnGetKey( int type )
{
	//s_state.score += 1000;
}

void SC_OnGetPowerup( int type )
{
	//s_state.score += 1000;
}

void SC_OnGetWeapon( weapontype_t weapon, boolean was_dropped )
{
	//s_state.score += 1000;
}

void SC_OnMappedWall( boolean is_boundary, boolean is_secret )
{
	//s_state.score += is_boundary + (is_secret * 100);
}

void SC_OnMobjDamaged( mobj_t * target, mobj_t * inflictor, mobj_t * source, int damage, fixed_t thrust )
{
	//s_state.score += damage / 10;
}

static int SC_PointsForKill( mobj_t * killed )
{
	switch( killed->type )
	{
	case MT_POSSESSED: return 100;
	case MT_SHOTGUY: return 250;
	case MT_TROOP: return 400; // imp
	case MT_SERGEANT: return 650; // pinky
	case MT_HEAD: return 850; // cacodemon
	case MT_SKULL: return 225; // lost soul
	case MT_BRUISER: return 1000; // baron
	case MT_SPIDER: return 6500; // mastermind
	case MT_CYBORG: return 7500; // cyberdemon
	
	case MT_CHAINGUY:
	case MT_KNIGHT:
	case MT_BABY:
	case MT_PAIN:
	case MT_UNDEAD:
	case MT_FATSO:
	case MT_VILE:
		assert( false );
		return 0;

	default:
		return 0;
	}
}

void SC_OnMobjKilled( mobj_t * target, mobj_t * inflictor, mobj_t * source )
{
	int points = SC_PointsForKill(target);
	if ( !points )
	{
		return;
	}
	if ( inflictor->type == MT_BARREL ) 
	{
		points *= 2;
	}
	s_state.score += points;
}

void SC_OnTouchSecretSector()
{
	//s_state.score += 1000;
}

