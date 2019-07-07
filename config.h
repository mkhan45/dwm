/* See LICENSE file for copyright and license details. */ #include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 3;
static const unsigned int vertpadbar= 8;
static const unsigned int horizpadbar = 0;
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVu Sans Mono:size=14" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_white[]       = "#f8f8f8";
static const char col_bright[]      = "#ffffff";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#c9b9b9";
static const char col_gray4[]       = "#eeeeee";
static const char col_black[]       = "#090916";
static const char col_cyan[]        = "#005577";
static const char col_greyblue[]    = "#23232f";
static const char col_lightgrey[]   = "#71718f";
static const char col_red[]         = "#ce3521";
static const unsigned int baralpha  = 0xd7;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_greyblue, col_lightgrey },
	[SchemeSel]  = { col_bright, col_black,  col_red  },
};
static const int alphas[][3]        = {
        /*                fg        bg         border   */
        [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
        [SchemeSel] =  { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	/* { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 }, */
};

#include "fibonacci.c"

/* layout(s) */
static const float mfact     = 0.57; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
        { "C"  ,      centeredmaster },
        { "@"  ,      spiral },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/dash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "drun" };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "firefox", NULL };

static const char *volumedowncmd[]  = { "/home/fish/dwm/scripts/volumedown.dash", NULL };
static const char *volumeupcmd[]  = { "/home/fish/dwm/scripts/volumeup.dash", NULL };
static const char *volumetogglecmd[]  = { "/home/fish/dwm/scripts/volumetoggle.dash", NULL };
static const char *lockcmd[]  = { "/home/fish/dwm/scripts/lock.dash", NULL };


/* volume buttons: 4294967197 */
static Key keys[] = {
	/* modifier                     key        function        argument */
        { ControlMask|ShiftMask,        12,        spawn,          {.v = volumeupcmd}},
        { ControlMask|ShiftMask,        11,        spawn,          {.v = volumedowncmd}},
        { ControlMask|ShiftMask,        10,        spawn,          {.v = volumetogglecmd}},
	{ 0,                            133,       holdbar,        { 0 } }, //super
	{ MODKEY,                       40,        spawn,          {.v = dmenucmd } }, //d
	{ MODKEY,                       36,        spawn,          {.v = termcmd } }, //enter
	{ MODKEY,                       41,        spawn,          {.v = browsercmd } }, //f
	{ MODKEY|ShiftMask,             38,        spawn,          {.v = lockcmd} }, //a
	{ MODKEY,                       56,        togglebar,      {0} },
	{ MODKEY,                       44,        focusstack,     {.i = +1 } },
	{ MODKEY,                       45,        focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             40,        incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             31,        incnmaster,     {.i = +1 } },
	{ MODKEY,                       43,        setmfact,       {.f = -0.05} },
	{ MODKEY,                       46,        setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             46,        setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             43,        setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             36, zoom,           {0} },
	{ MODKEY,                       23,    view,           {0} },
	{ MODKEY|ShiftMask,             24,      killclient,     {0} },
	{ MODKEY,                       28,      setlayout,      {.v = &layouts[0]} }, //t
	{ MODKEY,                       58,      setlayout,      {.v = &layouts[2]} }, //m
	{ MODKEY,                       54,      setlayout,      {.v = &layouts[3]} }, //c
	{ MODKEY,                       39,      setlayout,      {.v = &layouts[4]} }, //s
	{ MODKEY,                       65,  setlayout,      {0} }, //space
	{ MODKEY|ShiftMask,             65,  togglefloating, {0} }, //space
	{ MODKEY,                       27,      rotatestack,    {1} }, //r
	{ MODKEY,                       19,      comboview,           {.ui = ~0 } }, //0
	{ MODKEY|ShiftMask,             19,      combotag,            {.ui = ~0 } }, //0
	{ MODKEY,                       59,  focusmon,       {.i = -1 } }, //comma
	{ MODKEY,                       60, focusmon,       {.i = +1 } }, //period
	{ MODKEY|ShiftMask,             59,  tagmon,         {.i = -1 } }, //comma 
	{ MODKEY|ShiftMask,             60, tagmon,         {.i = +1 } }, //period
	TAGKEYS(                        10,                      0) //1
	TAGKEYS(                        11,                      1)
	TAGKEYS(                        12,                      2)
	TAGKEYS(                        13,                      3)
	TAGKEYS(                        14,                      4)
	TAGKEYS(                        15,                      5)
	TAGKEYS(                        16,                      6)
	TAGKEYS(                        17,                      7)
	TAGKEYS(                        18,                      8)
	{ MODKEY|ShiftMask,             26,      quit,           {0} }, //e
	{ MODKEY|ControlMask|ShiftMask, 24,      quit,           {1} } 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

