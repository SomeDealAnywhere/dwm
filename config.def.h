/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx        = 1;    /* border pixel of windows */
static unsigned int snap            = 32;   /* snap pixel */
static const int swallowfloating    = 1;    /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 9;   /* horiz inner gap between windows */
static const unsigned int gappiv    = 9;   /* vert inner gap between windows */
static const unsigned int gappoh    = 9;   /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 9;   /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;    /* 1 means no outer gap when there is only one window */
static int showbar                  = 1;    /* 0 means no bar */
static int topbar                   = 1;    /* 0 means bottom bar */
static const int user_bh            = 20;    /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static char font[]                  = "Lucida Sans:size=10:antialias=true:hinting=true";
static char dmenufont[]             = "Lucida Sans:size=10:antialias=true:hinting=true";
static const char *fonts[]          = { "Lucida Sans:size=10:antialias=true:hinting=true", "NotoColorEmoji:pixelsize=10:antialias=true:hinting=true" };
static char normbgcolor[]           = "#1e1f29";
static char normbordercolor[]       = "#1b1c25";
static char normfgcolor[]           = "#f8f8f2";
static char selbgcolor[]            = "#bd93f9";
static char selbordercolor[]        = "#bd93f9";
static char selfgcolor[]            = "#1e1f29";
static char *colors[][3] = {
	/*               	fg           bg           border   */
	[SchemeNorm]	    = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel] 	    = { selfgcolor,  selbgcolor,  selbordercolor  },
	[SchemeStatus]      = { normfgcolor, normbgcolor,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]     = { selfgcolor,  selbgcolor,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]    = { normfgcolor, normbgcolor,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]     = { normfgcolor, normbgcolor,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]    = { normfgcolor, normbgcolor,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "Lutris",  NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{  NULL,     NULL,     "Event Tester", 0,         0,          0,           1,        -1 },   /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol         arrange                      function */
	{ "[L-1]",          spiral },                   /* first entry is default */
	{ "[L-2]",          dwindle },
	{ "[L-3]",          tile },    
	{ "[L-4]",          bstack },
	{ "[L-5]",          bstackhoriz },
	{ "[L-6]",          centeredmaster },
	{ "[L-7]",          centeredfloatingmaster },
	{ "[L-8]",          grid },
	{ "[L-9]",          horizgrid },
	{ "[F-L]",          NULL },                      /* no layout function means floating behavior */
	{ NULL,             NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

 /* Xresources preferences to load at startup */
 
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",       	FLOAT,   &mfact },
};

#include "movestack.c"
static const Key keys[] = {
	/* Modifier             Chain  Key        Function        Argument        Comment              */
	{ MODKEY,               -1,    XK_j,      focusstack,     {.i = +1 } },   
	{ MODKEY,               -1,    XK_k,      focusstack,     {.i = -1 } },   
	{ MODKEY|ShiftMask,     -1,    XK_j,      movestack,      {.i = +1 } },   
	{ MODKEY|ShiftMask,     -1,    XK_k,      movestack,      {.i = -1 } },   
	{ MODKEY,               -1,    XK_comma,  focusmon,       {.i = -1 } },   
	{ MODKEY,               -1,    XK_period, focusmon,       {.i = +1 } },   
	{ MODKEY|ShiftMask,     -1,    XK_comma,  tagmon,         {.i = -1 } },   
	{ MODKEY|ShiftMask,     -1,    XK_period, tagmon,         {.i = +1 } },   
	{ MODKEY,               -1,    XK_f,      togglefullscr,  {0} },          
	{ MODKEY,               -1,    XK_q,      killclient,     {0} },          
	{ MODKEY|ControlMask,   -1,    XK_r,      quit,           {1} },           
	{ MODKEY|ShiftMask,     -1,    XK_q,      quit,           {0} },          
	{ MODKEY|ShiftMask,     -1,    XK_h,      setmfact,       {.f = -0.05} },  
	{ MODKEY|ShiftMask,     -1,    XK_l,      setmfact,       {.f = +0.05} }, 
	{ MODKEY,               -1,    XK_Return, zoom,           {0} },          

        /* Tags (Workspaces) */
	TAGKEYS(                -1,    XK_1,                      0)
	TAGKEYS(                -1,    XK_2,                      1)
	TAGKEYS(                -1,    XK_3,                      2)
	TAGKEYS(                -1,    XK_4,                      3)
	TAGKEYS(                -1,    XK_5,                      4)
	TAGKEYS(                -1,    XK_6,                      5)
	TAGKEYS(                -1,    XK_7,                      6)
	TAGKEYS(                -1,    XK_8,                      7)
	TAGKEYS(                -1,    XK_9,                      8)

        /* Layouts */
	{ MODKEY,               XK_l,  XK_1,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,               XK_l,  XK_2,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,               XK_l,  XK_3,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,               XK_l,  XK_4,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,               XK_l,  XK_5,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,               XK_l,  XK_6,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,               XK_l,  XK_7,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,               XK_l,  XK_8,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,               XK_l,  XK_9,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY,               XK_l,  XK_f,      togglefloating, {0} },
        
        /* Run */
        { MODKEY,               -1,    XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,               XK_r,  XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY,               XK_r,  XK_b,      spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY,               XK_r,  XK_f,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "ranger", NULL } } },
	{ MODKEY,               XK_r,  XK_e,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "vim", NULL } } },
	{ MODKEY,               XK_r,  XK_s,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },

        /* Disabled default bindings */
/*	{ MODKEY,               -1,    XK_b,      togglebar,      {0} },       
	{ MODKEY,               -1,    XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,               -1,    XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,               -1,    XK_Tab,    view,           {0} },          
	{ MODKEY,               -1,    XK_0,      view,           {.ui = ~0 } },  
	{ MODKEY|ShiftMask,     -1,    XK_0,      tag,            {.ui = ~0 } },    */

        /* Vanity gaps bindings */
/*	{ MODKEY,               XK_g,  XK_g,      togglegaps,     {0} },
	{ MODKEY,               XK_g,  XK_r,      defaultgaps,    {0} },
	{ MODKEY,               XK_g,  XK_equal,  incrgaps,       {.i = +1 } },
	{ MODKEY,               XK_g,  XK_minus,  incrgaps,       {.i = -1 } },
	{ MODKEY|ShiftMask,     XK_g,  XK_equal,  incrigaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_g,  XK_minus,  incrigaps,      {.i = -1 } },
	{ MODKEY|ControlMask,   XK_g,  XK_equal,  incrogaps,      {.i = +1 } },
	{ MODKEY|ControlMask,   XK_g,  XK_minus,  incrogaps,      {.i = -1 } },
	{ MODKEY,               XK_g,  XK_l,      incrihgaps,     {.i = +1 } },
	{ MODKEY,               XK_g,  XK_h,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,     XK_g,  XK_l,      incrohgaps,     {.i = +1 } }, 
	{ MODKEY|ShiftMask,     XK_g,  XK_h,      incrohgaps,     {.i = -1 } }, 
	{ MODKEY,               XK_g,  XK_k,      incrivgaps,     {.i = +1 } }, 
	{ MODKEY,               XK_g,  XK_j,      incrivgaps,     {.i = -1 } }, 
	{ MODKEY|ShiftMask,     XK_g,  XK_k,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_g,  XK_j,      incrovgaps,     {.i = -1 } },    */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
