# Business Indicator

## Purpose of the Product

Standalone microcontroller based device provides a clear, physical way to

communicate user availability using colored lights. It also allows others to leave voice

messages when the user is not in presence. The product aims to improve personal

productivity and boundary-setting in shared environments such as offices, remote

workspaces and libraries.

## Functional Requirements

## Status Indicator Lights:

The device includes 4 LED lights, which represent a different availability status:

1 Red LED: User is busy (do not disturb).

1 Orange LED: User is busy (contact only if urgent).

1 Yellow LED: User is busy (can talk if needed).

1 Green LED: User is available.

## Microcontroller:

The system is controlled by an ESP32	DevKit-C, chosen for its built-in Wi-

Fi/Bluetooth capabilities and sufficient GPIO pins for input/output management.

## Vo i c e R e c o r d i n g S y s t em:

1 Microphone connected to the ESP32 for capturing voice messages.

1 SD Card Module for temporary storage of audio files.

1 Push And Hold Button dedicated to starting/stopping voice recording.


## User Interface:

Main Function Buttons:

1 Start/Stop Button: Controls the countdown timer.

1 Power On/Off Button: Turns the entire device on or off.

1 Play Messages Button: Starts to play all the messages.

Light Control Buttons:

1 Button for Red LED

1 Button for Orange LED

1 Button for Yellow LED

1 Button for Green LED

Rotary Encoders (Turning Knobs):

1 Left Encoder: Navigates between timer units (hours/minutes).

1 Right Encoder: Sets the value of the selected time unit.

## Display:

1 Timer Display which shows the current countdown or scheduled time the user is
available.

## Power:

1 USB-C Socket for power supply.

## Speaker Output:

MAX98357A I2S Amplifier

Converts digital I2S output from ESP32 to analog audio.

Avoids the risks of using ESP32's DAC directly.


4 - ohm, 3W Speaker

For message playback.

Integrated into enclosure for clarity.

## Physical Design(this part is changeable)

The device enclosure will be transparent.

All buttons and rotary encoders will be placed on the front or top of the box for
ease of use.

The microphone will be clearly marked.

## Intended Use Scenarios

Workplace Desk: Alert coworkers about availability without speaking.

Home Office: Prevent interruptions during focused tasks or meetings.

Shared Study Spaces: Signal study/work mode and accept important messages
when away.

# Total Components

Component Quantity

LED Lights 4

Push Buttons 7

Turning Knobs 2

Push And Hold Button 1


# Component Quantity

Microphone 1

SD Card Module 1

Display Screen 1

USB-B Port

# How to Use

**Power On the Device**

Plug in the device using the USB-C port.(If a built-in battery is included, there is no need for that.)

Press the ON/OFF Button to turn it on.

**Set Your Availability Status**

Press one of the 4 Light Buttons to show your current status:
Red = Do Not Disturb
Orange = Contact Only if Urgent
Yellow = Can Talk if Needed
Green = Available

Only one light will be active at a time.

**Use the Timer**

Turn the Left Encoder to switch between hours and minutes.

Turn the Right Encoder to set the time.

Press the Start/Stop Button to begin or pause the countdown.

The timer will be shown on the display screen.

**Record a Voice Message**

If the user is not in presence, press and hold the Record Button to start recording.

Speak into the marked microphone**.**

Release the hold to stop.

File saved to SD card

## Play Messages

When the user presses the 'Play Messages' button, it starts playing all the messages.

A small delay between messages helps identify individual recordings.

**Power Off When Done**

Press the ON/OFF Button again to turn off the device safely.


