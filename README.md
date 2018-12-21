# Bob's Rocket Panel
A hardware control panel for Kerbal Space Program and the software to support it. This is not a new concept, but if I wanted to play something off the shelf, I'd be playing a flight simulator. This is Kerbal, and we build crazy things for it.

## Goals
* Minimum latency hardware->Kerbal
* Low latency Kerbal->hardware
* Low memory footprint on host PC

## Stack
* Connection with Kerbal is provided through [Telemachus](https://forum.kerbalspaceprogram.com/index.php?/topic/172757-142-telemachus-continued-web-based-telemetry-and-cameras/)
* Driver connects to Telemachus through websockets and hardware over serial
* Hardware is Arduino-based

## Todo
* Implement websocket connection to Telemachus
* Send telemetry data to hardware
* Hardware: gauges to display telemetry
* Config file: set serial port, Telemachus host and port, update frequency
* Driver: optional verbose output to console and/or log file

## This is not a complete or even mostly-working thing yet
Do not even use at your own risk! Presently the firmware sends events to the driver and that's it. 
