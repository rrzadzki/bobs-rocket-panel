# Bob's Rocket Panel
A hardware control panel for Kerbal Space Program built on the Kerbal SimPit mod and designed to keep essential gauges, information, and non-flight controls front and center.

## Functional Goals
* Visual access to all resource levels
* Visual access and visual warnings of safety critical information (like altitude above the surface, velocities)
* Selection and toggling of all action groups
* Big, beautiful arm-and-fire staging control

## Non-Functional Goals
* Low latency
* Well-documented code and build
* Physically hearty and satisfying to chonk those switches

## Stack

My original approach was to use Telemachus as the interface into Kerbal, with a driver program subscribing to Telemachus websockets and passing information back and forth between there and USB with the controller. After some dozen hours or so working that out, I made a discovery;

***Do you know there's a Kerbal Controllers subreddit?***

It's true. I was at this a while before I discovered it, actually while trying to figure out if there was a Telemachus fork still alive. /r/KerbalControllers led me to the KerbalSimPit mod (via their extremely helpful [Software Comparison Thread](https://www.reddit.com/r/KerbalControllers/comments/ajavos/software_comparison_thread/)) which pairs with an Arduino library to integrate the game and hardware controllers.

As much as I enjoyed learning threading, serial communications, and websockets in C++, I've now opted to go with KerbalSimPit as the basis of my software just to give this project a hope of being completed before I get my degree.

The hardware will be Arduino based, with some support boards I'm planning to cut myself on my little CNC router. 