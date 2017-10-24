! HoveringFillsCommandLine, by Hugo Labrande
! Version 1
!
! Licence: this file is public domain, but I would appreciate being credited in your game :)

! This simple extension recreates a very cool effect I first saw in No Mires, a Spanish game by Edlobez, written in Java. I believe that game won XComp3, a competition for games which experiment with interfaces!
! The function "HoverFillLink" takes two arguments: the text, and the command. The text is displayed in a hyperlink; when the player hovers over the link, the command line fills with the command, and clicking on the link enters the command.

! Usage :
!   print "You can see a "; HoverFillLink("torch", "take torch"); print " here";

Include "vorple.h";
Include "vorple-hyperlinks.h";

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