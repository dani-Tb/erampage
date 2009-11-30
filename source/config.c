//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2000, 2003 - Matt Saettler (EDuke Enhancements)
Copyright (C) 2004, 2007 - EDuke32 developers
Copyright (C) 2009 - dimag0g

This file is part of eRampage port for Redneck Rampage series, derived from EDuke32

eRampage is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
//-------------------------------------------------------------------------

/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
*/

#include "baselayer.h"
#include "duke3d.h"
#include "scriplib.h"
#include "osd.h"
#include "osdcmds.h"
#include "osdfuncs.h"

// we load this in to get default button and key assignments
// as well as setting up function mappings

#define __SETUP__   // JBF 20031211
#include "_functio.h"

/*
===================
=
= CONFIG_FunctionNameToNum
=
===================
*/

hashtable_t gamefuncH    = { NUMGAMEFUNCTIONS<<1, NULL };

int32_t CONFIG_FunctionNameToNum(char * func) {
    int32_t i;

    i = hash_find(&gamefuncH,func);

    if (i < 0) {
        char *str = strtolower(Bstrdup(func),Bstrlen(func));
        i = hash_find(&gamefuncH,str);
        Bfree(str);

        return i;
    }

    return i;
}

/*
===================
=
= CONFIG_FunctionNumToName
=
===================
*/

char * CONFIG_FunctionNumToName(int32_t func) {
    if ((unsigned)func >= (unsigned)NUMGAMEFUNCTIONS)
        return NULL;
    return gamefunctions[func];
}

/*
===================
=
= CONFIG_AnalogNameToNum
=
===================
*/


int32_t CONFIG_AnalogNameToNum(char * func) {

    if (!Bstrcasecmp(func,"analog_turning")) {
        return analog_turning;
    }
    if (!Bstrcasecmp(func,"analog_strafing")) {
        return analog_strafing;
    }
    if (!Bstrcasecmp(func,"analog_moving")) {
        return analog_moving;
    }
    if (!Bstrcasecmp(func,"analog_lookingupanddown")) {
        return analog_lookingupanddown;
    }

    return -1;
}


char * CONFIG_AnalogNumToName(int32_t func) {
    switch (func) {
    case analog_turning:
        return "analog_turning";
    case analog_strafing:
        return "analog_strafing";
    case analog_moving:
        return "analog_moving";
    case analog_lookingupanddown:
        return "analog_lookingupanddown";
    }

    return NULL;
}


/*
===================
=
= CONFIG_SetDefaults
=
===================
*/

void CONFIG_SetDefaultKeys(int32_t type) {
    int32_t i,f;

    Bmemset(ud.config.KeyboardKeys, 0xff, sizeof(ud.config.KeyboardKeys));
    Bmemset(&KeyBindings,0,sizeof(KeyBindings));
    Bmemset(&MouseBindings,0,sizeof(MouseBindings));

    if (type == 1) {
        for (i=0; i < (int32_t)(sizeof(oldkeydefaults)/sizeof(oldkeydefaults[0])); i+=3) {
            f = CONFIG_FunctionNameToNum((char *)oldkeydefaults[i+0]);
            if (f == -1) continue;
            ud.config.KeyboardKeys[f][0] = KB_StringToScanCode((char *)oldkeydefaults[i+1]);
            ud.config.KeyboardKeys[f][1] = KB_StringToScanCode((char *)oldkeydefaults[i+2]);

            if (f == gamefunc_Show_Console) OSD_CaptureKey(ud.config.KeyboardKeys[f][0]);
            else CONFIG_MapKey(f, ud.config.KeyboardKeys[f][0], 0, ud.config.KeyboardKeys[f][1], 0);
        }
        return;
    }

    for (i=0; i < (int32_t)(sizeof(keydefaults)/sizeof(keydefaults[0])); i+=3) {
        f = CONFIG_FunctionNameToNum(keydefaults[i+0]);
        if (f == -1) continue;
        ud.config.KeyboardKeys[f][0] = KB_StringToScanCode(keydefaults[i+1]);
        ud.config.KeyboardKeys[f][1] = KB_StringToScanCode(keydefaults[i+2]);

        if (f == gamefunc_Show_Console) OSD_CaptureKey(ud.config.KeyboardKeys[f][0]);
        else CONFIG_MapKey(f, ud.config.KeyboardKeys[f][0], 0, ud.config.KeyboardKeys[f][1], 0);
    }
}

void CONFIG_SetDefaults(void) {
    // JBF 20031211
    int32_t i;

    ud.config.scripthandle = -1;
    ud.config.ScreenWidth = 1024;
    ud.config.ScreenHeight = 768;
    ud.config.ScreenMode = 0;
#if defined(POLYMOST) && defined(USE_OPENGL)
    ud.config.ScreenBPP = 32;
#else
    ud.config.ScreenBPP = 8;
#endif
    ud.config.useprecache = 1;
    ud.config.ForceSetup = 1;
    ud.config.NoAutoLoad = 1;
    ud.config.AmbienceToggle = 1;
    ud.config.AutoAim = 1;
    ud.config.FXDevice = 0;
    ud.config.FXVolume = 220;
#if defined(_WIN32)
    ud.config.MixRate = 44100;
#else
    ud.config.MixRate = 48000;
#endif
    ud.config.MouseBias = 0;
    ud.config.MouseDeadZone = 0;
    ud.config.MusicDevice = 0;
    ud.config.MusicToggle = 1;
    ud.config.MusicVolume = 200;
    g_myAimMode = g_player[0].ps->aim_mode = 1;
    ud.config.NumBits = 16;
    ud.config.NumChannels = 2;
    ud.config.NumVoices = 32;
    ud.config.ReverseStereo = 0;
    ud.config.RunMode = ud.auto_run = 1;
    ud.config.ShowOpponentWeapons = 0;
    ud.config.SmoothInput = 1;
    ud.config.SoundToggle = 1;
    ud.althud = 1;
    ud.automsg = 0;
    ud.autovote = 0;
    ud.brightness = 8;
    ud.camerasprite = -1;
    ud.color = 0;
    ud.crosshair = 1;
    ud.crosshairscale = 50;
    ud.obituaries = 1;
    ud.democams = 1;
    ud.detail = 1;
    ud.drawweapon = 1;
    ud.idplayers = 1;
    ud.levelstats = 0;
    ud.lockout = 0;
    ud.m_ffire = 1;
    ud.m_marker = 1;
    ud.mouseaiming = 0;
    ud.mouseflip = 1;
    ud.msgdisptime = 120;
    ud.pwlockout[0] = '\0';
    ud.runkey_mode = 0;
    ud.screen_size = 4;
    ud.screen_tilting = 1;
    ud.shadows = 1;
    ud.statusbarmode = 0;
    ud.statusbarscale = 100;
    ud.team = 0;
    ud.viewbob = 1;
    ud.weaponsway = 1;
    ud.weaponswitch = 3;	// new+empty
    ud.angleinterpolation = 0;
    ud.config.UseJoystick = 0;
    ud.config.UseMouse = 1;
    ud.config.VoiceToggle = 5; // bitfield, 1 = local, 2 = dummy, 4 = other players in DM
    ud.display_bonus_screen = 1;
    ud.show_level_text = 1;
    ud.configversion = 0;
    ud.weaponscale = 100;
    ud.textscale = 100;

    ud.config.CheckForUpdates = 1;

    Bstrcpy(ud.rtsname, "DUKE.RTS");
    Bstrcpy(szPlayerName, "Duke");

    Bstrcpy(ud.ridecule[0], "An inspiration for birth control.");
    Bstrcpy(ud.ridecule[1], "You're gonna die for that!");
    Bstrcpy(ud.ridecule[2], "It hurts to be you.");
    Bstrcpy(ud.ridecule[3], "Lucky Son of a Bitch.");
    Bstrcpy(ud.ridecule[4], "Hmmm....Payback time.");
    Bstrcpy(ud.ridecule[5], "You bottom dwelling scum sucker.");
    Bstrcpy(ud.ridecule[6], "Damn, you're ugly.");
    Bstrcpy(ud.ridecule[7], "Ha ha ha...Wasted!");
    Bstrcpy(ud.ridecule[8], "You suck!");
    Bstrcpy(ud.ridecule[9], "AARRRGHHHHH!!!");

    // JBF 20031211

    CONFIG_SetDefaultKeys(0);

    memset(ud.config.MouseFunctions, -1, sizeof(ud.config.MouseFunctions));
    for (i=0; i<MAXMOUSEBUTTONS; i++) {
        ud.config.MouseFunctions[i][0] = CONFIG_FunctionNameToNum(mousedefaults[i]);
        CONTROL_MapButton(ud.config.MouseFunctions[i][0], i, 0, controldevice_mouse);
        if (i>=4) continue;
        ud.config.MouseFunctions[i][1] = CONFIG_FunctionNameToNum(mouseclickeddefaults[i]);
        CONTROL_MapButton(ud.config.MouseFunctions[i][1], i, 1, controldevice_mouse);
    }

    memset(ud.config.MouseDigitalFunctions, -1, sizeof(ud.config.MouseDigitalFunctions));
    for (i=0; i<MAXMOUSEAXES; i++) {
        ud.config.MouseAnalogueScale[i] = 65536;
        CONTROL_SetAnalogAxisScale(i, ud.config.MouseAnalogueScale[i], controldevice_mouse);

        ud.config.MouseDigitalFunctions[i][0] = CONFIG_FunctionNameToNum(mousedigitaldefaults[i*2]);
        ud.config.MouseDigitalFunctions[i][1] = CONFIG_FunctionNameToNum(mousedigitaldefaults[i*2+1]);
        CONTROL_MapDigitalAxis(i, ud.config.MouseDigitalFunctions[i][0], 0, controldevice_mouse);
        CONTROL_MapDigitalAxis(i, ud.config.MouseDigitalFunctions[i][1], 1, controldevice_mouse);

        ud.config.MouseAnalogueAxes[i] = CONFIG_AnalogNameToNum(mouseanalogdefaults[i]);
        CONTROL_MapAnalogAxis(i, ud.config.MouseAnalogueAxes[i], controldevice_mouse);
    }
    CONTROL_MouseSensitivity = DEFAULTMOUSESENSITIVITY;

    memset(ud.config.JoystickFunctions, -1, sizeof(ud.config.JoystickFunctions));
    for (i=0; i<MAXJOYBUTTONS; i++) {
        ud.config.JoystickFunctions[i][0] = CONFIG_FunctionNameToNum(joystickdefaults[i]);
        ud.config.JoystickFunctions[i][1] = CONFIG_FunctionNameToNum(joystickclickeddefaults[i]);
        CONTROL_MapButton(ud.config.JoystickFunctions[i][0], i, 0, controldevice_joystick);
        CONTROL_MapButton(ud.config.JoystickFunctions[i][1], i, 1, controldevice_joystick);
    }

    memset(ud.config.JoystickDigitalFunctions, -1, sizeof(ud.config.JoystickDigitalFunctions));
    for (i=0; i<MAXJOYAXES; i++) {
        ud.config.JoystickAnalogueScale[i] = 65536;
        ud.config.JoystickAnalogueDead[i] = 1000;
        ud.config.JoystickAnalogueSaturate[i] = 9500;
        CONTROL_SetAnalogAxisScale(i, ud.config.JoystickAnalogueScale[i], controldevice_joystick);

        ud.config.JoystickDigitalFunctions[i][0] = CONFIG_FunctionNameToNum(joystickdigitaldefaults[i*2]);
        ud.config.JoystickDigitalFunctions[i][1] = CONFIG_FunctionNameToNum(joystickdigitaldefaults[i*2+1]);
        CONTROL_MapDigitalAxis(i, ud.config.JoystickDigitalFunctions[i][0], 0, controldevice_joystick);
        CONTROL_MapDigitalAxis(i, ud.config.JoystickDigitalFunctions[i][1], 1, controldevice_joystick);

        ud.config.JoystickAnalogueAxes[i] = CONFIG_AnalogNameToNum(joystickanalogdefaults[i]);
        CONTROL_MapAnalogAxis(i, ud.config.JoystickAnalogueAxes[i], controldevice_joystick);
    }
}
/*
===================
=
= CONFIG_ReadKeys
=
===================
*/

void CONFIG_ReadKeys(void) {
    int32_t i;
    int32_t numkeyentries;
    int32_t function;
    char keyname1[80];
    char keyname2[80];
    kb_scancode key1,key2;

    if (ud.config.scripthandle < 0) return;

    numkeyentries = SCRIPT_NumberEntries(ud.config.scripthandle,"KeyDefinitions");

    Bmemset(&KeyBindings,0,sizeof(KeyBindings));

    for (i=0; i<numkeyentries; i++) {
        function = CONFIG_FunctionNameToNum(SCRIPT_Entry(ud.config.scripthandle,"KeyDefinitions", i));
        if (function != -1) {
            memset(keyname1,0,sizeof(keyname1));
            memset(keyname2,0,sizeof(keyname2));
            SCRIPT_GetDoubleString
            (
                ud.config.scripthandle,
                "KeyDefinitions",
                SCRIPT_Entry(ud.config.scripthandle, "KeyDefinitions", i),
                keyname1,
                keyname2
            );
            key1 = 0xff;
            key2 = 0xff;
            if (keyname1[0]) {
                key1 = (uint8_t) KB_StringToScanCode(keyname1);
            }
            if (keyname2[0]) {
                key2 = (uint8_t) KB_StringToScanCode(keyname2);
            }
            ud.config.KeyboardKeys[function][0] = key1;
            ud.config.KeyboardKeys[function][1] = key2;
        }
    }

    for (i=0; i<NUMGAMEFUNCTIONS; i++) {
        if (i == gamefunc_Show_Console)
            OSD_CaptureKey(ud.config.KeyboardKeys[i][0]);
        else
            CONFIG_MapKey(i, ud.config.KeyboardKeys[i][0], 0, ud.config.KeyboardKeys[i][1], 0);
    }
}

// wrapper for CONTROL_MapKey(), generates key bindings to reflect changes to keyboard setup
void CONFIG_MapKey(int32_t which, kb_scancode key1, kb_scancode oldkey1, kb_scancode key2, kb_scancode oldkey2) {
    int32_t i, j, k;
    int32_t ii[] = { key1, key2, oldkey1, oldkey2 };
    char buf[64];

    CONTROL_MapKey(which, key1, key2);

    for (k = 0; (unsigned)k < (sizeof(ii) / sizeof(ii[0])); k++) {
        if (ii[k] == 0xff || !ii[k]) continue;

        for (j=0; ConsoleKeys[j].name; j++)
            if (ii[k] == ConsoleKeys[j].id)
                break;
        if (ConsoleKeys[j].name)
            KeyBindings[ii[k]].key=Bstrdup(ConsoleKeys[j].name);

        KeyBindings[ii[k]].repeat = 1;
        KeyBindings[ii[k]].cmd[0] = 0;
        tempbuf[0] = 0;

        for (i=NUMGAMEFUNCTIONS-1; i>=0; i--) {
            if (ud.config.KeyboardKeys[i][0] == ii[k] || ud.config.KeyboardKeys[i][1] == ii[k]) {
                Bsprintf(buf,"gamefunc_%s; ",CONFIG_FunctionNumToName(i));
                Bstrcat(tempbuf,buf);
            }
        }
        Bstrncpy(KeyBindings[ii[k]].cmd,tempbuf, MAXBINDSTRINGLENGTH-1);

        i = Bstrlen(KeyBindings[ii[k]].cmd);
        if (i)
            KeyBindings[ii[k]].cmd[i-2] = 0; // cut off the trailing "; "
    }
}

/*
===================
=
= CONFIG_SetupMouse
=
===================
*/

void CONFIG_SetupMouse(void) {
    int32_t i;
    char str[80];
    char temp[80];
    int32_t scale;

    if (ud.config.scripthandle < 0) return;

    for (i=0; i<MAXMOUSEBUTTONS; i++) {
        Bsprintf(str,"MouseButton%d",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle,"Controls", str,temp))
            ud.config.MouseFunctions[i][0] = CONFIG_FunctionNameToNum(temp);

        Bsprintf(str,"MouseButtonClicked%d",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle,"Controls", str,temp))
            ud.config.MouseFunctions[i][1] = CONFIG_FunctionNameToNum(temp);
    }

    // map over the axes
    for (i=0; i<MAXMOUSEAXES; i++) {
        Bsprintf(str,"MouseAnalogAxes%d",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle, "Controls", str,temp))
            if (CONFIG_AnalogNameToNum(temp) != -1 || (!temp[0] && CONFIG_FunctionNameToNum(temp) != -1))
                ud.config.MouseAnalogueAxes[i] = CONFIG_AnalogNameToNum(temp);

        Bsprintf(str,"MouseDigitalAxes%d_0",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle, "Controls", str,temp))
            if (CONFIG_FunctionNameToNum(temp) != -1 || (!temp[0] && CONFIG_FunctionNameToNum(temp) != -1))
                ud.config.MouseDigitalFunctions[i][0] = CONFIG_FunctionNameToNum(temp);

        Bsprintf(str,"MouseDigitalAxes%d_1",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle, "Controls", str,temp))
            if (CONFIG_FunctionNameToNum(temp) != -1 || (!temp[0] && CONFIG_FunctionNameToNum(temp) != -1))
                ud.config.MouseDigitalFunctions[i][1] = CONFIG_FunctionNameToNum(temp);

        Bsprintf(str,"MouseAnalogScale%d",i);
        scale = ud.config.MouseAnalogueScale[i];
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls", str,&scale);
        ud.config.MouseAnalogueScale[i] = scale;
    }

    {
        tempbuf[0] = 0;
        SCRIPT_GetString(ud.config.scripthandle, "Controls","Mouse_Sensitivity",&tempbuf[0]);
        if (tempbuf[0]) CONTROL_MouseSensitivity = atof(tempbuf);
    }

    for (i=0; i<MAXMOUSEBUTTONS; i++) {
        CONTROL_MapButton(ud.config.MouseFunctions[i][0], i, 0, controldevice_mouse);
        CONTROL_MapButton(ud.config.MouseFunctions[i][1], i, 1,  controldevice_mouse);
    }
    for (i=0; i<MAXMOUSEAXES; i++) {
        CONTROL_MapAnalogAxis(i, ud.config.MouseAnalogueAxes[i], controldevice_mouse);
        CONTROL_MapDigitalAxis(i, ud.config.MouseDigitalFunctions[i][0], 0,controldevice_mouse);
        CONTROL_MapDigitalAxis(i, ud.config.MouseDigitalFunctions[i][1], 1,controldevice_mouse);
        CONTROL_SetAnalogAxisScale(i, ud.config.MouseAnalogueScale[i], controldevice_mouse);
    }
}

/*
===================
=
= CONFIG_SetupJoystick
=
===================
*/

void CONFIG_SetupJoystick(void) {
    int32_t i;
    char str[80];
    char temp[80];
    int32_t scale;

    if (ud.config.scripthandle < 0) return;

    for (i=0; i<MAXJOYBUTTONS; i++) {
        Bsprintf(str,"JoystickButton%d",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle,"Controls", str,temp))
            ud.config.JoystickFunctions[i][0] = CONFIG_FunctionNameToNum(temp);

        Bsprintf(str,"JoystickButtonClicked%d",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle,"Controls", str,temp))
            ud.config.JoystickFunctions[i][1] = CONFIG_FunctionNameToNum(temp);
    }

    // map over the axes
    for (i=0; i<MAXJOYAXES; i++) {
        Bsprintf(str,"JoystickAnalogAxes%d",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle, "Controls", str,temp))
            if (CONFIG_AnalogNameToNum(temp) != -1 || (!temp[0] && CONFIG_FunctionNameToNum(temp) != -1))
                ud.config.JoystickAnalogueAxes[i] = CONFIG_AnalogNameToNum(temp);

        Bsprintf(str,"JoystickDigitalAxes%d_0",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle, "Controls", str,temp))
            if (CONFIG_FunctionNameToNum(temp) != -1 || (!temp[0] && CONFIG_FunctionNameToNum(temp) != -1))
                ud.config.JoystickDigitalFunctions[i][0] = CONFIG_FunctionNameToNum(temp);

        Bsprintf(str,"JoystickDigitalAxes%d_1",i);
        temp[0] = 0;
        if (!SCRIPT_GetString(ud.config.scripthandle, "Controls", str,temp))
            if (CONFIG_FunctionNameToNum(temp) != -1 || (!temp[0] && CONFIG_FunctionNameToNum(temp) != -1))
                ud.config.JoystickDigitalFunctions[i][1] = CONFIG_FunctionNameToNum(temp);

        Bsprintf(str,"JoystickAnalogScale%d",i);
        scale = ud.config.JoystickAnalogueScale[i];
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls", str,&scale);
        ud.config.JoystickAnalogueScale[i] = scale;

        Bsprintf(str,"JoystickAnalogDead%d",i);
        scale = ud.config.JoystickAnalogueDead[i];
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls", str,&scale);
        ud.config.JoystickAnalogueDead[i] = scale;

        Bsprintf(str,"JoystickAnalogSaturate%d",i);
        scale = ud.config.JoystickAnalogueSaturate[i];
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls", str,&scale);
        ud.config.JoystickAnalogueSaturate[i] = scale;
    }

    for (i=0; i<MAXJOYBUTTONS; i++) {
        CONTROL_MapButton(ud.config.JoystickFunctions[i][0], i, 0, controldevice_joystick);
        CONTROL_MapButton(ud.config.JoystickFunctions[i][1], i, 1,  controldevice_joystick);
    }
    for (i=0; i<MAXJOYAXES; i++) {
        CONTROL_MapAnalogAxis(i, ud.config.JoystickAnalogueAxes[i], controldevice_joystick);
        CONTROL_MapDigitalAxis(i, ud.config.JoystickDigitalFunctions[i][0], 0, controldevice_joystick);
        CONTROL_MapDigitalAxis(i, ud.config.JoystickDigitalFunctions[i][1], 1, controldevice_joystick);
        CONTROL_SetAnalogAxisScale(i, ud.config.JoystickAnalogueScale[i], controldevice_joystick);
    }
}

/*
===================
=
= CONFIG_ReadSetup
=
===================
*/
extern char *duke3dgrp;
extern void G_CheckPlayerColor(int32_t *color,int32_t prev_color);
extern palette_t CrosshairColors;
extern palette_t DefaultCrosshairColors;
extern char mod_dir[BMAX_PATH];
extern int32_t r_maxfps;
extern int32_t g_noSetup;

int32_t CONFIG_ReadSetup(void) {
    int32_t dummy, i = 0;
    char commmacro[] = "CommbatMacro# ";
    extern int32_t CommandWeaponChoice;
    char tempbuf[1024];

    CONTROL_ClearAssignments();
    CONFIG_SetDefaults();

    ud.config.setupread = 1;

    pathsearchmode = 1;
    if (SafeFileExists(setupfilename) && ud.config.scripthandle < 0)  // JBF 20031211
        ud.config.scripthandle = SCRIPT_Load(setupfilename);
    else if (SafeFileExists(SETUPFILENAME) && ud.config.scripthandle < 0) {
        Bsprintf(tempbuf,"The configuration file \"%s\" was not found. "
                 "Import configuration data from \"%s\"?",setupfilename,SETUPFILENAME);

        i=wm_ynbox("Import Configuration Settings",tempbuf);
        if (i) ud.config.scripthandle = SCRIPT_Load(SETUPFILENAME);
    } else if (SafeFileExists("duke3d.cfg") && ud.config.scripthandle < 0) {
        Bsprintf(tempbuf,"The configuration file \"%s\" was not found. "
                 "Import configuration data from \"duke3d.cfg\"?",setupfilename);

        i=wm_ynbox("Import Configuration Settings",tempbuf);
        if (i) ud.config.scripthandle = SCRIPT_Load("duke3d.cfg");
    }
    pathsearchmode = 0;

    if (ud.config.scripthandle < 0) return -1;

    if (ud.config.scripthandle >= 0) {
        char dummybuf[64];

        for (dummy = 0; dummy < 10; dummy++) {
            commmacro[13] = dummy+'0';
            SCRIPT_GetString(ud.config.scripthandle, "Comm Setup",commmacro,&ud.ridecule[dummy][0]);
        }

        SCRIPT_GetString(ud.config.scripthandle, "Comm Setup","PlayerName",&tempbuf[0]);

        while (Bstrlen(stripcolorcodes(dummybuf,tempbuf)) > 10)
            tempbuf[Bstrlen(tempbuf)-1] = '\0';

        Bstrncpy(szPlayerName,tempbuf,sizeof(szPlayerName)-1);
        szPlayerName[sizeof(szPlayerName)-1] = '\0';

        SCRIPT_GetString(ud.config.scripthandle, "Comm Setup","RTSName",&ud.rtsname[0]);

#ifndef RANCID_NETWORKING
        // The packetrate mechanism is specific to the eduke32 networking code
        SCRIPT_GetNumber(ud.config.scripthandle, "Comm Setup", "Rate",(int32_t *)&packetrate);
        packetrate = min(max(packetrate,50),1000);
#endif

        SCRIPT_GetNumber(ud.config.scripthandle, "Setup","ConfigVersion",&ud.configversion);
        SCRIPT_GetNumber(ud.config.scripthandle, "Setup","ForceSetup",&ud.config.ForceSetup);
        SCRIPT_GetNumber(ud.config.scripthandle, "Setup","NoAutoLoad",&ud.config.NoAutoLoad);

#ifdef _WIN32
        if (g_noSetup == 0 && mod_dir[0] == '/') {
            struct stat st;
            SCRIPT_GetString(ud.config.scripthandle, "Setup","ModDir",&mod_dir[0]);

            if (stat(mod_dir, &st) >= 0) {
                if ((st.st_mode & S_IFDIR) != S_IFDIR) {
                    initprintf("Invalid mod dir in cfg!\n");
                    Bsprintf(mod_dir,"/");
                }
            }
        }
#endif

        {
            extern char defaultduke3dgrp[BMAX_PATH];
            if (!Bstrcmp(defaultduke3dgrp,"duke3d.grp"))
                SCRIPT_GetString(ud.config.scripthandle, "Setup","SelectedGRP",&duke3dgrp[0]);
        }

        {
            tempbuf[0] = 0;
            SCRIPT_GetString(ud.config.scripthandle, "Screen Setup", "AmbientLight",&tempbuf[0]);
            if (atof(tempbuf)) {
                r_ambientlight = atof(tempbuf);
                r_ambientlightrecip = 1.f/r_ambientlight;
            }
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "Detail",&ud.detail);

        {
            extern int32_t g_frameDelay;

            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "MaxFPS",&r_maxfps);
            r_maxfps = max(0,min(1000,r_maxfps));

            if (r_maxfps)
                g_frameDelay = (1000/r_maxfps);
            else g_frameDelay = 0;
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "Messages",&ud.fta_on);

        if (!NAM) {
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "Out",&ud.lockout);
            SCRIPT_GetString(ud.config.scripthandle, "Screen Setup","Password",&ud.pwlockout[0]);
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "ScreenGamma",&ud.brightness);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "ScreenHeight",&ud.config.ScreenHeight);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "ScreenMode",&ud.config.ScreenMode);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "ScreenSize",&ud.screen_size);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "ScreenWidth",&ud.config.ScreenWidth);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "Shadows",&ud.shadows);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "Tilt",&ud.screen_tilting);

        {
            tempbuf[0] = 0;
            SCRIPT_GetString(ud.config.scripthandle, "Screen Setup", "VidGamma",&tempbuf[0]);
            if (tempbuf[0]) vid_gamma = atof(tempbuf);
        }

        {
            tempbuf[0] = 0;
            SCRIPT_GetString(ud.config.scripthandle, "Screen Setup", "VidBrightness",&tempbuf[0]);
            if (tempbuf[0]) vid_brightness = atof(tempbuf);
        }

        {
            tempbuf[0] = 0;
            SCRIPT_GetString(ud.config.scripthandle, "Screen Setup", "VidContrast",&tempbuf[0]);
            if (tempbuf[0]) vid_contrast = atof(tempbuf);
        }

#ifdef RENDERTYPEWIN
        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "WindowPositioning", (int32_t *)&windowpos);
            windowx = -1;
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "WindowPosX", (int32_t *)&windowx);
            windowy = -1;
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "WindowPosY", (int32_t *)&windowy);
        }
#endif

        {
#ifdef _WIN32
            dummy = 0;
#else
            dummy = 1;
#endif
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "OSDTextMode",&dummy);
            OSD_SetTextMode(dummy);
        }

#ifdef RENDERTYPEWIN
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "MaxRefreshFreq", (int32_t*)&maxrefreshfreq);
#endif

#if defined(POLYMOST) && defined(USE_OPENGL)
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLAnimationSmoothing", &r_animsmoothing);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLAnisotropy", &glanisotropy);
        /*SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLDepthPeeling", &r_depthpeeling);*/
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLDetailMapping", &r_detailmapping);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLFullbrights", &r_fullbrights);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLGlowMapping", &r_glowmapping);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLOcclusionChecking", &r_modelocclusionchecking);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLParallaxSkyClamping", &r_parallaxskyclamping);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLParallaxSkyPanning", &r_parallaxskypanning);
        /*SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLPeelsCount", &r_peelscount);*/
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLProjectionFix", &glprojectionhacks);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "Polymer", &dummy);
        if (dummy > 0) glrendmode = 4;
        else glrendmode = 3;
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLTextureMode", &gltexfiltermode);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLTextureQuality", &r_downsize);
        r_downsizevar = r_downsize;
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLUseCompressedTextureCache", &glusetexcache);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLUseTextureCacheCompression", &glusetexcachecompression);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLUseTextureCompr", &glusetexcompr);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLVBOCount", &r_vbocount);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLVBOs", &r_vbos);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLVertexArrays", &r_vertexarrays);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLVSync", &vsync);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "GLWidescreen", &glwidescreen);
        SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "OSDHightile",&osdhightile);

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "ScreenBPP", &ud.config.ScreenBPP);
            if (ud.config.ScreenBPP < 8) ud.config.ScreenBPP = 32;
        }

        {
            dummy = usehightile;
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "UseHightile",&dummy);
            usehightile = dummy != 0;
        }

        {
            dummy = usemodels;
            SCRIPT_GetNumber(ud.config.scripthandle, "Screen Setup", "UseModels",&dummy);
            usemodels = dummy != 0;
        }
#endif

        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "AltHud",&ud.althud);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc","AngleInterpolation",&ud.angleinterpolation);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "AutoMsg",&ud.automsg);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "AutoVote",&ud.autovote);

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "Color",&ud.color);
            G_CheckPlayerColor((int32_t *)&ud.color,-1);
            g_player[0].ps->palookup = g_player[0].pcolor = ud.color;
        }

        {
            tempbuf[0] = 0;
            SCRIPT_GetString(ud.config.scripthandle, "Misc", "CrosshairColor",&tempbuf[0]);
            if (tempbuf[0]) {
                char *ptr = strtok(tempbuf,",");
                palette_t temppal;
                char *palptr = (char *)&temppal;

                i = 0;
                while (ptr != NULL && i < 3) {
                    palptr[i++] = atoi(ptr);
                    ptr = strtok(NULL,",");
                }
                if (i == 3) {
                    Bmemcpy(&CrosshairColors,&temppal,sizeof(palette_t));
                    DefaultCrosshairColors.f = 1;
                }
            }
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "CrosshairScale",&ud.crosshairscale);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "Crosshairs",&ud.crosshair);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "DeathMessages",&ud.obituaries);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "DemoCams",&ud.democams);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "Executions",&ud.executions);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "IDPlayers",&ud.idplayers);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "MPMessageDisplayTime",&ud.msgdisptime);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "RunMode",&ud.config.RunMode);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "ShowFPS",&ud.tickrate);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "ShowLevelStats",&ud.levelstats);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "ShowOpponentWeapons",&ud.config.ShowOpponentWeapons);
        ud.showweapons = ud.config.ShowOpponentWeapons;
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "ShowViewWeapon",&ud.drawweapon);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "StatusBarMode",&ud.statusbarmode);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "StatusBarScale",&ud.statusbarscale);

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "Team",&dummy);
            ud.team = 0;
            if (dummy < 4 && dummy > -1) ud.team = dummy;
            g_player[0].pteam = ud.team;
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "TextScale",&ud.textscale);
        dummy = ud.config.useprecache;
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "UsePrecache",&dummy);
        ud.config.useprecache = dummy != 0;
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "ViewBobbing",&ud.viewbob);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "WeaponScale",&ud.weaponscale);
        SCRIPT_GetNumber(ud.config.scripthandle, "Misc", "WeaponSway",&ud.weaponsway);

        // weapon choices are defaulted in G_CheckCommandLine, which may override them
        if (!CommandWeaponChoice)
            for (i=0; i<10; i++) {
                Bsprintf(buf,"WeaponChoice%d",i);
                dummy = -1;
                SCRIPT_GetNumber(ud.config.scripthandle, "Misc", buf, &dummy);
                if (dummy >= 0) g_player[0].wchoice[i] = dummy;
            }

        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "AmbienceToggle",&ud.config.AmbienceToggle);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "FXDevice",&ud.config.FXDevice);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "FXVolume",&ud.config.FXVolume);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "MixRate",&ud.config.MixRate);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "MusicDevice",&ud.config.MusicDevice);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "MusicToggle",&ud.config.MusicToggle);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "MusicVolume",&ud.config.MusicVolume);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "NumBits",&ud.config.NumBits);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "NumChannels",&ud.config.NumChannels);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "NumVoices",&ud.config.NumVoices);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "ReverseStereo",&ud.config.ReverseStereo);
        SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "SoundToggle",&ud.config.SoundToggle);

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Sound Setup", "VoiceToggle",&ud.config.VoiceToggle);
            // hack to switch old VoiceToggle value over to new bitfield format
            if (ud.config.VoiceToggle == 2) ud.config.VoiceToggle = 5;
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","AimingFlag",(int32_t *)&g_myAimMode);    // (if toggle mode) gives state

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Controls","AutoAim",&ud.config.AutoAim);          // JBF 20031125
            g_player[0].ps->auto_aim = ud.config.AutoAim;
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","MouseAimingFlipped",&ud.mouseflip);  // mouse aiming inverted

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Controls","MouseAiming",&ud.mouseaiming);		// 1=momentary/0=toggle
            g_player[0].ps->aim_mode = ud.mouseaiming;
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","MouseBias",&ud.config.MouseBias);
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","MouseDeadZone",&ud.config.MouseDeadZone);
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","RunKeyBehaviour",&ud.runkey_mode);   // JBF 20031125

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Controls","SmoothInput",&ud.config.SmoothInput);
            control_smoothmouse = ud.config.SmoothInput;
        }

        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","UseJoystick",&ud.config.UseJoystick);
        SCRIPT_GetNumber(ud.config.scripthandle, "Controls","UseMouse",&ud.config.UseMouse);

        {
            SCRIPT_GetNumber(ud.config.scripthandle, "Controls","WeaponSwitchMode",&ud.weaponswitch);
            g_player[0].ps->weaponswitch = ud.weaponswitch;
        }

#ifdef _WIN32
        SCRIPT_GetNumber(ud.config.scripthandle, "Updates", "CheckForUpdates", &ud.config.CheckForUpdates);
        SCRIPT_GetNumber(ud.config.scripthandle, "Updates", "LastUpdateCheck", &ud.config.LastUpdateCheck);
#endif

    }

    CONFIG_ReadKeys();

    //CONFIG_SetupMouse(ud.config.scripthandle);
    //CONFIG_SetupJoystick(ud.config.scripthandle);
    ud.config.setupread = 1;
    return 0;
}

/*
===================
=
= CONFIG_WriteSetup
=
===================
*/

void CONFIG_WriteBinds(void) { // save binds and aliases to <cfgname>_binds.cfg
    int32_t i;
    FILE *fp;
    char *ptr = Bstrdup(setupfilename);
    char tempbuf[128];

    Bsprintf(tempbuf, "%s_binds.cfg", strtok(ptr, "."));
    fp = fopen(tempbuf, "wt");

    if (fp) {
        symbol_t *symb;

        fprintf(fp,"// this file automatically generated by eRampage\n");
        fprintf(fp,"// these settings take precedence over your main cfg file\n");
        fprintf(fp,"// do not modify if you lack the sense of beauty =)\n");

        for (i=0; i<MAXBOUNDKEYS; i++)
            if (KeyBindings[i].cmd[0] && KeyBindings[i].key)
                fprintf(fp,"bind \"%s\"%s \"%s\"\n",KeyBindings[i].key,KeyBindings[i].repeat?"":" norepeat",KeyBindings[i].cmd);

        for (i=0; i<MAXMOUSEBUTTONS; i++)
            if (MouseBindings[i].cmd[0])
                fprintf(fp,"bind \"%s\"%s \"%s\"\n",MouseBindings[i].key,MouseBindings[i].repeat?"":" norepeat",MouseBindings[i].cmd);

        for (symb=symbols; symb!=NULL; symb=symb->next)
            if (symb->func == (void *)OSD_ALIAS)
                fprintf(fp,"alias \"%s\" \"%s\"\n", symb->name, symb->help);

        /*        for (i = 0; i < sizeof(cvar)/sizeof(cvarmappings); i++)
                    if (!(cvar[i].type&CVAR_NOSAVE))
                        fprintf(fp,"%s \"%d\"\n",cvar[i].name,*(int32_t*)cvar[i].var);
                        */
        OSD_WriteCvars(fp);
        fclose(fp);
        Bsprintf(tempbuf,"Wrote %s_binds.cfg\n",ptr);
        OSD_Printf(tempbuf);
        Bfree(ptr);
        return;
    }
    Bsprintf(tempbuf,"Error writing %s_binds.cfg: %s\n",ptr,strerror(errno));
    OSD_Printf(tempbuf);
    Bfree(ptr);
}

void CONFIG_WriteSetup(void) {
    int32_t dummy;
    char tempbuf[1024];

    if (!ud.config.setupread) return;

    if (ud.config.scripthandle < 0)
        ud.config.scripthandle = SCRIPT_Init(setupfilename);

    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","AimingFlag",(int32_t) g_myAimMode,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","AutoAim",ud.config.AutoAim,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","MouseAimingFlipped",ud.mouseflip,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","MouseAiming",ud.mouseaiming,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","MouseBias",ud.config.MouseBias,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","MouseDeadZone",ud.config.MouseDeadZone,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","RunKeyBehaviour",ud.runkey_mode,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","SmoothInput",ud.config.SmoothInput,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","UseJoystick",ud.config.UseJoystick,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","UseMouse",ud.config.UseMouse,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Controls","WeaponSwitchMode",ud.weaponswitch,FALSE,FALSE);

    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "AltHud",ud.althud,FALSE,FALSE);
//    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "AngleInterpolation",ud.angleinterpolation,false,false);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "AutoMsg",ud.automsg,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "AutoVote",ud.autovote,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "Color",ud.color,FALSE,FALSE);

    if (CrosshairColors.r != DefaultCrosshairColors.r || CrosshairColors.g != DefaultCrosshairColors.g
            || CrosshairColors.b != DefaultCrosshairColors.b) {
        Bsprintf(tempbuf,"%d,%d,%d",CrosshairColors.r,CrosshairColors.g,CrosshairColors.b);
        SCRIPT_PutString(ud.config.scripthandle, "Misc", "CrosshairColor",tempbuf);
    }

    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "CrosshairScale",ud.crosshairscale,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "Crosshairs",ud.crosshair,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "DeathMessages",ud.obituaries,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "DemoCams",ud.democams,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "Executions",++ud.executions,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "IDPlayers",ud.idplayers,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "MPMessageDisplayTime",ud.msgdisptime,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "RunMode",ud.config.RunMode,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "ShowFPS",ud.tickrate,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "ShowLevelStats",ud.levelstats,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "ShowOpponentWeapons",ud.config.ShowOpponentWeapons,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "ShowViewWeapon",ud.drawweapon,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "StatusBarMode",ud.statusbarmode,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "StatusBarScale",ud.statusbarscale,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "Team",ud.team,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "TextScale",ud.textscale,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "UsePrecache",ud.config.useprecache,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "ViewBobbing",ud.viewbob,FALSE,FALSE);

    for (dummy=0; dummy<10; dummy++) {
        Bsprintf(buf,"WeaponChoice%d",dummy);
        SCRIPT_PutNumber(ud.config.scripthandle, "Misc",buf,g_player[myconnectindex].wchoice[dummy],FALSE,FALSE);
    }

    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "WeaponScale",ud.weaponscale,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Misc", "WeaponSway",ud.weaponsway,FALSE,FALSE);

    SCRIPT_PutNumber(ud.config.scripthandle, "Setup","ConfigVersion",BYTEVERSION_JF,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Setup", "ForceSetup",ud.config.ForceSetup,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Setup", "NoAutoLoad",ud.config.NoAutoLoad,FALSE,FALSE);

    {
        Bsprintf(tempbuf,"%.2f",r_ambientlight);
        SCRIPT_PutString(ud.config.scripthandle, "Screen Setup", "AmbientLight",tempbuf);
    }

    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "Detail",ud.detail,FALSE,FALSE);
#if defined(POLYMOST) && defined(USE_OPENGL)
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLAnimationSmoothing",r_animsmoothing,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLAnisotropy",glanisotropy,FALSE,FALSE);
    /*SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLDepthPeeling",r_depthpeeling,false,false);*/
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLDetailMapping", r_detailmapping,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLFullbrights", r_fullbrights,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLGlowMapping", r_glowmapping,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLOcclusionChecking", r_modelocclusionchecking,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLParallaxSkyClamping",r_parallaxskyclamping,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLParallaxSkyPanning",r_parallaxskypanning,FALSE,FALSE);
    /*SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLPeelsCount",r_peelscount,false,false);*/
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLProjectionFix",glprojectionhacks,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "Polymer",glrendmode == 4,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLTextureMode",gltexfiltermode,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLTextureQuality", r_downsize,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLUseCompressedTextureCache", glusetexcache,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLUseTextureCacheCompression", glusetexcachecompression,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLUseTextureCompr",glusetexcompr,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLVBOCount", r_vbocount,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLVBOs", r_vbos,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLVertexArrays", r_vertexarrays,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLVSync", vsync,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "GLWidescreen",glwidescreen,FALSE,FALSE);
#endif
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "MaxFPS", r_maxfps, FALSE, FALSE);
#ifdef RENDERTYPEWIN
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "MaxRefreshFreq",maxrefreshfreq,FALSE,FALSE);
#endif
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "Messages",ud.fta_on,FALSE,FALSE);

    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "OSDHightile",osdhightile,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "OSDTextMode",OSD_GetTextMode(),FALSE,FALSE);

    if (!NAM) {
        SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "Out",ud.lockout,FALSE,FALSE);
        SCRIPT_PutString(ud.config.scripthandle, "Screen Setup", "Password",ud.pwlockout);
    }

    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "ScreenBPP",ud.config.ScreenBPP,FALSE,FALSE);  // JBF 20040523
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "ScreenGamma",ud.brightness,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "ScreenHeight",ud.config.ScreenHeight,FALSE,FALSE);    // JBF 20031206
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "ScreenMode",ud.config.ScreenMode,FALSE,FALSE);    // JBF 20031206
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "ScreenSize",ud.screen_size,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "ScreenWidth",ud.config.ScreenWidth,FALSE,FALSE);  // JBF 20031206
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "Shadows",ud.shadows,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "Tilt",ud.screen_tilting,FALSE,FALSE);

#if defined(POLYMOST) && defined(USE_OPENGL)
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "UseHightile",usehightile,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "UseModels",usemodels,FALSE,FALSE);
#endif

    {
        Bsprintf(tempbuf,"%.2f",vid_brightness);
        SCRIPT_PutString(ud.config.scripthandle, "Screen Setup", "VidBrightness",tempbuf);
        Bsprintf(tempbuf,"%.2f",vid_contrast);
        SCRIPT_PutString(ud.config.scripthandle, "Screen Setup", "VidContrast",tempbuf);
        Bsprintf(tempbuf,"%.2f",vid_gamma);
        SCRIPT_PutString(ud.config.scripthandle, "Screen Setup", "VidGamma",tempbuf);
    }

#ifdef RENDERTYPEWIN
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "WindowPositioning", windowpos, FALSE, FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "WindowPosX", windowx, FALSE, FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Screen Setup", "WindowPosY", windowy, FALSE, FALSE);
#endif

    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "AmbienceToggle",ud.config.AmbienceToggle,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "FXVolume",ud.config.FXVolume,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "MusicToggle",ud.config.MusicToggle,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "MusicVolume",ud.config.MusicVolume,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "NumVoices",ud.config.NumVoices,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "NumChannels",ud.config.NumChannels,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "NumBits",ud.config.NumBits,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "MixRate",ud.config.MixRate,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "ReverseStereo",ud.config.ReverseStereo,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "SoundToggle",ud.config.SoundToggle,FALSE,FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Sound Setup", "VoiceToggle",ud.config.VoiceToggle,FALSE,FALSE);

#ifdef _WIN32
    SCRIPT_PutNumber(ud.config.scripthandle, "Updates", "CheckForUpdates", ud.config.CheckForUpdates, FALSE, FALSE);
    SCRIPT_PutNumber(ud.config.scripthandle, "Updates", "LastUpdateCheck", ud.config.LastUpdateCheck, FALSE, FALSE);
#endif

    // JBF 20031211
    for (dummy=0; dummy<NUMGAMEFUNCTIONS; dummy++) {
        SCRIPT_PutDoubleString(ud.config.scripthandle, "KeyDefinitions", CONFIG_FunctionNumToName(dummy),
                               KB_ScanCodeToString(ud.config.KeyboardKeys[dummy][0]), KB_ScanCodeToString(ud.config.KeyboardKeys[dummy][1]));
    }

    for (dummy=0; dummy<MAXMOUSEBUTTONS; dummy++) {
        Bsprintf(buf,"MouseButton%d",dummy);
        SCRIPT_PutString(ud.config.scripthandle,"Controls", buf, CONFIG_FunctionNumToName(ud.config.MouseFunctions[dummy][0]));

        if (dummy >= (MAXMOUSEBUTTONS-2)) continue;

        Bsprintf(buf,"MouseButtonClicked%d",dummy);
        SCRIPT_PutString(ud.config.scripthandle,"Controls", buf, CONFIG_FunctionNumToName(ud.config.MouseFunctions[dummy][1]));
    }

    for (dummy=0; dummy<MAXMOUSEAXES; dummy++) {
        Bsprintf(buf,"MouseAnalogAxes%d",dummy);
        SCRIPT_PutString(ud.config.scripthandle, "Controls", buf, CONFIG_AnalogNumToName(ud.config.MouseAnalogueAxes[dummy]));

        Bsprintf(buf,"MouseDigitalAxes%d_0",dummy);
        SCRIPT_PutString(ud.config.scripthandle, "Controls", buf, CONFIG_FunctionNumToName(ud.config.MouseDigitalFunctions[dummy][0]));

        Bsprintf(buf,"MouseDigitalAxes%d_1",dummy);
        SCRIPT_PutString(ud.config.scripthandle, "Controls", buf, CONFIG_FunctionNumToName(ud.config.MouseDigitalFunctions[dummy][1]));

        Bsprintf(buf,"MouseAnalogScale%d",dummy);
        SCRIPT_PutNumber(ud.config.scripthandle, "Controls", buf, ud.config.MouseAnalogueScale[dummy], FALSE, FALSE);
    }

    Bsprintf(tempbuf,"%.2f",CONTROL_MouseSensitivity);
    SCRIPT_PutString(ud.config.scripthandle,  "Controls","Mouse_Sensitivity",tempbuf);

    for (dummy=0; dummy<MAXJOYBUTTONS; dummy++) {
        Bsprintf(buf,"JoystickButton%d",dummy);
        SCRIPT_PutString(ud.config.scripthandle,"Controls", buf, CONFIG_FunctionNumToName(ud.config.JoystickFunctions[dummy][0]));

        Bsprintf(buf,"JoystickButtonClicked%d",dummy);
        SCRIPT_PutString(ud.config.scripthandle,"Controls", buf, CONFIG_FunctionNumToName(ud.config.JoystickFunctions[dummy][1]));
    }
    for (dummy=0; dummy<MAXJOYAXES; dummy++) {
        Bsprintf(buf,"JoystickAnalogAxes%d",dummy);
        SCRIPT_PutString(ud.config.scripthandle, "Controls", buf, CONFIG_AnalogNumToName(ud.config.JoystickAnalogueAxes[dummy]));

        Bsprintf(buf,"JoystickDigitalAxes%d_0",dummy);
        SCRIPT_PutString(ud.config.scripthandle, "Controls", buf, CONFIG_FunctionNumToName(ud.config.JoystickDigitalFunctions[dummy][0]));

        Bsprintf(buf,"JoystickDigitalAxes%d_1",dummy);
        SCRIPT_PutString(ud.config.scripthandle, "Controls", buf, CONFIG_FunctionNumToName(ud.config.JoystickDigitalFunctions[dummy][1]));

        Bsprintf(buf,"JoystickAnalogScale%d",dummy);
        SCRIPT_PutNumber(ud.config.scripthandle, "Controls", buf, ud.config.JoystickAnalogueScale[dummy], FALSE, FALSE);

        Bsprintf(buf,"JoystickAnalogDead%d",dummy);
        SCRIPT_PutNumber(ud.config.scripthandle, "Controls", buf, ud.config.JoystickAnalogueDead[dummy], FALSE, FALSE);

        Bsprintf(buf,"JoystickAnalogSaturate%d",dummy);
        SCRIPT_PutNumber(ud.config.scripthandle, "Controls", buf, ud.config.JoystickAnalogueSaturate[dummy], FALSE, FALSE);
    }

    SCRIPT_PutString(ud.config.scripthandle, "Comm Setup","PlayerName",&szPlayerName[0]);
    SCRIPT_PutString(ud.config.scripthandle, "Comm Setup","RTSName",&ud.rtsname[0]);

#ifndef RANCID_NETWORKING
    // The packetrate mechanism is specific to the eduke32 networking code
    SCRIPT_PutNumber(ud.config.scripthandle, "Comm Setup", "Rate", packetrate, FALSE, FALSE);
#endif


    SCRIPT_PutString(ud.config.scripthandle, "Setup","SelectedGRP",&duke3dgrp[0]);

#ifdef _WIN32
    if (g_noSetup == 0)
        SCRIPT_PutString(ud.config.scripthandle, "Setup","ModDir",&mod_dir[0]);
#endif

    {
        char commmacro[] = "CommbatMacro# ";

        for (dummy = 0; dummy < 10; dummy++) {
            commmacro[13] = dummy+'0';
            SCRIPT_PutString(ud.config.scripthandle, "Comm Setup",commmacro,&ud.ridecule[dummy][0]);
        }
    }

    SCRIPT_Save(ud.config.scripthandle, setupfilename);
    SCRIPT_Free(ud.config.scripthandle);
    OSD_Printf("Wrote %s\n",setupfilename);
    CONFIG_WriteBinds();
}


int32_t CONFIG_GetMapBestTime(char *mapname) {
    int32_t t = -1;
    char m[BMAX_PATH], *p;

    strcpy(m, mapname);
    p = strrchr(m, '/');
    if (!p) p = strrchr(m, '\\');
    if (p) strcpy(m, p);
    for (p=m; *p; p++) *p = tolower(*p);

    // cheap hack because SCRIPT_GetNumber doesn't like the slashes
    p = m;
    while (*p == '/') p++;

    if (!ud.config.setupread) return -1;
    if (ud.config.scripthandle < 0) return -1;

    SCRIPT_GetNumber(ud.config.scripthandle, "MapTimes", p, &t);
    return t;
}

int32_t CONFIG_SetMapBestTime(char *mapname, int32_t tm) {
    char m[BMAX_PATH], *p;

    strcpy(m, mapname);
    p = strrchr(m, '/');
    if (!p) p = strrchr(m, '\\');
    if (p) strcpy(m, p);
    for (p=m; *p; p++) *p = tolower(*p);

    // cheap hack because SCRIPT_GetNumber doesn't like the slashes
    p = m;
    while (*p == '/') p++;

    if (ud.config.scripthandle < 0) ud.config.scripthandle = SCRIPT_Init(setupfilename);
    if (ud.config.scripthandle < 0) return -1;

    SCRIPT_PutNumber(ud.config.scripthandle, "MapTimes", p, tm, FALSE, FALSE);
    return 0;
}

/*
 * vim:ts=4:sw=4:
 */

