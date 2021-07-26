System_file;

!==============================
! Vorple Howler Spatial
!    a wrapper for the spatial plugin to howler.js in Vorple

!  These functions hook to the part of Howler that deals with panning,
!  stereo, and 3D modelling of a sonic space. These functions are not
!  necessarily needed, so they are spun off in their own file. This file
!  includes Vorple Howler, so you just need to include this file.
!  
!  The class defined in this extension is SpatialHowl, which takes all the
!  Howl properties, and more:
!    -- orientation (three integer coordinates defining the 3D vector the
!       sound is following),
!    -- stereo (integer value between -100 and 100),
!    -- pos (three integer coordinates defining where the source of the sound
!       is placed)
!    -- onpos, onstereo, onorientation (javascript hooks to be executed every
!       time these changes)
!    -- pannerattr (an object of type PannerNode; as far as I understand it,
!       defining a Panner Node object and connecting spatial howls to it will
!       allow you to have these howls sound like they are from the same
!       stationary source; but this honestly too technical for me. If someone
!       could explain it to us...)
!  
!  After initializing your SpatialHowl by calling
!  HowlerInitialiseSpatialHowl(mySpatialHowl), here are some methods you can call:
!    -- HowlerSetListenerPos and HowlerSetListenerOrientation : update the
!       player's position and orientation in the 3D space, which will affect
!       how the sounds are heard
!    -- HowlerGetStereo / HowlerSetStereo, etc. - getters and setters for each
!       of the new parameters contained in SpatialHowl. Note that HowlerGetPos
!       will write three integer coordinates in the pos_coordinates array, and
!       HowlerGetOrientation will write in the orientation_coordinates array.


Include "vorple-howler.h";

Class SpatialHowl
! all of this is normal stuff...
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
! this is to deal with spatial stuff
        orientation 1 0 0,          ! must be an array of 3 coordinates x y z
        stereo 0,                   ! between -100 and 100
        pos 0 0 0,                  ! must be an array of 3 coordinates x y z
        pannerattr 0,               ! must be an object of class PannerNode (see below)
        onstereo 0,                 ! all of these can be strings or routines that return some javascript code
        onpos 0,
        onorientation 0,
has;




! TODO: I don't understand what a panner node is, so this + HowlerSetPannerNode is untested for now

Constant DISTANCE_MODEL_LINEAR 0;
Constant DISTANCE_MODEL_INVERSE 1;
Constant DISTANCE_MODEL_EXPONENTIAL 2;

Constant PANNING_MODEL_HRTF 0;
Constant PANNING_MODEL_EQUALPOWER 1;

Class PannerNode
with    coneinnerangle 0,           ! must be in degrees, between 0 and 360
        coneouterangle 0,           ! must be in degrees, between 0 and 360
        coneoutergain 0,            ! must be a value between 0 and 100; we'll divide it by 100
        distancemodel DISTANCE_MODEL_INVERSE, ! see https://webaudio.github.io/web-audio-api/#idl-def-DistanceModelType
        maxdistance 10000,          ! number
        refdistance 1,              ! number
        rollofffactor 1,            ! will be divided by 100
        panningmodel PANNING_MODEL_HRTF;


! we use hugehugestr here, so careful about the side-effects to BuildCommand (there shouldn't be any)
! TODO: we're basically reimplementing BuildCommand, but should we be implementing ConcatenateUnicodeStrings (i.e. unicode conversion too) here?
[ HowlerInitialiseSpatialHowl o autop ;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (string) o.identifier; print " = new Howl({";
        print "src: '"; PrintOrRun(o,source,1); print "',";
        if (o has HTML5) { print "html5: true,";}
        if (o has looping) { print "loop: true,";}
        if (autop == 0) { print "autoplay: false,"; } else { print "autoplay: true,";}
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
        ! "spatial howler" specific
        print "orientation: [", o.&orientation-->0, ", ", o.&orientation-->1, ", ", o.&orientation-->2, "],";
        print "stereo: ", o.stereo, " /100,";
        print "pos: [", o.&pos-->0, ", ", o.&pos-->1, ", ", o.&pos-->2, "],";
        if (o.onstereo ~=0) { print "onstereo: function(id) {"; PrintOrRun(o,onstereo,1); print "},";}
        if (o.onpos ~=0) { print "onpos: function(id) {"; PrintOrRun(o,onpos,1); print "},";}
        if (o.onorientation ~=0) { print "onorientation: function(id) {"; PrintOrRun(o,onorientation,1); print "},";}
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



!============================
! Functions


!=====================
! Global methods

! TODO: do we really need to implement the "stereo" global method that shifts everyone's stereo? That might be a headache and is it useful

! This is the player's position; you could change it if they move in the room
[ HowlerSetListenerPos x y z ;
    ! This could be simplified if IntToString supported negative numbers - but we still need to reimplement BuildCommand since it can only take 7 arguments
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "Howler.pos(";
        print (PrintStringOrArray)  MinusSign(x), (PrintStringOrArray) IntToString(Abs(x)), ", ";
        print (PrintStringOrArray)  MinusSign(y), (PrintStringOrArray) IntToString(Abs(y)), ", ";
        print (PrintStringOrArray)  MinusSign(z), (PrintStringOrArray) IntToString(Abs(z));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
        VorpleExecuteJavaScriptCommand(hugehugestr);
    ! VorpleExecuteJavaScriptCommand(BuildCommand("pos(", IntToString(x), ", ", IntToString(y), ", ", IntToString(z), ")"));
];

! This is the player's orientation; you could change it if they look at various things in the room
[ HowlerSetListenerOrientation x y z xup yup zup;
    ! This has just too many arguments to use BuildCommand
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "Howler.orientation(";
        print (PrintStringOrArray)  MinusSign(x), (PrintStringOrArray) IntToString(Abs(x)), ", ";
        print (PrintStringOrArray)  MinusSign(y), (PrintStringOrArray) IntToString(Abs(y)), ", ";
        print (PrintStringOrArray)  MinusSign(z), (PrintStringOrArray) IntToString(Abs(z)), ", ";
        print (PrintStringOrArray)  MinusSign(xup), (PrintStringOrArray) IntToString(Abs(xup)), ", ";
        print (PrintStringOrArray)  MinusSign(yup), (PrintStringOrArray) IntToString(Abs(yup)), ", ";
        print (PrintStringOrArray)  MinusSign(zup), (PrintStringOrArray) IntToString(Abs(zup));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
        VorpleExecuteJavaScriptCommand(hugehugestr);
];


!=======================
! Setters

! This could be avoided if IntToString supported negative numbers
! TODO fix this after IntToString has been fixed
[ MinusSign x;
    if (x < 0) { return "-"; } else { return "";}
];

[ Abs n;
    if (n < 0) { return -n;} else { return n;}
];

[ HowlerSetStereo o n;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! This could be simplified if IntToString supported negative numbers
        VorpleExecuteJavaScriptCommand(BuildCommand("howls.", VorpleEscape(o.identifier), ".stereo(", MinusSign(n), IntToString(Abs(n)), " / 100);"));
        
    } else { o.stereo = n; }
];

! TODO: give more documentation, I don't understand how it works and why it tapers off so quickly
[ HowlerSetPos o x y z;
        if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! This could be simplified if IntToString supported negative numbers - but we still need to reimplement BuildCommand since it can only take 7 arguments
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".pos(";
        print (PrintStringOrArray) MinusSign(x), (PrintStringOrArray) IntToString(Abs(x)), ", ";
        print (PrintStringOrArray) MinusSign(y), (PrintStringOrArray) IntToString(Abs(y)), ", ";
        print (PrintStringOrArray) MinusSign(z), (PrintStringOrArray) IntToString(Abs(z)), ");";
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
        VorpleExecuteJavaScriptCommand(hugehugestr);
    } else {
        o.&pos-->0 = x;
        o.&pos-->1 = y;
        o.&pos-->2 = z;
    }
];

! TODO: more documentation, understand how it works (doesn't seem to affect the sound??)
[ HowlerSetOrientation o x y z;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        ! This could be simplified if IntToString supported negative numbers - but we still need to reimplement BuildCommand since it can only take 7 arguments
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".orientation(";
        print (PrintStringOrArray)  MinusSign(x), (PrintStringOrArray) IntToString(Abs(x)), ", ";
        print (PrintStringOrArray)  MinusSign(y), (PrintStringOrArray) IntToString(Abs(y)), ", ";
        print (PrintStringOrArray)  MinusSign(z), (PrintStringOrArray) IntToString(Abs(z));
        print ");";
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
        VorpleExecuteJavaScriptCommand(hugehugestr);
    } else {
        o.&orientation-->0 = x;
        o.&orientation-->1 = y;
        o.&orientation-->2 = z;
    }
];

! TODO: THIS HASN'T BEEN TESTED
! TODO: should we actually have a "pannerNode" field in the declaration of the spatial sound?
[ HowlerSetPannerNode o panner;
    if (HowlerIsThatHowlInitialised(o) ~= 0) {
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
        print "howls.", (PrintStringOrArray) VorpleEscape(o.identifier), ".panner({";
        print "coneInnerAngle : ", (PrintStringOrArray) IntToString(panner.coneinnerangle), ",";
        print "coneOuterAngle : ", (PrintStringOrArray) IntToString(panner.coneouterangle), ",";
        print "coneOuterGain : ", (PrintStringOrArray) IntToString(panner.coneoutergain), " / 100,";
        print "distanceModel : ";
        if (panner.distancemodel == DISTANCE_MODEL_LINEAR) { print "'linear'"; }
        if (panner.distancemodel == DISTANCE_MODEL_INVERSE) { print "'inverse'"; }
        if (panner.distancemodel == DISTANCE_MODEL_EXPONENTIAL) { print "'exponential'"; }
        print ", maxDistance : ", (PrintStringOrArray) IntToString(panner.maxdistance), ",";
        print "refDistance : ", (PrintStringOrArray) IntToString(panner.refdistance), ",";
        print "rolloffFactor : ", (PrintStringOrArray) IntToString(panner.rollofffactor), " /100,";
        print "panningModel : ";
            if (panner.panningmodel == PANNING_MODEL_HRTF) { print "'HRTF'}"; }
            if (panner.panningmodel == PANNING_MODEL_EQUALPOWER) { print "'equalpower'}"; }
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
        VorpleExecuteJavaScriptCommand(hugehugestr);
    } else {
        o.pannerattr = panner;
    }
];




!=======================
! Getters

[ HowlerGetStereo o;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        return o.initial_rate;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".stereo() * 100;"));
        return VorpleWhatNumberWasReturned();
    }
];

Array pos_coordinates-->3;
[ HowlerGetPos o;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        pos_coordinates-->0 = o.&pos-->0;
        pos_coordinates-->1 = o.&pos-->1;
        pos_coordinates-->2 = o.&pos-->2;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".pos()[0];"));
        pos_coordinates-->0 = VorpleWhatNumberWasReturned();
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".pos()[1];"));
        pos_coordinates-->1 = VorpleWhatNumberWasReturned();
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".pos()[2];"));
        pos_coordinates-->2 = VorpleWhatNumberWasReturned();
    }
    return pos_coordinates;
];


Array orientation_coordinates-->3;
[ HowlerGetOrientation o;
    if (HowlerIsThatHowlInitialised(o) == 0) {
        orientation_coordinates-->0 = o.&orientation-->0;
        orientation_coordinates-->1 = o.&orientation-->1;
        orientation_coordinates-->2 = o.&orientation-->2;
    } else {
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".orientation()[0];"));
        orientation_coordinates-->0 = VorpleWhatNumberWasReturned();
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".orientation()[1];"));
        orientation_coordinates-->1 = VorpleWhatNumberWasReturned();
        VorpleExecuteJavaScriptCommand(BuildCommand("return howls.", VorpleEscape(o.identifier), ".orientation()[2];"));
        orientation_coordinates-->2 = VorpleWhatNumberWasReturned();
    }
    return orientation_coordinates;
];

! TODO: get panner node values ? Or do we just store everything in Inform ?


