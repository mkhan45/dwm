
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 5;
static const unsigned int vertpadbar = 10;
static const unsigned int horizpadbar = 1;
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVu Sans Mono Nerd Font:size=16.5" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_white[]       = "#f8f8f8";
static const char col_bright[]      = "#ffffff";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#cab8b5";
static const char col_gray4[]       = "#eeeeee";
static const char col_black[]       = "#070710";
static const char col_cyan[]        = "#005577";
static const char col_greyblue[]    = "#23232f";
static const char col_lightgrey[]   = "#71718f";
static const char col_red[]         = "#ce3521";
static const char col_bar[]         = "#252528";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_bar, col_lightgrey },
	[SchemeSel]  = { col_bright, col_black,  col_red  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,         0,         -1 },
	{ "st",       NULL,       NULL,       0,            0,           1,         0,         -1 },
	{ "steam",    NULL, "Steam Keyboard", 0,            1,           1,         1,         -1 },
	/* { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 }, */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */


static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
static const char *sccmd[] = { "sc", "0" };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "/home/fish/dwm/scripts/browser_launch.dash", "false" };
static const char *browsercmdwindow[]  = { "/home/fish/dwm/scripts/browser_launch.dash", "true" };
static const char *actionscmd[]  = { "/home/fish/dwm/scripts/actions.dash", NULL };
static const char *lockcmd[]  = { "/home/fish/dwm/scripts/lock.dash", NULL };
static const char *homesearchcmd[]  = { "/home/fish/dwm/scripts/dirsearch.sh", NULL };

static const char *volumedowncmd[]  = { "/home/fish/dwm/scripts/volumedown.dash", NULL};
static const char *volumeupcmd[]  = { "/home/fish/dwm/scripts/volumeup.dash", NULL};
static const char *volumetogglecmd[]  = { "amixer", "-q", "set", "Master", "toggle"};

static Key keys[] = {
	/* modifier                     key        function        argument */
        {0,                             0x1008ff13,spawn,          {.v = volumeupcmd}},
        {0,                             0x1008ff11,spawn,          {.v = volumedowncmd}},
        {0,                             0x1008ff12,spawn,          {.v = volumetogglecmd}},
        // volume scroller is going bad so just use skip/prev buttons
        {0,                             XF86XK_AudioNext,  spawn,          {.v = volumeupcmd}},
        {0,                             XF86XK_AudioPrev,  spawn,          {.v = volumedowncmd}},
        {0,                             0x1008ff12,spawn,          {.v = volumetogglecmd}},
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = sccmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = homesearchcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = browsercmdwindow } },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_a,      spawn,          {.v = actionscmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	/* { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } }, */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_grave,      comboview,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_grave,      combotag,            {.ui = ~0 } },
	{ MODKEY,                       XK_p,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_o, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_p,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_o, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_0,    view,           {.ui = ~0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} } 
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

