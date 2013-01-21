Eggbot.
=======

January's robot is an eggbot.

The eggbot:
* brings a container of water to a boil
* retrieves a room-temperature egg from the storage gantry
* immerses the egg in simmering water (about 95C) for five minutes
* immerses the egg in cool water for 45 seconds
* places the egg in an egg-cup

Elements of Eggbot
==================

Gripper
-------

This is the effector that grasps the egg. The part which will be immersed in water will be made of stainless steel and silicone. It will hold the egg for the entire cycle.

Storage gantry
--------------

This is a room-temperature rack for holding one or more uncooked eggs.

Cooker
------

This is a small metal bowl with an immersion heater and temperature sensor. It will bring the water to a boil and then down to a steady 95C for the duration of the cook cycle.

Cooler
------

Equally sized bowl for quickly cooling the egg after the heating cycle.

Egg cup
-------

An egg cup with an aligned base for receiving the egg after cooling.

Crane
-----

A rotating or gantry crane for lowering, raising, and translating the gripper.

Decisions
=========

* Rotating or gantry crane?
* Steppers or servos?
* Encoders?
* Electronics? Teensy 2++, RPi, Beaglebone?

Longshot functionality
======================

* Automatic filling and draining of bowls
* Fill sensors for bowls to ensure they're sufficiently full
* Fan for additional evaporative cooling of egg
