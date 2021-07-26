System_file;

!==============================
! Vorple - Tuna Convolver for Howler
!    a wrapper to use the convolution in the Tuna WebAudio library with Howler

!  Tuna is a JavaScript sound effect library with a few very interesting effects.
!  I needed a convolver for one of my projects that was already using Howler, so
!  I wrote an interface between Vorple and Tuna's convolvers applied to Howler
!  sounds. It works pretty well!
!  
!  A convolver's effect is defined by: an impulse sound file (there are tons on
!  freesound.org, depending on the effect you want to achieve); the dryLeven and
!  wetLevel parameters control respectively the volume of your regular sound and
!  the volume of the impulse you're convolving with; a level parameter so you can
!  adjust the overall volume of the result; and highCut and lowCut parameters
!  specifying the high and low frequencies you want to cut, if any.
!  
!  After initialising your convolver object using
!  TunaInitialiseConvolver(yourConvolver), the only thing you can do with this
!  extension is plug a convolver to the master node, i.e. the output node of Howler,
!  using HowlerPlugMasterToConvolver(yourConvolver). To go back to a dry sound,
!  just call HowlerUnplugConvolver().

Include "vorple-howler.h";

Attribute initialised;


! Initializing Tuna
! This is ran after VorpleSetupHowler (because of the way things are included) so Howler.ctx isn't null
Object VorpleSetupTunaRule "" VorpleInterfaceSetup
    with description [;
        VorpleExecuteJavaScriptCommand("window.tuna = new Tuna(Howler.ctx);");
    ];	



! Convolver class

Class Convolver
  with  identifier "REPLACE_ME",    ! must be a string
        highCut 22050,          ! 20 to 22050
        lowCut 20,              ! 20 to 22050
        dryLevel 1,             ! 0 to 1+
        wetLevel 1,             ! 0 to 1+
        level 1,                ! 0 to 1+, adjusts total output of both wet and dry impulse
        impulse "REPLACE_ME",   ! the path to your impulse response
        bypass 0,
;
  
! we use hugehugestr here, so careful about the side-effects to BuildCommand (there shouldn't be any)
! TODO: we're basically reimplementing BuildCommand, but should we be implementing ConcatenateUnicodeStrings (i.e. unicode conversion too) here?
[ TunaInitialiseConvolver o;
    if (o hasnt initialised) { ! TODO: replace this with a JS call, if it exists?
        !@output_stream 3 hugehugestr;
        bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
 
        print "window.convolver", (string) o.identifier; print " = new window.tuna.Convolver({";
        print "highCut: ", o.highCut, ", ";
        print "lowCut: ", o.lowCut, ", ";
        print "dryLevel: ", o.dryLevel, ", ";
        print "wetLevel: ", o.wetLevel, ", ";
        print "level: ", o.level, ", ";
        print "impulse: '", (PrintStringOrArray) o.impulse, "', ";
        print "bypass:", o.bypass;
        print "})";
        !@output_stream -3;
        bp_output_stream(-3);
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("Howler command too long; please increase LEN_HUGEHUGESTR");
	}
!	PrintStringOrArray(hugehugestr); print "^";
	VorpleExecuteJavaScriptCommand(hugehugestr);
        give o initialised;
    }
];

[ HowlerPlugMasterToConvolver o;
    VorpleExecuteJavaScriptCommand("Howler.masterGain.disconnect();");
    VorpleExecuteJavaScriptCommand(BuildCommand("Howler.masterGain.connect(window.convolver", o.identifier, ");"));
    VorpleExecuteJavaScriptCommand(BuildCommand("window.convolver", o.identifier, ".connect(Howler.ctx.destination);"));
];

[ HowlerUnplugConvolver o;
    VorpleExecuteJavaScriptCommand("Howler.masterGain.disconnect();");
    VorpleExecuteJavaScriptCommand(BuildCommand("Howler.masterGain.connect(Howler.ctx.destination);"));
];
