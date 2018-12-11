/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 8;        /* gap pixel between windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "misc ohsnap.icons:size=11", "xos4 terminus:size=9:lang=ru" };
static const char dmenufont[]       = "misc ohsnap.icons:size=11";
static const char col_bg[]          = "#303030";
static const char col_dgray[]       = "#a9a9a9";
static const char col_gray[]        = "#d3d3d3";
static const char col_red[]         = "#ff0000";
static const char col_green[]       = "#4cbb17";
static const char col_blue[]        = "#1e90ff";
static const char col_magenta[]     = "#d0417e";
static const char col_cyan[]        = "#87cefa";
static const char col_yellow[]      = "#fbec5d";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]   = { col_dgray,  col_bg,  col_bg    },
	[SchemeSel]    = { col_gray,   col_bg,  col_dgray },
	[SchemeWarn]   = { col_yellow, col_bg,  col_bg    },
	[SchemeUrgent] = { col_red,    col_bg,  col_bg    },
};

/* tagging */
static const char *tags[] = { "main", "web", "dev", "steam", "temp" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "Chromium",     NULL,       NULL,       1 << 1,       False,       -1 },
	{ "Firefox",      NULL,       NULL,       1 << 4,       False,       -1 },
	{ "Gimp",         NULL,       NULL,       0,            True,        -1 },
	{ "Steam",        NULL,       NULL,       1 << 3,       True,        -1 },
};

/* layout(s) */
static const float mfact     = 0.70; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[t]",      tile },    /* first entry is default */
	{ "[f]",      NULL },    /* no layout function means floating behavior */
	{ "[m]",      monocle },
	{ "[c]",      col },
	{ "[g]",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_dgray, "-sb", col_bg, "-sf", col_green, NULL };
static const char *termcmd[]       = { "st", "-g", "140x35+550+350", NULL };
static const char *stcmd[]         = { "st", "-g", "125x30+550+300", "-f", "Ubuntu Mono:size=11", NULL };
static const char *wwwcmd[]        = { "chromium", NULL };
static const char *mutecmd[]       = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *volupcmd[]      = { "amixer", "-q", "set", "Master", "5%+", NULL };
static const char *voldncmd[]      = { "amixer", "-q", "set", "Master", "5%-", NULL };
static const char *mpctogglecmd[]  = { "mpc", "-q", "toggle", NULL };
static const char *mpcstopcmd[]    = { "mpc", "-q", "stop", NULL };
static const char *mpcprevcmd[]    = { "mpc", "-q", "prev", NULL };
static const char *mpcnextcmd[]    = { "mpc", "-q", "next", NULL };
static const char *mpcvolupcmd[]   = { "mpc", "-q", "volume", "+5", NULL };
static const char *mpcvoldowncmd[] = { "mpc", "-q", "volume", "-5", NULL };
static const char *mpcvoloncmd[]   = { "mpc", "-q", "volume", "100", NULL };
static const char *mpcvoloffcmd[]  = { "mpc", "-q", "volume", "0", NULL };
static const char *rotateicmd[]    = { "xrandr", "--output", "DVI-I-1", "--rotate", "inverted", NULL };
static const char *rotatencmd[]    = { "xrandr", "--output", "DVI-I-1", "--rotate", "normal", NULL };
static const char *rotatelcmd[]    = { "xrandr", "--output", "DVI-I-1", "--rotate", "left", NULL };
static const char *rotatercmd[]    = { "xrandr", "--output", "DVI-I-1", "--rotate", "right", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_x,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_grave,  spawn,          {.v = stcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_j,      setcfact,       {.f = +0.020} },
	{ MODKEY|ShiftMask,             XK_k,      setcfact,       {.f = -0.020} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_p,      quit,           {0} },
	/* custom */
	{ False,                        XF86XK_HomePage,           spawn,          {.v = wwwcmd } },
	{ False,                        XF86XK_AudioPlay,          spawn,          {.v = mpctogglecmd } },
	{ False,                        XF86XK_AudioStop,          spawn,          {.v = mpcstopcmd } },
	{ False,                        XF86XK_AudioPrev,          spawn,          {.v = mpcprevcmd } },
	{ False,                        XF86XK_AudioNext,          spawn,          {.v = mpcnextcmd } },
	{ MODKEY,                       XK_KP_Add,                 spawn,          {.v = mpcvolupcmd } },
	{ MODKEY,                       XK_KP_Subtract,            spawn,          {.v = mpcvoldowncmd } },
	{ MODKEY,                       XK_KP_Multiply,            spawn,          {.v = mpcvoloncmd } },
	{ MODKEY,                       XK_KP_Divide,              spawn,          {.v = mpcvoloffcmd } },
	{ False,                        XF86XK_AudioMute,          spawn,          {.v = mutecmd } },
	{ False,                        XF86XK_AudioRaiseVolume,   spawn,          {.v = volupcmd } },
	{ False,                        XF86XK_AudioLowerVolume,   spawn,          {.v = voldncmd } },
	{ MODKEY,                       XK_Up,                     spawn,          {.v = rotateicmd } },
	{ MODKEY,                       XK_Down,                   spawn,          {.v = rotatencmd } },
	{ MODKEY,                       XK_Left,                   spawn,          {.v = rotatelcmd } },
	{ MODKEY,                       XK_Right,                  spawn,          {.v = rotatercmd } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

