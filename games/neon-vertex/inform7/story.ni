"Neon Vertex" by Hugo Labrande
[Adapted to Inform 7 by Juhana Leinonen]

Book 1 - Game setup

Include Vorple Hyperlinks by Juhana Leinonen.
Include Vorple Multimedia by Juhana Leinonen.
Include Vorple Notifications by Juhana Leinonen.
Include Vorple Screen Effects by Juhana Leinonen.

Release along with the "Vorple" interpreter.

Release along with the file "bg.jpg".
Release along with the file "titlescreen.gif".
Release along with the file "car.jpg".
Release along with the file "casino.jpg".
Release along with the file "club.jpg".
Release along with the file "theatre.jpg".
Release along with the file "restaurant.jpg".

Release along with the file "title-music.mp3".
Release along with the file "rain-outside.mp3".
Release along with the file "rain-inside-car.mp3".

Release along with the style sheet "style.css".

When play begins:
	center "[bold type]N E O N   V E R T E X[roman type]";
	center "[bold type]by Hugo Labrande[roman type]";
	if Vorple is supported:
		place an image "titlescreen.gif" with the description "Neon Vertex", centered;
	center "[fantasy font style][start link][end style]";
	wait for the SPACE key;
	clear the screen;
	play music file "rain-outside.mp3", looping.

To say start link:
	if Vorple is supported:
		place a link to execute the JavaScript command "vorple.prompt.queueKeypress(' ')" reading "START";
	otherwise:
		say "[paragraph break]Press space to continue."
	

Book 2 - Rooms

Chapter 1 - Parking lot

Parking lot is a room. "You turn your back on the closed restaurant. You're good to go. Ready to roam the wild landscapes, and have a bit of fun elsewhere. Time to hit the road in your car." 
The printed name is "On the parking lot".

The restaurant is scenery in the parking lot. The description is "A fancy restaurant, now closed for the night. You've never been - out of your price range. The place is closed, though you wonder if there's a back entrance... That'd be fun."
Understand "cafe" and "fancy" as the restaurant.

Instead of opening or entering the restaurant:
	say "The front door is locked."
	
The car is an enterable scenery container in the parking lot. The description is "Slick and dependable. It is waiting for you."
Understand "your car" and "slick" and "dependable" as the car.

Before examining the car:
	place an image "car.jpg" with the description "Your car.", centered.

After entering the car:
	say "Here you are. In your car. Many roads await you. You feel like having fun. What do you do?[paragraph break]Do you go to the ";
	place a link to command "theatre";
	say " to a ";
	place a link to command "club";
	say ", or to the ";
	place a link to command "casino";
	say "?";
	play music file "rain-inside-car.mp3", looping.

Instead of attacking the car:
	say "You slam the roof of your car.[first time][line break]The wipers, that were stuck, start moving again. Nice![only]";
	award the "fixer" achievement.

	
Chapter 2 - Back alley

The back alley is south of the parking lot. "Not much to see in the darkness here. There's a back door, I suppose, and a dumpster."

The dumpster is an enterable scenery container in the back alley. The description is "It reeks of fish and oil. It's full."
Understand "garbage" and "bin" and "container" as the dumpster.

Instead of searching the dumpster when the wire is handled:
		say "Nuh-uh, been there done that." instead;

Instead of searching the dumpster:
		say "I guess... You dive in the dumpster and see what is there. That piece of wire looks like it'd be useful.";
		now the player carries the wire.
	
Instead of entering the dumpster:
	try searching the dumpster.
		
The wire is a thing. The description is "A metal wire, a few inches long, pretty thin. Perfect for lockpicking."
Understand "metal" and "lockpick" and "lock pick" and "pick" as the wire.

The back door is a locked openable scenery container in the back alley. "Locked, of course - but the lock doesn't seem too sturdy."

Instead of unlocking the back door with the wire:
	say "After a few seconds, the lock opens. You enter the restaurant, and have the feast of a lifetime...";
	award the "lockpick" achievement;
	place an image "restaurant.jpg" with the description "At the restaurant", centered;
	end the story finally.
	
Instead of opening the back door when the player carries the wire:
	try unlocking the back door with the wire.


Book 3 - Endings

Going to the casino is an action applying to nothing.
Understand "casino" as going to the casino when the player is in the car.

Carry out going to the casino:
	show the "casino" ending.

Going to the club is an action applying to nothing.
Understand "club" as going to the club when the player is in the car.

Carry out going to the club:
	show the "club" ending.

Going to the theatre is an action applying to nothing.
Understand "theatre" as going to the theatre when the player is in the car.

Carry out going to the theatre:
	show the "theatre" ending.
	
To show the (destination - text) ending:
	say "You decide to go to the [destination].";	
	place an image "[destination].jpg" with the description "A [destination] scene", centered;
	say "This was the path you took; but there are many others...";
	play music file "title-music.mp3";
	award the destination achievement;
	end the story finally.
	

Book 4 - Achievements

To award the (description - text) achievement:
	display a success notification reading "You've earned an achievement: [description]".


Book 5 - Information

The story headline is "A Vorple sample game, by Hugo Labrande."
The story creation year is 2018.
	
Showing license information is an action out of world.
Understand "license" and "licence" as showing license information.

Carry out showing license information:
	say "This sample game has been written by Hugo Labrande and adapted to Inform 7 by Juhana Leinonen. It is distributed under Creative Commons by-nc-sa License, which is available at https://creativecommons.org/licenses/by-nc-sa/2.0/. It is meant to showcase the capabilities of Vorple, by Juhana Leinonen, which is under MIT License.[paragraph break]";
	say "The media used in this game are:[line break] - Title screen: POTUS 3012, by Beeple (http://beeple.tumblr.com/post/119532497979/potus-3012-tbt-free-hd-visual-source-material)[line break] - Title music: Dark Hopeful Ambience, by PatrickLieberkind (https://freesound.org/people/PatrickLieberkind/sounds/396471/)[line break] - Background image: I never did write that love song, by Jim Nix (https://www.flickr.com/photos/jimnix/39159662530/)[line break] - Audi TTS, by Listers Group (https://www.flickr.com/photos/listersgroup/20555373522/)[line break] - Joyce theatre, NYC 6/2005, by Susan Sermoneta (https://www.flickr.com/photos/en321/19513570/)[line break] - Hed Kandi - 10 years in Ibiza, by Carlitos (https://www.flickr.com/photos/carlitos/11051208056/)[line break] - WSCATL2017Casino-ALS-46, by Walker Stalker Con (https://www.flickr.com/photos/walkerstalkerphotos/38243113996/)[line break] - GINGER, by Les Hôtels JARO (https://www.flickr.com/photos/95006659@@64N04/9736630882/)[line break] - 1 Minute Rain (medium), by yzhamua (https://freesound.org/people/yzhamua/sounds/188655/)[line break] - Rain on a car roof, by Stevious42 (https://freesound.org/people/Stevious42/sounds/259652/)[paragraph break]".
	
After printing the banner text:
	say "Type LICENSE for the licence."
