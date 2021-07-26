System_file;

#Ifndef VORPLE_HOWLER;
Constant VORPLE_HOWLER;

!==============================
! Vorple Howler
!    a wrapper for howler.js in Vorple
!    ported from Nathanaël Marion's I7 library

!  This extension provides hooks to represent Howl objects in Inform 6,
!    set their characteristics, and update the corresponding Howl objects
!    in the JavaScript code. It is directly inspired by Natrium729's
!    Inform 7 extension that provides the same functionality.
!  
!  This extension defines the Howl class with the following properties:
!    -- identifier, a string
!    -- source, a string or a routine outputting a string (for more flexibility)
!    -- initial_volume, an integer between 0 and 100
!    -- initial_rate (to slow down or speed up), 100 by default
!    -- onloaderror, onplayerror, onplay, onend,  onpause, onstop,  onmute,
!       onvolume, onrate, onseek, onfade, onunlock, which can be strings or
!       routines returning a string, that contain JavaScript code that needs to
!       be executed by these hooks, that run after every event of the corresponding
!       type (onfade runs every time a sound gets faded in or out, etc.).
!  Additionally, you get three attributes:
!    -- looping, for a sound that loops;
!    -- muted, for a sound that is muted;
!    -- HTML5 (to be honest, I don't know what this does).
!  
!  An example for an atmospheric noise would be:
!  Howl dungeonBgNoise
!    with identifier "DUNGEON_BG_NOISE",
!         source "dungeon.mp3",
!         initial_volume 60,
!  has looping;
!  
!  which you then initialise (for instance right after initialising Vorple)
!  with HowlerInitialiseHowl(dungeonBgNoise);.
!  
!  A few global methods are available, all pretty self-explanatory:
!  HowlerMuteAllSounds, HowlerUnmuteAllSounds, HowlerUnloadAllSounds,
!  HowlerSetGlobalVolume(n), and finally HowlerGlobalVolume, which returns
!  an integer between 0 and 100.
!  
!  Methods to operate on howls are as follows:
!    -- HowlerPlayHowl(yourHowl) plays the howl, interrupting all sounds
!       currently playing; HowlerPlayHowl(yourHowl, LEAVE_CURRENT_PLAYBACKS)
!       doesn't interrupt.
!    -- HowlerPauseHowl(yourHowl), and HowlerUnpauseHowl(yourHowl)
!    -- HowlerStopHowl(yourHowl) and HowlerUnloadHowl(yourHowl)
!    -- HowlerMuteHowl(yourHowl) and HowlerUnmuteHowl(yourHowl)
!    -- HowlerLoopHowl(yourHowl) and HowlerUnloopHowl(yourHowl)
!    -- HowlerSetHowlVolume(yourHowl, n), with n between 0 and 100
!    -- HowlerSetHowlRate(yourHowl, n)
!    -- HowlerFadeHowlBetween(yourHowl, n1, n2, t) will create a fade effect
!       going from volume n1 to volume n2 in t milliseconds ; and
!       HowlerFadeHowlIn(yourHowl, t) and HowlFadeHowlOut(yourHowl, t). Note
!       that for all 3 functions, the howl has to already be playing; calling
!       HowlerPlayHowl right before calling these should work.
!    -- HowlerSetHowlPosition(yourHowl, n1, n2) : skip to the "n1.n2" timecode
!       in your sound.
!  
!  Finally, a few functions are available to check on the status of howls:
!    -- HowlerIsHowlPlaying(yourHowl)
!    -- HowlerGetHowlState(yourHowl) - can either be the constant HOWL_LOADED,
!       HOWL_LOADING, or HOWL_UNLOADED.
!    -- HowlerDoesHowlLoop(yourHowl)
!    -- HowlerGetHowlRate(yourHowl) and HowlerGetHowlVolume(yourHowl)
!    -- HowlerGetHowlPosition(yourHowl) and HowlerGetHowlDuration(yourHowl).
!       Note that for these two, since Howler returns a floating point number,
!       the result is stored as two integers in an array, namely current_position
!       and howl_duration.


!===========
! Inits
Object VorpleSetupHowlerRule "" VorpleInterfaceSetup
    with description [; 
        VorpleExecuteJavaScriptCommand("window.howls = {};");
        VorpleExecuteJavaScriptCommand("Howler.mute(false).volume(1).unload(); howls = {};");
    ];	


!===========
! General Functions

[ HowlerMuteAllSounds;
    VorpleExecuteJavaScriptCommand("Howler.mute(true);");
];

[ HowlerUnmuteAllSounds;
    VorpleExecuteJavaScriptCommand("Howler.mute(false);");
];

[ HowlerUnloadAllSounds;
    VorpleExecuteJavaScriptCommand("Howler.unload(); howls = {};");
];

[ HowlerSetGlobalVolume n;
    VorpleExecuteJavaScriptCommand(BuildCommand("Howler.volume(", IntToString(n), " / 100);"));
];

[ HowlerGlobalVolume ;
    VorpleExecuteJavaScriptCommand("return Howler.volume() * 100;");
    return VorpleWhatNumberWasReturned();
];


!===========
! Howler object (howl)

Constant HOWL_UNLOADED 0;
Constant HOWL_LOADING 1;
Constant HOWL_LOADED 2;

Attribute HTML5;
Attribute looping;
Attribute muted;

Class Howl
with    source 0,                   ! this can be a string or a routine (to support dynamic/proceduralish code)
        initial_volume 100,         ! must be a number
        initial_rate 100,           ! must be a number
        identifier "REPLACE_ME",    ! must be a string
        onload 0,                   ! all of these can be strings or routines that return some javascript code
        onloaderror 0,
        onplayerror 0,
        onplay 0,
        onend 0,
        onpause 0,
        onstop 0,
        onmute 0,
        onvolume 0,
        onrate 0,
        onseek 0,
        onfade 0,
        onunlock 0,
has;

[ HowlerIsThatHowlInitialised o ;
    VorpleExecuteJavaScriptCommand(BuildCommand("return howls.hasOwnProperty('", o.identifier, "');"));
    if (VorpleWhatBooleanWasReturned()) {rtrue;} else {rfalse;}
];

Constant DONT_AUTOPLAY 0; ! not necessary but for consistency
Constant AUTOPLAY_HOWL 1;

! we use hugehugestr here, so careful about the side-effects to BuildCommand (there shouldn't be any)
! TODO: we're basically reimplementing BuildCommand, but should we be implementing ConcatenateUnicodeStrings (i.e. unicode conversion too) here?
[ HowlerInitialiseHowl o autop ;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (string) o.identifier; print " = new Howl({";
        print "src: '"; PrintOrRun(o,source,1); print "',";
        if (o has HTML5) { print "html5: true,";}
        if (o has looping) { print "loop: true,";}
        if (autop == 0) { print "autoplay: false,"; } else { print "autopay: true,";}
        if (o has muted) { print "muted: true,";}
        print "rate: ", o.initial_rate, " /100,";
        if (o.onload ~=0) { print "onload: function() {"; PrintOrRun(o,onload,1); print "},";}
        print "onloaderror: function(id, err) {console.log(err); "; if (o.onloaderror ~= 0) {PrintOrRun(o,onloaderror,1);} print "},";
        print "onplayerror: function(id, err) {console.log(err); "; if (o.onplayerror ~= 0) {PrintOrRun(o,onplayerror,1);} print "},";
        if (o.onplay ~=0) { print "onplay: function(id) {"; PrintOrRun(o,onplay,1); print "},";}
        if (o.onend ~=0) { print "onend: function(id) {"; PrintOrRun(o,onend,1); print "},";}
        if (o.onpause ~=0) { print "onpause: function(id) {"; PrintOrRun(o,onpause,1); print "},";}
        if (o.onstop ~=0) { print "onstop: function(id) {"; PrintOrRun(o,onstop,1); print "},";}
        if (o.onmute ~=0) { print "onmute: function(id) {"; PrintOrRun(o,onmute,1); print "},";}
        if (o.onvolume ~=0) { print "onvolume: function(id) {"; PrintOrRun(o,onvolume,1); print "},";}
        if (o.onrate ~=0) { print "onrate: function(id) {"; PrintOrRun(o,onrate,1); print "},";}
        if (o.onseek ~=0) { print "onseek: function(id) {"; PrintOrRun(o,onseek,1); print "},";}
        if (o.onfade ~=0) { print "onfade: function(id) {"; PrintOrRun(o,onfade,1); print "},";}
        if (o.onunlock ~=0) { print "onunlock: function(id) {"; PrintOrRun(o,onunlock,1); print "},";}
        print "});";
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
!	PrintStringOrArray(hugehugestr); print "^";
	VorpleExecuteJavaScriptCommand(hugehugestr);
    }
];








!=================
! Operating on howls

[ HowlerPreloadHowl o;
    HowlerInitialiseHowl(o);
];

Constant LEAVE_CURRENT_PLAYBACKS 1;
[ HowlerPlayHowl o playbacks ;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        HowlerInitialiseHowl(o, AUTOPLAY_HOWL);
    } else {
        if (playbacks == LEAVE_CURRENT_PLAYBACKS) {
            VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".play();"));
        } else {
            VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".stop().play();"));
        }
    }
];

[ HowlerPauseHowl o ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".pause();"));
    }
];
[ HowlerUnpauseHowl o ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".play();"));
    }
];

[ HowlerStopHowl o ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".stop();"));
    }
];

[ HowlerMuteHowl o ;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        give o muted;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".mute(true);"));
    }
];
[ HowlerUnmuteHowl o ;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        give o muted;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".mute(false);"));
    }
];

[ HowlerSetHowlVolume o n;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        o.initial_volume = n;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".volume(", IntToString(n), " / 100);"));
    }
];

! fade o from volume n1% to volume n2% in t milliseconds
! the sound o has to already be playing (HowlerPlayHowl then this function immediately after)
[ HowlerFadeHowlBetween o n1 n2 t;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! need to replicate buildcommand because we can't use inttostring twice in a call to it (otherwise it's the same array that's used twice)
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".fade(";
        PrintStringOrArray(IntToString(n1));
        print " / 100, ";
        PrintStringOrArray(IntToString(n2));
        print " / 100, ";
        PrintStringOrArray(IntToString(t));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        VorpleExecuteJavaScriptCommand(hugehugestr);
        !PrintStringOrArray(hugehugestr);
    }
];
! fade o out, in t milliseconds
! the sound o has to already be playing (HowlerPlayHowl then this function immediately after)
[ HowlerFadeOutHowl o t;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! need to replicate buildcommand because we can't use VorpleEscape twice in a call to it (otherwise it's the same array that's used twice)
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".fade(";
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".volume(), 0, ";
        PrintStringOrArray(IntToString(t));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        VorpleExecuteJavaScriptCommand(hugehugestr);
    } else { o.initial_volume = 0; }
];
! fade o in, in t milliseconds
! the sound o has to already be playing (HowlerPlayHowl then this function immediately after), but no need to specify a starting volume of 0 in the howl
[ HowlerFadeInHowl o t ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".fade(0,1,", IntToString(t), ");"));
    } else { o.initial_volume = 100; }
];
! fade o in to volume n1, in t milliseconds
! the sound o has to already be playing (HowlerPlayHowl then this function immediately after)
[ HowlerFadeHowlTo o n1 t ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! need to replicate buildcommand because we can't use VorpleEscape twice in a call to it (otherwise it's the same array that's used twice)
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray)VorpleEscape(o.identifier), ".fade(";
        print "howls.", (PrintStringOrArray)VorpleEscape(o.identifier), ".volume(), ";
        PrintStringOrArray(IntToString(n1)); print " / 100, ";
        PrintStringOrArray(IntToString(t));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        VorpleExecuteJavaScriptCommand(hugehugestr);
    }
];

[ HowlerSetRate o n;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".rate(", IntToString(n), " / 100);"));
    } else { o.initial_rate = n; }
];

[ HowlerSetHowlPosition o s d ;
    ! s for seconds, d for anything after the decimal point : if s = 12 and d = 215 then set to 12.215s
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! need to replicate buildcommand because we can't use VorpleEscape twice in a call to it (otherwise it's the same array that's used twice)
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".seek(";
        PrintStringOrArray(IntToString(s)); print ".";
        PrintStringOrArray(IntToString(d));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        VorpleExecuteJavaScriptCommand(hugehugestr);
    }
];

[ HowlerLoopHowl o ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".loop(true)"));
    } else { give o looping ; }
];
[ HowlerUnloopHowl o ;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".loop(false)"));
    } else { give o ~looping ; }
];


[ HowlerUnloadHowl o;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! need to replicate buildcommand because we can't use VorpleEscape twice in a call to it (otherwise it's the same array that's used twice)
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".unload(); ";
        print "delete howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ";";
        !@output_stream -3;
        bp_output_stream(-3);
        VorpleExecuteJavaScriptCommand(hugehugestr);
    }
];


!=====================
! Getting the state of howls

[ HowlerGetHowlVolume o;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        return o.initial_volume;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".volume() * 100;"));
        return VorpleWhatNumberWasReturned();
    }
];

[ HowlerGetHowlRate o;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        return o.initial_rate;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".rate() * 100;"));
        return VorpleWhatNumberWasReturned();
    }
];

! Howler's seek gives you seconds and milliseconds
! Inform 6 doesn't support real numbers, and can only go to 32768, which means we can't just multiply by 1000 (maximum of 32 seconds then?)
! So we need an integer for the seconds and another one for the milliseconds
Array current_position --> 2;
[ HowlerGetHowlPosition o;
    ! First clear the array
    current_position-->0 = 0; current_position-->1 = 0;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! Get the seconds
        VorpleExecuteJavaScriptCommand(BuildCommand("return Math.trunc(howls.", VorpleEscape(o.identifier), ".seek())"));
        current_position-->0 = VorpleWhatNumberWasReturned();
        ! Get the milliseconds
        VorpleExecuteJavaScriptCommand(BuildCommand("return (howls.", VorpleEscape(o.identifier), ".seek() % 1) * 1000"));
        current_position-->1 = VorpleWhatNumberWasReturned();
    }
    return current_position;
];

[ HowlerDoesHowlLoop o;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        if (o has looping) { rtrue; } else { rfalse; }
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".loop();"));
        return VorpleWhatBooleanWasReturned();
    }
];

[ HowlerGetHowlState o        tx;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".state();"));
        tx = VorpleWhatTextWasReturned();
        if (compare_string(tx, 6, "loaded")) { return HOWL_LOADED;}
        if (compare_string(tx, 7, "loading")) { return HOWL_LOADING; }
    }
    return HOWL_UNLOADED;
];

[ HowlerIsHowlPlaying o ;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        return 0;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".playing();"));
        return VorpleWhatBooleanWasReturned();
    }
];

! same as the position: we need to return an array
Array howl_duration -->2;
[ HowlerGetHowlDuration o;
    ! clear the array
    howl_duration-->0 = 0; howl_duration-->1 = 0;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! Get the seconds
        VorpleExecuteJavaScriptCommand(BuildCommand("return Math.trunc(howls.", VorpleEscape(o.identifier), ".duration())"));
        howl_duration-->0 = VorpleWhatNumberWasReturned();
        ! Get the milliseconds
        VorpleExecuteJavaScriptCommand(BuildCommand("return (howls.", VorpleEscape(o.identifier), ".duration() % 1) * 1000"));
        howl_duration-->1 = VorpleWhatNumberWasReturned();
    }
    return howl_duration;
];
#Endif; ! VORPLE_HOWLER
