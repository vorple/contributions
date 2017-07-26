! HoveringFillsCommandLine, by Hugo Labrande
! Version 1
!
! Licence: this file is public domain, but I would appreciate being credited in your game :)

! This simple extension recreates an effect I first saw in DONTSEE / No Mires, a Spanish game which entered XComp3, which was written in Java.
! The function "HoverFillLink" takes two arguments: the text, and the command. The text is displayed in a hyperlink; when the player hovers over the link, the command line fills with the command, and clicking on the link enters the command.

Include "vorple.h";
Include "vorple-hyperlinks.h";
Include "vorple-command-prompt-control.h";

[ HoverFillLink text cmd classes   id ;
    if (cmd == 0) { cmd = text; }
    if (isVorpleSupported()){
	id = UniqueIdentifier();
	VorplePlaceElement("a", BuildCommand("vorple-link vorple-commandlink link-", id, " ", classes), text);
	VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').attr('href', '#').attr('data-command', '", VorpleEscape(cmd), "')"));
	VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').hover(function(){ vorple.prompt.setValue('", VorpleEscape(cmd), "'); }, function () { vorple.prompt.setValue(''); } )"));
    } else { print (PrintStringOrArray) text; }
    return ;
];