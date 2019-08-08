! Vorplectron by Nathanael Marion (I6 version by Hugo Labrande)
!    "Makes it possible for a Vorple story package with Vorplectron to communicate with the latter."

System_file;
#Ifndef VORPLECTRON;
Constant VORPLECTRON;

Include "vorple.h";

!=================================================
! Part - To know if we are in vorplectron

Global in_vorplectron = 0;

! We put this in the interface setup rulebook but it should be in VorpleStartup (or maybe VorpleStartup should be a rulebook)
Object detectIfVorplectronIsRunning "" VorpleInterfaceSetup
    with description [;
        if (isVorpleSupported()) {
            VorpleExecuteJavaScriptCommand("return (typeof vorplectron !== 'undefined')");
            if (VorpleWhatBooleanWasReturned()) {
                in_vorplectron = 1;
            }
        }
    ];
    
[ areWeInVorplectron ;
    return in_vorplectron;
];


!=================================================
! Part - Sending messages to Vorplectron

[ VorplectronSendMessage msg arg ;
    if (areWeInVorplectron()) {
        VorpleExecuteJavaScriptCommand(BuildCommand("return vorplectron.send('", VorpleEscape(msg), "', ",arg,")"));
    }
];

! Put this after VorpleInitalise() in Initialise
[ VorplectronStoryReady ;
    VorplectronSendMessage("vorpleReady");
];

[ VorplectronGetVersion ;
    if (areWeInVorplectron()) {
        VorplectronSendMessage("getAppVersion");
        return VorpleWhatTextWasReturned();
    }
    return "";
];

! quitting 
[ VorplectronQuitApp ;
    VorplectronSendMessage("quit");
];
[ VorplectronQuitApplication ;
    VorplectronQuitApp();
];


[ VorplectronQuitSub ;
    L__M(##Quit, 2);
    if (YesOrNo() ~= 0) VorplectronQuitApp();
];

Extend 'quit' replace
*               ->VorplectronQuit;

! fullscreen

[ isVorplectronFullScreen ;
    if (areWeInVorplectron()) {
        VorplectronSendMessage("isFullScreen");
        return VorpleWhatBooleanWasReturned();
    } else { rfalse; }
];

[ toggleFullScreen ;
    VorplectronSendMessage("toggleFullScreen");
];

[ enableFullScreen ;
    VorplectronSendMessage("setFullScreen", "true");
];

[ disableFullScreen ;
    VorplectronSendMessage("setFullScreen", "false");
];



!==========================
! External files

Constant GLKDATA = 0;
Constant GLKSAVE = 1;

[ askPlayerToSave file type         i ;
    if (type == GLKDATA) { i = "glkdata"; } else { i = "glksave"; }
    if (areWeInVorplectron()) {
        VorpleExecuteJavaScriptCommand(BuildCommand("return vorplectron.send('saveExternalFile', { data:FS.readFile('/gamedata/",file,".", i, "', {encoding: 'binary'}), type:'",i, "' }"));
    }
];

[ askPlayerToLoad file type         i ;
    if (type == GLKDATA) { i = "glkdata"; } else { i = "glksave"; }
    if (areWeInVorplectron() ) {
        VorpleExecuteJavaScriptCommand(BuildCommand("
		{
			let data = vorplectron.send('loadExternalFile', '", i, "');
			if (data) {
				FS.writeFile('/gamedata/", file, ".", i, "', data);
				return true;
			} else {
				return false;
			}
		}
		"));
    }
];


#Endif;
