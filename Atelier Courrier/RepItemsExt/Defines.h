#pragma once

// Mode visualisation des résultats
enum SHOWRESULTS
{
	SHOW_EFFECTIVE	=	1,
	SHOW_NBCAS		=	SHOW_EFFECTIVE<<1,
	SHOW_PERCENT	=	SHOW_NBCAS<<1,
	SHOW_ZSCORE		=   SHOW_PERCENT<<1,
	SHOW_ALL		=	SHOW_EFFECTIVE|SHOW_NBCAS|SHOW_PERCENT,
};

// Mode RepItem (Mode standard, Mode simple)
enum SHOWMODES
{
	SHOWMODE_STANDARD = 1,
	SHOWMODE_SIMPLE   = 2,
};

// Mode avec ou sans sélection sites
enum SITEMODES
{
	SHOW_NOSITE = 1,
	SHOW_SITES	= 2,
};
