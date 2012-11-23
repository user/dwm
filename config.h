#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "-*-ohsnap-medium-*-*-*-14-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#222222";
static const char normbgcolor[]     = "#3a3a3a";
static const char normfgcolor[]     = "#dcdcdc";
static const char selbordercolor[]  = "#aaaaaa";
static const char selbgcolor[]      = "#6d6d6d";
static const char selfgcolor[]      = "#bcbcbb";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "main", "web", "dev", "media", "temp" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "Chromium",     NULL,       NULL,       1 << 1,       False,       -1 },
	{ "Pidgin",       NULL,       NULL,       1 << 3,       True,        -1 },
	{ "Sonata",       NULL,       NULL,       1 << 3,       True,        -1 },
	{ "Transmission", NULL,       NULL,       1 << 3,       True,        -1 },
	{ "Firefox",      NULL,       NULL,       1 << 4,       False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[t]",      tile },    /* first entry is default */
	{ "[f]",      NULL },    /* no layout function means floating behavior */
	{ "[m]",      monocle },
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
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "sakura", "-c", "90", "-r", "30", NULL };
static const char *stcmd[]    = { "st", NULL };
static const char *wwwcmd[]   = { "chromium", NULL };
static const char *mutecmd[]  = { "amixer", "set", "Master", "toggle", NULL };
static const char *volupcmd[] = { "amixer", "set", "Master", "5%+", NULL };
static const char *voldncmd[] = { "amixer", "set", "Master", "5%-", NULL };
static const char *mpctogglecmd[]  = { "mpc", "toggle", NULL };
static const char *mpcstopcmd[]    = { "mpc", "stop", NULL };
static const char *mpcprevcmd[]    = { "mpc", "prev", NULL };
static const char *mpcnextcmd[]    = { "mpc", "next", NULL };
static const char *mpcvolupcmd[]   = { "mpc", "volume", "+5", NULL };
static const char *mpcvoldowncmd[] = { "mpc", "volume", "-5", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_x,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_grave,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
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
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	/* custom */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = stcmd } },
	{ False,                        XF86XK_HomePage,           spawn,          {.v = wwwcmd } },
	{ False,                        XF86XK_AudioPlay,          spawn,          {.v = mpctogglecmd } },
	{ False,                        XF86XK_AudioStop,          spawn,          {.v = mpcstopcmd } },
	{ False,                        XF86XK_AudioPrev,          spawn,          {.v = mpcprevcmd } },
	{ False,                        XF86XK_AudioNext,          spawn,          {.v = mpcnextcmd } },
	{ MODKEY,                       XK_KP_Add,                 spawn,          {.v = mpcvolupcmd } },
	{ MODKEY,                       XK_KP_Subtract,            spawn,          {.v = mpcvoldowncmd  } },
	{ False,                        XF86XK_AudioMute,          spawn,          {.v = mutecmd  } },
	{ False,                        XF86XK_AudioRaiseVolume,   spawn,          {.v = volupcmd  } },
	{ False,                        XF86XK_AudioLowerVolume,   spawn,          {.v = voldncmd  } },
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

