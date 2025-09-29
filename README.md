# ESP32 Internet Radio

Professional internet radio player with circular TFT display and web configuration interface.

## Features
- 🎵 MP3 stream playback
- 📻 Circular 1.28" GC9A01 TFT display
- 🌐 Web configuration interface
- 💾 URL persistence in SPIFFS
- 🔊 I2S audio output
- 📶 WiFi auto-reconnect

## Hardware Requirements
- ESP32 Dev Board
- GC9A01 1.28" 240x240 Circular TFT
- I2S DAC (MAX98357 or similar)
- Speakers

## Wiring
| TFT | ESP32 |
|-----|-------|
| SCLK| GPIO18|
| MOSI| GPIO23|
| CS  | GPIO5 |
| DC  | GPIO2 |
| RST | GPIO4 |
| BL  | GPIO15|

| I2S | ESP32 |
|-----|-------|
| BCK | GPIO26|
| LRC | GPIO25|
| DIN | GPIO22|

## Installation
1. Install required libraries:
   - TFT_eSPI
   - ESP32-audioI2S

2. Configure TFT_eSPI:
   - Edit `User_Setup.h` for GC9A01
   - Set correct pin configuration

3. Upload code and access web interface:
   - Connect to ESP32 WiFi
   - Open browser to `http://[ESP_IP]`

## Usage
- Change stream URLs via web interface
- Settings saved automatically
- Auto-reconnect on WiFi failure
- Real-time status display

## Stream Examples
- 80s Hits: `http://streams.rsa-sachsen.de/80erhits/mp3-192/streams.rsa-sachsen.de`
- 90s Hits: `http://streams.rsa-sachsen.de/90erhits/mp3-192/streams.rsa-sachsen.de`
- Rock: `http://streams.rsa-sachsen.de/rsa-rock/mp3-192/streams.rsa-sachsen.de`

## License
MIT License
