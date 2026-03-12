#include <X11/XF86keysym.h>
#include "selfrestart.c"

/*
luvrok's dwm build

patches applied (including some slight changes):
dwm-awesomebar-20250923-6.6.diff
dwm-pertag-20200914-61bb8b2.diff
dwm-statuscmd-20241009-8933ebc.diff
dwm-togglelayout-6.6.diff
dwm-togglelayout-6.6-pertag-fix.diff
dwm-steam-6.2.diff
dwm-actualfullscreen-20211013-cb3f58a.diff
dwm-hide_vacant_tags-6.4.diff
dwm-vanitygaps-20200610-f09418b.diff
dwm-fixmultimon-6.4.diff
dwm-focusmaster-return-6.2.diff
dwm-focusmonmouse-6.2.diff
dwm-stacker-6.6.diff
dwm-sticky-6.5.diff
dwm-floatrules-20210801-138b405.diff
dwm-xrdb-6.4.diff
dwm-destroyfocus-20210329-61bb8b2.diff
dwm-r1615-selfrestart.diff
dwm-center-6.2.diff
dwm-preventfocusshift-20240831-6.5.diff
dwm-movecenter-6.5.diff
dwm-xcursor-20250909-74edc27.diff
dwm-borderrule-20231226-e7f651b.diff
dwm-wintype-rules-6.6.diff - ref. https://lists.suckless.org/hackers/2005/17374.html
dwm-ipc-20201106-f04cac6.diff
dwm-noborderflicker-20211227-8657affa2a61.diff
dwm-nobordermonocle-20260112-798c5db.diff
dwm-fixborders-6.2.diff
dwm-taglayouts-6.4.diff

TODO (maybe someday):
https://dwm.suckless.org/patches/swallow/
https://dwm.suckless.org/patches/betterswallow/
https://dwm.suckless.org/patches/dynamicswallow/
https://dwm.suckless.org/patches/restoreafterrestart/ - find out how it can work with selfrestart or find similiar patch
https://dwm.suckless.org/patches/ipc/
patch that can keep aspect ratio during mouse resize for specific windows

*/

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int refreshrate        = 120;      /* refresh rate (per second) for client move/resize */

static const char *fonts[]          = { "JetBrainsMonoNL NFP:size=12", "Font Awesome 6 Free Solid:size=12" };
static const char dmenufont[]       = "JetBrainsMonoNL NFP:size=12";

/* default colors used if xrdb is not loaded */
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#3c3836";
static char normfgcolor[]           = "#ebdbb2";
static char selfgcolor[]            = "#d65d0e";
static char selbordercolor[]        = "#d65d0e";
static char selbgcolor[]            = "#000000";

static char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel]  = { selfgcolor, selbgcolor, selbordercolor },
  [SchemeHid]  = { selbgcolor, normbgcolor, selbgcolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int taglayouts[] = { 1, 0, 0, 1, 0, 0, 0, 0, 0 };

#define WTYPE "_NET_WM_WINDOW_TYPE_"

static const Rule rules[] = {
  /* class             role      instance  title  tags mask   isfloating   iscentered     canfocus  monitor      float x,y,w,h    floatborderpx     border-width */
  { "firefox",         NULL,     NULL,     NULL,  0,          0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
  { NULL,              NULL,     NULL,     "Picture-in-Picture", 0, 1,     0,             1,        -1,          1820,1020,720,400,-1,                0 },
  { "obsidian",        NULL,     NULL,     NULL,  0,          0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
  { "kitty",           NULL,     NULL,     NULL,  0,          0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
  { "dmenu",           NULL,     NULL,     NULL,  0,          1,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },

  { "Spotify",         NULL,     NULL,     NULL,  0,          0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
  { "qBittorrent",     NULL,     NULL,     NULL,  1 << 8,     0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },

  { "Element",         NULL,     NULL,     NULL,  1 << 3,     0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
  { "TelegramDesktop", NULL,     NULL,     NULL,  1 << 3,     0,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
  { "TelegramDesktop", WTYPE "UTILITY", NULL, NULL, 0,        1,           0,             1,        -1,          -1,-1,-1,-1,       0,                0 },
  { "Zathura",         NULL,     NULL,     NULL,  0,          1,           0,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },

  { "nixos_menu_log",  NULL,     NULL,     NULL,  0,          1,           0,             0,        -1,          1820,1020,720,400,-1,                0 },

  { "Dragon-drop",     NULL,     NULL,     NULL,  0,          1,           1,             1,        -1,          -1,-1,-1,-1,      -1,               -1 },
};

static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol  arrange function */
  { "󰓒", tile },      // тайлинг (основная раскладка по умолчанию)
  { "", monocle },   // одно окно на весь экран
  { "󰇥", NULL },      // floating (NULL = плавающие окна)
};

#define MODKEY Mod4Mask

#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
{ MOD, XK_n,     ACTION##stack, {.i = PREVSEL } }, \
{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper for launching gtk application */
#define GTKCMD(cmd) { .v = (const char*[]){ "/usr/bin/gtk-launch", cmd, NULL } }

#define STATUSBAR "dwmblocks"
#define BROWSER  "firefox"
#define TERMINAL  "kitty"

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu", NULL };

void spawn_with_lang_switch(const Arg *arg) {
    system("xkb-switch -s us && pkill -RTMIN+1 dwmblocks");  // change language to english
    spawn(arg);                  // start dmenu_run
}

static const char *firefoxlife[] = { "select-profile-firefox--rofi", "life", NULL};
static const char *element[] = { "element-desktop", NULL };
static const char *throne[] = { "Throne", NULL };
static const char *obsidian[] = { "obsidian", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *firefoxcmd[]  = { "select-profile-firefox--rofi", NULL };
static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *greenclipcmd[] = { "dwm-greenclip", NULL };
static const char *lang_pkill[] = { "pkill", "-RTMIN+1", "dwmblocks", NULL};
static const char *upbrt[] = {"dwm-brightness", "up", NULL};
static const char *downbrt[] = {"dwm-brightness", "down", NULL};
static const char *upvol[] = {"dwm-volume", "up", NULL};
static const char *downvol[] = {"dwm-volume", "down", NULL};
static const char *mutevol[] = {"dwm-volume", "mute", NULL};

static const Key keys[  ] = {
  /* modifier                     key                       function                argument */
  { MODKEY,                       XK_p,                     spawn_with_lang_switch, SHCMD("dmenu_menu") },
  { MODKEY,                       XK_c,                     spawn,                  { .v = greenclipcmd } },
  { MODKEY|ShiftMask,             XK_t,                     spawn,                  { .v = termcmd } },
  { MODKEY|ShiftMask,             XK_f,                     spawn,                  { .v = firefoxcmd } },
  { MODKEY,                       XK_space,                 spawn,                  { .v = lang_pkill } },
  { MODKEY,                       XK_w,                     spawn,                  SHCMD("dwm-power_menu") },
  { MODKEY,                       XK_u,                     spawn,                  SHCMD("rofi-pass") },
  { MODKEY|ShiftMask,             XK_u,                     spawn,                  SHCMD("nixos_menu") },
  { MODKEY|ShiftMask,             XK_d,                     spawn,                  SHCMD("audioswitcher") },

  /* XF86Keys */
  { 0,                            XF86XK_AudioMute,         spawn,                  { .v = mutevol}},
  { 0,                            XF86XK_AudioLowerVolume,  spawn,                  { .v = downvol}},
  { 0,                            XF86XK_AudioRaiseVolume,  spawn,                  { .v = upvol}},
  { 0,                            XF86XK_MonBrightnessUp,   spawn,                  { .v = upbrt}},
  { 0,                            XF86XK_MonBrightnessDown, spawn,                  { .v = downbrt}},
  { 0,                            XK_Print,                 spawn,                  { .v = screenshotcmd } },

  // { MODKEY,                       XK_F7,                    spawn,                  SHCMD("xrandr --output HDMI-A-1 --left-of DisplayPort-0 --scale 1x1 --mode 2560x1440 --rate 120.00") },
  // { MODKEY|ShiftMask,             XK_F7,                    spawn,                  SHCMD("xrandr --output HDMI-A-1 --off") },

  { MODKEY|ShiftMask,             XK_b,                     togglebar,              { 0 } },
  { MODKEY,                       XK_y,                     togglefullscr,          { 0 } },
  STACKKEYS(MODKEY,                                         focus)
  STACKKEYS(MODKEY|ShiftMask,                               push)
  { MODKEY,                       XK_i,                     incnmaster,             { .i = +1 } }, /* increase numbers of master windows */
  { MODKEY,                       XK_d,                     incnmaster,             { .i = -1 } }, /* decrease numbers of master windows */
  { MODKEY,                       XK_h,                     setmfact,               { .f = -0.05  } }, /* decrease size of master windows */
  { MODKEY,                       XK_l,                     setmfact,               { .f = +0.05  } }, /* increase size of master windows */
  { MODKEY|ControlMask,           XK_space,                 focusmaster,            { 0 } },

  { MODKEY|ShiftMask,             XK_c,                     killclient,             { 0 } },
  { MODKEY|ShiftMask,             XK_m,                     zoom,                   { 0 } },
  { MODKEY,                       XK_Tab,                   view,                   { 0 } },

  { MODKEY,                       XK_t,                     setlayout,              { .v = &layouts[0] } },
  { MODKEY,                       XK_f,                     setlayout,              { .v = &layouts[2] } },
  { MODKEY,                       XK_m,                     setlayout,              { .v = &layouts[1] } },
  // { MODKEY,                       XK_space,                 setlayout,              { 0 } },
  { MODKEY|ShiftMask,             XK_space,                 togglefloating,         { 0 } },
  { MODKEY,                       XK_0,                     view,                   { .ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,                     tag,                    { .ui = ~0 } },

  { MODKEY,                       XK_comma,                 focusmon,               { .i = -1 } },
  { MODKEY,                       XK_period,                focusmon,               { .i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,                 tagmon,                 { .i = -1 } },
  { MODKEY|ShiftMask,             XK_period,                tagmon,                 { .i = +1 } },
  { MODKEY|ShiftMask,             XK_a,                     movecenter,             { 0 } },
  { MODKEY,                       XK_F5,                    xrdb,                   { .v = NULL } },
  { MODKEY,                       XK_s,                     show,                   { 0 } },
  { MODKEY|ShiftMask,             XK_s,                     showall,                { 0 } },
  { MODKEY|ShiftMask,             XK_h,                     hide,                   { 0 } },
  // { MODKEY,                       XK_h,                     incrgaps,               { .i = +1 } },
  // { MODKEY,                       XK_l,                     incrgaps,               { .i = -1 } },
  // { MODKEY,                       XK_0,                     togglegaps,             { 0 } },
  // { MODKEY|ShiftMask,             XK_0,                     defaultgaps,            { 0 } },

  TAGKEYS(                        XK_1,                     0)
  TAGKEYS(                        XK_2,                     1)
  TAGKEYS(                        XK_3,                     2)
  TAGKEYS(                        XK_4,                     3)
  TAGKEYS(                        XK_5,                     4)
  TAGKEYS(                        XK_6,                     5)
  TAGKEYS(                        XK_7,                     6)
  TAGKEYS(                        XK_8,                     7)
  TAGKEYS(                        XK_9,                     8)

  { MODKEY|ControlMask|ShiftMask, XK_q,                     self_restart,           { 0 } },
  { MODKEY|ControlMask|ShiftMask, XK_k,                     self_restart_test,      { 0 } },
  { MODKEY|ShiftMask,             XK_BackSpace,             quit,                   { 0 } },
  { MODKEY,                       XK_s,                     togglesticky,           { 0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
  { ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
  { ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
  { ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
  { ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

