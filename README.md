# bobs-rocket-panel
A hardware control panel for Kerbal Space Program and the software to support it. This is not a new concept, but if I wanted to play something off the shelf, I'd be playing a flight simulator. This is Kerbal, and we build crazy things for it.

## Stack
* Connection with Kerbal is provided through [Telemachus|https://forum.kerbalspaceprogram.com/index.php?/topic/172757-142-telemachus-continued-web-based-telemetry-and-cameras/]
* Driver connects to Telemachus through websockets and hardware over serial
* Hardware is Arduino-based
