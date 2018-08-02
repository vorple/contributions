! RightClickCommands, by Hugo Labrande
! Version 1
!
! Licence: this file is public domain, but I would appreciate being credited in your game :)

! Here are three functions to add hyperlinks on which you can left or right click:
!   - ActionsOnClick, which maps the left click to an explicit command and the right click to another one
!   - ActionOnRightClick, which maps the left click to "examine X" where X is the word on which there is a hyperlink, and the right click to an explicit action
!   - LucasClicks, which maps the left click to "examine X" and the right click to a logical command (open it if it's closed, etc)

! Usage:
!
! The following lines are equivalent:
!   print "You can see a "; ActionsOnClick("statue", "examine statue", "push statue"); print " here.";
!   print "You can see a "; ActionOnRightClick("statue", "push statue"); print " here.";
!
! You can also do:
!   Object woodenbox "wooden box" room
!   with name 'wooden' 'box',
!   has openable;
!   print "You can see a "; LucasClicks(woodenbox); print " here";
! This creates a box that will be examined on left click and opened on right click.




! TODO: the player might expect two consecutive right-clicks on the same link to have different commands (like "open box" then "close box")
!       If the text (typically the description) is re-printed the new link will be updated, so it's not too bad.
!       But maybe we want to change all the links that were pointing to "open box" to "close box" depending on the state of the box?
!       For this we would need to:
!           tag all the links pertaining to a specific object, maybe with the command as an attribute,
!           when opening the box, insert a call to a function that would
!                  go through all the links tagged with the name of the object (using a rule in the hyperlinks rulebook for instance)
!                  change the attribute to the right command
!       Might be more hassle than it's worth...





System_file; ! for warning suppression

Include "vorple.h";
Include "vorple-hyperlinks.h";

Array object_name -> 100;
Array actionarray -> 100;
Array actionarray2 -> 100;

[ ActionsOnClick text leftClickAction rightClickAction classes        id ;
    ! Bonus syntax: ActionsOnClick(obj, str1, str2)
    if (text ofclass Object) {
        bp_output_stream(3, object_name, 100);
        print (name) text;
        bp_output_stream(-3);
        ActionsOnClick(object_name, leftClickAction, rightClickAction, classes); return ;
    }

    if (leftClickAction == 0) { print (PrintStringOrArray) text; }
    else {
        if (isVorpleSupported()){
            id = UniqueIdentifier();
            VorplePlaceElement("a", BuildCommand("vorple-link vorple-commandlink link-", id, " ", classes), text);
            ! Action on left click 
            VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').attr('href', '#').attr('data-command', '",VorpleEscape(leftClickAction), "').attr('data-silent', 'false')"));
                        ! from HoveringFillsCommandLine
                        VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').hover(function(){ vorple.prompt.setValue('", VorpleEscape(leftClickAction), "'); }, function () { vorple.prompt.setValue(''); } )"));
            ! Action on right click
            bp_output_stream(3, hugehugestr, 2000);
            print "$('a.link-"; print (PrintStringOrArray) id;
            print "').mousedown(function(event){ switch(event.which) { case 1: break; default: vorple.prompt.queueCommand('";
            print (PrintStringOrArray) VorpleEscape(rightClickAction);
            print "', $(this).data('silent')); break; }});";
            bp_output_stream(-3);
            !print (PrintStringOrArray) hugehugestr;
            VorpleExecuteJavaScriptCommand(hugehugestr);
            ! Disable the contextual menu showing in the browser
            ! NOTE: you may want to disable that menu for the whole game, since the player is encouraged to right-click on words
            !       but may get tired of having to close down the menu. To do so, you can edit your HTML file to add
            !              <body oncontextmenu="return false;">
            VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').contextmenu(function(){return false;});"));
        } else { print (PrintStringOrArray) text; }
    }
];

[ ActionOnRightClick text rightClickAction ;
    bp_output_stream(3, actionarray, 100);
    print "examine ";
    print (PrintStringOrArray) text;
    bp_output_stream(-3);
    ActionsOnClick(text, actionarray, rightClickAction);
];

! In Monkey Island (and others), left click is to examine and right click is to perform the most sensible action
! The most sensible action is determined from the properties of the object
! This piece of code is inspired by a routine I found in the source code of "Looking for Iain"
[ LogicalAction obj;
    if (obj has edible) {return "eat ";}
    if (obj has clothing) {return "wear ";}
    if (obj has switchable) {
        if (obj has on) {return "switch off ";} else {return "switch on ";}
    }
    if (obj has talkable || obj has animate) {return "talk to ";}
    if (obj has door) {return "open ";}
    if (obj has openable) {
        if (obj has open) {return "close ";} else { return "open ";}
    }
    if (obj has container) {return "empty ";}
    if (obj has enterable) {return "enter ";}
    return "use ";
];

[ LucasClicks obj ;
    ! Left click
    bp_output_stream(3, actionarray, 100);
    print "examine ";
    print (name) obj;
    bp_output_stream(-3);
    ! Right click
    bp_output_stream(3, actionarray2, 100);
    print (PrintStringOrArray) LogicalAction(obj);
    print (name) obj;
    bp_output_stream(-3);
    
    ActionsOnClick(obj, actionarray, actionarray2);
];

