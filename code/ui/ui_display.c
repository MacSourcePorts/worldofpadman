/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/*
=======================================================================

DISPLAY OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"

#define BACK0 "menu/buttons/back0"
#define BACK1 "menu/buttons/back1"
#define GRAPHICS0 "menu/buttons/graphics0"
#define GRASHICS1 "menu/buttons/graphics1"
#define DISPLAY0 "menu/buttons/display0"
#define DISPLAY1 "menu/buttons/display1"
#define SOUND0 "menu/buttons/sound0"
#define SOUND1 "menu/buttons/sound1"
#define NETWORK0 "menu/buttons/network0"
#define NETWORK1 "menu/buttons/network1"
#define ACCEPT0 "menu/buttons/accept"
#define ACCEPT1 "menu/buttons/accept"

#define ID_GRAPHICS 10
#define ID_DISPLAY 11
#define ID_SOUND 12
#define ID_NETWORK 13

#define ID_IGNOREHWG 14
#define ID_BRIGHTNESS 15
//#define ID_SCREENSIZE 16
#define ID_SYNCEVERYFRAME 16
#define ID_SIMPLEITEMS 17
#define ID_WALLMARKS 18
#define ID_HIGHQUALITYSKY 19
#define ID_LENSFLARE 20
#define ID_DYNAMICLIGHTS 21
#define ID_FLARES 22
#define ID_INGAMEVIDEO 23
#define ID_ANAGLYPH 24
#define ID_GREYSCALE 25

#define ID_BACK 26

#define XPOSITION 180

typedef struct {
	menuframework_s menu;

	menubitmap_s graphics;
	menubitmap_s display;
	menubitmap_s sound;
	menubitmap_s network;

	menuradiobutton_s ignoreHWG;
	menuslider_s brightness;
//	menuslider_s screensize;
	menuradiobutton_s synceveryframe;
	menuradiobutton_s simpleitems;
	menuradiobutton_s wallmarks;
	menuradiobutton_s highqualitysky;
	menuradiobutton_s lensFlare;
	menuradiobutton_s dynamiclights;
	menuradiobutton_s flares;
	menuradiobutton_s ingamevideo;
	menulist_s anaglyph;
	menuslider_s greyscale;

	menubitmap_s apply;
	menubitmap_s back;
} displayOptionsInfo_t;

static displayOptionsInfo_t displayOptionsInfo;

static const char *anaglyph_names[] = {"off",	   "red-cyan", "red-blue",	"red-green",
									   "cyan-red", "blue-red", "green-red", NULL};

static void ApplyPressed(void *unused, int notification) {
	if (notification != QM_ACTIVATED)
		return;

	trap_Cvar_SetValue("r_ignorehwgamma", (float)displayOptionsInfo.ignoreHWG.curvalue);
	trap_Cvar_SetValue("r_greyscale", (displayOptionsInfo.greyscale.curvalue / 100.0f));

	// hide the button and do the vid restart
	displayOptionsInfo.apply.generic.flags |= QMF_HIDDEN | QMF_INACTIVE;
	trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
}
/*
=================
UI_DisplayOptionsMenu_Event
=================
*/
static void UI_DisplayOptionsMenu_Event(void *ptr, int event) {
	if (event != QM_ACTIVATED) {
		return;
	}

	switch (((menucommon_s *)ptr)->id) {
	case ID_DISPLAY:
	case ID_GREYSCALE:
	case ID_IGNOREHWG:
		break;

	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_BRIGHTNESS:
		trap_Cvar_SetValue("r_gamma", displayOptionsInfo.brightness.curvalue / 10.0f);
		break;

//	case ID_SCREENSIZE:
//		trap_Cvar_SetValue("cg_viewsize", displayOptionsInfo.screensize.curvalue * 10);
//		break;
//
	case ID_SYNCEVERYFRAME:
		trap_Cvar_SetValue("r_finish", displayOptionsInfo.synceveryframe.curvalue);
		break;

	case ID_SIMPLEITEMS:
		trap_Cvar_SetValue("cg_simpleItems", displayOptionsInfo.simpleitems.curvalue);
		break;

	case ID_WALLMARKS:
		trap_Cvar_SetValue("cg_marks", displayOptionsInfo.wallmarks.curvalue);
		break;

	case ID_HIGHQUALITYSKY:
		trap_Cvar_SetValue("r_fastsky", !displayOptionsInfo.highqualitysky.curvalue);
		break;

	case ID_LENSFLARE:
		trap_Cvar_Set("cg_drawLensflare", va("%d", displayOptionsInfo.lensFlare.curvalue));
		break;

	case ID_DYNAMICLIGHTS:
		trap_Cvar_SetValue("r_dynamiclight", displayOptionsInfo.dynamiclights.curvalue);
		break;

	case ID_FLARES:
		trap_Cvar_SetValue("r_flares", displayOptionsInfo.flares.curvalue);
		break;

	case ID_INGAMEVIDEO:
		trap_Cvar_SetValue("r_inGameVideo", displayOptionsInfo.ingamevideo.curvalue);
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_ANAGLYPH:
		trap_Cvar_SetValue("r_anaglyphMode", (float)displayOptionsInfo.anaglyph.curvalue);
		if (!displayOptionsInfo.anaglyph.curvalue) {
			displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
		} else {
			displayOptionsInfo.greyscale.generic.flags &= ~QMF_GRAYED;
		}
		break;
	}
}

static void DisplayOptions_UpdateMenuItems(void) {
	displayOptionsInfo.apply.generic.flags |= QMF_HIDDEN | QMF_INACTIVE;

	if (UI_GetCvarInt("r_ignorehwgamma") != displayOptionsInfo.ignoreHWG.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	} else if (UI_GetCvarInt("r_greyscale") != displayOptionsInfo.greyscale.curvalue) {
		displayOptionsInfo.apply.generic.flags &= ~(QMF_HIDDEN | QMF_INACTIVE);
	}
}

static void DisplayOptions_MenuDraw(void) {
	DisplayOptions_UpdateMenuItems();

	Menu_Draw(&displayOptionsInfo.menu);
}

/*
===============
UI_DisplayOptionsMenu_Init
===============
*/
static void UI_DisplayOptionsMenu_Init(void) {
	int y;

	memset(&displayOptionsInfo, 0, sizeof(displayOptionsInfo));

	UI_DisplayOptionsMenu_Cache();
	displayOptionsInfo.menu.wrapAround = qtrue;
	displayOptionsInfo.menu.fullscreen = qtrue;
	displayOptionsInfo.menu.draw = DisplayOptions_MenuDraw;
	displayOptionsInfo.menu.bgparts = BGP_SYSTEMBG | BGP_SIMPLEBG;

	displayOptionsInfo.graphics.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.graphics.generic.name = GRAPHICS0;
	displayOptionsInfo.graphics.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.graphics.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.graphics.generic.id = ID_GRAPHICS;
	displayOptionsInfo.graphics.generic.x = 16;
	displayOptionsInfo.graphics.generic.y = 37;
	displayOptionsInfo.graphics.width = 160;
	displayOptionsInfo.graphics.height = 40;
	displayOptionsInfo.graphics.focuspic = GRASHICS1;
	displayOptionsInfo.graphics.focuspicinstead = qtrue;

	displayOptionsInfo.display.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.display.generic.name = DISPLAY0;
	displayOptionsInfo.display.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT;
	displayOptionsInfo.display.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.display.generic.id = ID_DISPLAY;
	displayOptionsInfo.display.generic.x = 169;
	displayOptionsInfo.display.generic.y = 30;
	displayOptionsInfo.display.width = 120;
	displayOptionsInfo.display.height = 40;
	displayOptionsInfo.display.focuspic = DISPLAY1;
	displayOptionsInfo.display.focuspicinstead = qtrue;

	displayOptionsInfo.sound.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.sound.generic.name = SOUND0;
	displayOptionsInfo.sound.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.sound.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.sound.generic.id = ID_SOUND;
	displayOptionsInfo.sound.generic.x = 36;
	displayOptionsInfo.sound.generic.y = 79;
	displayOptionsInfo.sound.width = 120;
	displayOptionsInfo.sound.height = 40;
	displayOptionsInfo.sound.focuspic = SOUND1;
	displayOptionsInfo.sound.focuspicinstead = qtrue;

	displayOptionsInfo.network.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.network.generic.name = NETWORK0;
	displayOptionsInfo.network.generic.flags = QMF_LEFT_JUSTIFY | QMF_HIGHLIGHT_IF_FOCUS;
	displayOptionsInfo.network.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.network.generic.id = ID_NETWORK;
	displayOptionsInfo.network.generic.x = 142;
	displayOptionsInfo.network.generic.y = 82;
	displayOptionsInfo.network.width = 160;
	displayOptionsInfo.network.height = 40;
	displayOptionsInfo.network.focuspic = NETWORK1;
	displayOptionsInfo.network.focuspicinstead = qtrue;

	y = 180 + 2 * (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.ignoreHWG.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.ignoreHWG.generic.name = "Ignore HW-Gamma:";
	displayOptionsInfo.ignoreHWG.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.ignoreHWG.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.ignoreHWG.generic.id = ID_IGNOREHWG;
	displayOptionsInfo.ignoreHWG.generic.x = XPOSITION;
	displayOptionsInfo.ignoreHWG.generic.y = y;
	displayOptionsInfo.ignoreHWG.generic.toolTip =
		"If enabled you won't be able to adjust the brightness in game and will be locked and controlled by your "
		"current graphics card and monitor options. It is recommended to leave it off so you can adjust the brightness "
		"via the slider if necessary.";

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.brightness.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.brightness.generic.name = "Brightness:";
	displayOptionsInfo.brightness.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.brightness.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.brightness.generic.id = ID_BRIGHTNESS;
	displayOptionsInfo.brightness.generic.x = XPOSITION;
	displayOptionsInfo.brightness.generic.y = y;
	displayOptionsInfo.brightness.minvalue = 5;
	displayOptionsInfo.brightness.maxvalue = 20;
	if (!uis.glconfig.deviceSupportsGamma)
		displayOptionsInfo.brightness.generic.flags |= QMF_GRAYED;

/*	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.screensize.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.screensize.generic.name = "Screen Size:";
	displayOptionsInfo.screensize.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.screensize.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.screensize.generic.id = ID_SCREENSIZE;
	displayOptionsInfo.screensize.generic.x = XPOSITION;
	displayOptionsInfo.screensize.generic.y = y;
	displayOptionsInfo.screensize.minvalue = 3;
    displayOptionsInfo.screensize.maxvalue = 10;
*/
	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.synceveryframe.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.synceveryframe.generic.name = "Sync Every Frame:";
	displayOptionsInfo.synceveryframe.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.synceveryframe.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.synceveryframe.generic.id = ID_SYNCEVERYFRAME;
	displayOptionsInfo.synceveryframe.generic.x = XPOSITION;
	displayOptionsInfo.synceveryframe.generic.y = y;
	displayOptionsInfo.synceveryframe.generic.toolTip =
		"Also known as V-SYNC. Enable only if you're experiencing graphical horizontal tearing artifacts.";

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.simpleitems.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.simpleitems.generic.name = "Simple Items:";
	displayOptionsInfo.simpleitems.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.simpleitems.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.simpleitems.generic.id = ID_SIMPLEITEMS;
	displayOptionsInfo.simpleitems.generic.x = XPOSITION;
	displayOptionsInfo.simpleitems.generic.y = y;
	displayOptionsInfo.simpleitems.generic.toolTip = "Enable this to see all 3d items replaced with 2d icons.";

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.wallmarks.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.wallmarks.generic.name = "Marks on Walls:";
	displayOptionsInfo.wallmarks.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.wallmarks.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.wallmarks.generic.id = ID_WALLMARKS;
	displayOptionsInfo.wallmarks.generic.x = XPOSITION;
	displayOptionsInfo.wallmarks.generic.y = y;
	displayOptionsInfo.wallmarks.generic.toolTip = "Enable this to see weapon effects on surfaces.";

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.highqualitysky.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.highqualitysky.generic.name = "High Quality Sky:";
	displayOptionsInfo.highqualitysky.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.highqualitysky.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.highqualitysky.generic.id = ID_HIGHQUALITYSKY;
	displayOptionsInfo.highqualitysky.generic.x = XPOSITION;
	displayOptionsInfo.highqualitysky.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.lensFlare.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.lensFlare.generic.name = "Sky Lens Flare:";
	displayOptionsInfo.lensFlare.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.lensFlare.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.lensFlare.generic.id = ID_LENSFLARE;
	displayOptionsInfo.lensFlare.generic.x = XPOSITION;
	displayOptionsInfo.lensFlare.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.dynamiclights.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.dynamiclights.generic.name = "Dynamic Lights:";
	displayOptionsInfo.dynamiclights.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.dynamiclights.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.dynamiclights.generic.id = ID_DYNAMICLIGHTS;
	displayOptionsInfo.dynamiclights.generic.x = XPOSITION;
	displayOptionsInfo.dynamiclights.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.flares.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.flares.generic.name = "Dynamic Flares:";
	displayOptionsInfo.flares.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.flares.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.flares.generic.id = ID_FLARES;
	displayOptionsInfo.flares.generic.x = XPOSITION;
	displayOptionsInfo.flares.generic.y = y;

	y += BIGCHAR_HEIGHT + 2;
	displayOptionsInfo.ingamevideo.generic.type = MTYPE_RADIOBUTTON;
	displayOptionsInfo.ingamevideo.generic.name = "Ingame Videos:";
	displayOptionsInfo.ingamevideo.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.ingamevideo.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.ingamevideo.generic.id = ID_FLARES;
	displayOptionsInfo.ingamevideo.generic.x = XPOSITION;
	displayOptionsInfo.ingamevideo.generic.y = y;

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.anaglyph.generic.type = MTYPE_SPINCONTROL;
	displayOptionsInfo.anaglyph.generic.name = "Stereoscopic 3D:";
	displayOptionsInfo.anaglyph.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.anaglyph.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.anaglyph.generic.id = ID_ANAGLYPH;
	displayOptionsInfo.anaglyph.generic.x = XPOSITION;
	displayOptionsInfo.anaglyph.generic.y = y;
	displayOptionsInfo.anaglyph.itemnames = anaglyph_names;
	displayOptionsInfo.anaglyph.generic.toolTip =
		"Switch on to play in 3D with the appriopriate glasses. Ensure the correct filter option you select matches "
		"that of your glasses or the effect won't work at all.";

	y += (BIGCHAR_HEIGHT + 2);
	displayOptionsInfo.greyscale.generic.type = MTYPE_SLIDER;
	displayOptionsInfo.greyscale.generic.name = "Greyscale:";
	displayOptionsInfo.greyscale.generic.flags = QMF_SMALLFONT;
	displayOptionsInfo.greyscale.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.greyscale.generic.id = ID_GREYSCALE;
	displayOptionsInfo.greyscale.generic.x = XPOSITION;
	displayOptionsInfo.greyscale.generic.y = y;
	displayOptionsInfo.greyscale.minvalue = 0;
	displayOptionsInfo.greyscale.maxvalue = 100;

	displayOptionsInfo.apply.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.apply.generic.name = ACCEPT0;
	displayOptionsInfo.apply.generic.flags = QMF_PULSEIFFOCUS | QMF_HIDDEN | QMF_INACTIVE;
	displayOptionsInfo.apply.generic.callback = ApplyPressed;
	displayOptionsInfo.apply.generic.x = 512;
	displayOptionsInfo.apply.generic.y = 440;
	displayOptionsInfo.apply.width = 120;
	displayOptionsInfo.apply.height = 40;
	displayOptionsInfo.apply.focuspic = ACCEPT1;

	displayOptionsInfo.back.generic.type = MTYPE_BITMAP;
	displayOptionsInfo.back.generic.name = BACK0;
	displayOptionsInfo.back.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	displayOptionsInfo.back.generic.callback = UI_DisplayOptionsMenu_Event;
	displayOptionsInfo.back.generic.id = ID_BACK;
	displayOptionsInfo.back.generic.x = 8;
	displayOptionsInfo.back.generic.y = 440;
	displayOptionsInfo.back.width = 80;
	displayOptionsInfo.back.height = 40;
	displayOptionsInfo.back.focuspic = BACK1;
	displayOptionsInfo.back.focuspicinstead = qtrue;

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.graphics);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.display);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.sound);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.network);

	Menu_AddItem(&displayOptionsInfo.menu, &displayOptionsInfo.ignoreHWG);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.brightness);
//	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.screensize);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.synceveryframe);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.simpleitems);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.wallmarks);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.highqualitysky);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.lensFlare);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.dynamiclights);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.flares);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.ingamevideo);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.anaglyph);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.greyscale);

	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.apply);
	Menu_AddItem(&displayOptionsInfo.menu, (void *)&displayOptionsInfo.back);

	displayOptionsInfo.ignoreHWG.curvalue = UI_GetCvarInt("r_ignorehwgamma");
	displayOptionsInfo.brightness.curvalue = trap_Cvar_VariableValue("r_gamma") * 10;
//	displayOptionsInfo.screensize.curvalue = trap_Cvar_VariableValue("cg_viewsize") / 10;
	displayOptionsInfo.synceveryframe.curvalue = trap_Cvar_VariableValue("r_finish") != 0;
	displayOptionsInfo.simpleitems.curvalue = trap_Cvar_VariableValue("cg_simpleItems") != 0;
	displayOptionsInfo.wallmarks.curvalue = trap_Cvar_VariableValue("cg_marks") != 0;
	displayOptionsInfo.highqualitysky.curvalue = trap_Cvar_VariableValue("r_fastsky") == 0;
	displayOptionsInfo.lensFlare.curvalue = trap_Cvar_VariableValue("cg_drawLensflare") != 0;
	displayOptionsInfo.dynamiclights.curvalue = trap_Cvar_VariableValue("r_dynamiclight") != 0;
	displayOptionsInfo.flares.curvalue = trap_Cvar_VariableValue("r_flares") != 0;
	displayOptionsInfo.ingamevideo.curvalue = trap_Cvar_VariableValue("r_inGameVideo") != 0;
	displayOptionsInfo.anaglyph.curvalue =
		Com_Clamp(0, (ARRAY_LEN(anaglyph_names) - 1), trap_Cvar_VariableValue("r_anaglyphMode"));
	displayOptionsInfo.greyscale.curvalue = Com_Clamp(0, 100, (trap_Cvar_VariableValue("r_greyscale") * 100));

	if (!displayOptionsInfo.anaglyph.curvalue) {
		displayOptionsInfo.greyscale.generic.flags |= QMF_GRAYED;
	}
}

/*
===============
UI_DisplayOptionsMenu_Cache
===============
*/
void UI_DisplayOptionsMenu_Cache(void) {
	trap_R_RegisterShaderNoMip(BACK0);
	trap_R_RegisterShaderNoMip(BACK1);
	trap_R_RegisterShaderNoMip(GRAPHICS0);
	trap_R_RegisterShaderNoMip(GRASHICS1);
	trap_R_RegisterShaderNoMip(DISPLAY0);
	trap_R_RegisterShaderNoMip(DISPLAY1);
	trap_R_RegisterShaderNoMip(SOUND0);
	trap_R_RegisterShaderNoMip(SOUND1);
	trap_R_RegisterShaderNoMip(NETWORK0);
	trap_R_RegisterShaderNoMip(NETWORK1);
}

/*
===============
UI_DisplayOptionsMenu
===============
*/
void UI_DisplayOptionsMenu(void) {
	UI_DisplayOptionsMenu_Init();
	UI_PushMenu(&displayOptionsInfo.menu);
	Menu_SetCursorToItem(&displayOptionsInfo.menu, &displayOptionsInfo.display);
}
