```markdown
# ESP32 Internet Radio

Professional internet radio player with circular TFT display and web configuration interface.

## Features
- 🎵 MP3 stream playback
- 📻 Circular 1.28" GC9A01 TFT display
- 🌐 Web configuration interface
- 💾 URL persistence in SPIFFS
- 🔊 Built-in DAC analog audio output
- 📶 WiFi auto-reconnect

## Hardware Requirements
- ESP32 Dev Board
- GC9A01 1.28" 240x240 Circular TFT
- Speakers with 3.5mm jack
- No external DAC required (uses built-in ESP32 DAC)

## Wiring
| TFT | ESP32 |
|-----|-------|
| SCLK| GPIO18|
| MOSI| GPIO23|
| CS  | GPIO5 |
| DC  | GPIO2 |
| RST | GPIO4 |
| BL  | GPIO15|

**Audio Output:**
- Left Channel: GPIO25 (DAC1)
- Right Channel: GPIO26 (DAC2)
- Ground: GND

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

## Audio Connection
Connect speakers directly to:
- GPIO25 → Left channel
- GPIO26 → Right channel  
- GND → Ground

## License
MIT License
```
