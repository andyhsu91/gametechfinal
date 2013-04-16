Alejandro Weibel
Andy Hsu
Nelson Cheng

./makeit will compile it
./assignment3 will run it


Controls
---------------------------------------------------
	- WASD moves your paddle
	- Mouse movement changes the direction the camera is looking
	- 'C' turns off and on mouse to click buttons
	- 'M' is mute
	- 'P' is pause
	- 'Esc' quits the game

Comments
---------------------------------------------------
	- Network Manager searches listens for any server broadcast packets in its constructor, if none are received then the network manager becomes a server, and begins broadcasting packets on the network.
	
	

Functionality that's been implemented
---------------------------------------------------------------
	- Network Manager has been worked on extensively and now works amazingly well
	- Network Manager automatically searches the local network and detects any available servers or clients, without any human input
	- gameUpdate has been moved to it's own file
	- Both single player and multiplayer modes work
	- Scores has been changed to have multiple scores
	- I/O has been refactored to handle different players
	- Extensive work in the Ball and Player classes to handle updates from the network on the client side
	- Added main menu


Issues Encountered
---------------------------------------------------------------
	- SDL_net does not provide a way to get your own IP address, so getMyIp() in NetworkManager uses a hacky way to get it
	- Removing all the bugs in the Network Manager was very very time consuming.
	

What we still have to accomplish:
---------------------------------------------------
	- Success and failure sound effects don't play correctly on the client side
	- Pausing kind of works in multiplayer. If the server pauses the game, the client continues to estimate where the ball will go, but as soon as the server unpauses everything resyncs with the server and continues normally. 
	- Move the mouse out of the way at beginning of game
	
