# Digital-Clock
A digital clock with ESP32 and OLED.
It should work with Arduino but it uses [U8G2 library](https://github.com/olikraus/u8g2/wiki/u8g2reference) which uses a lot of RAM so Uno may not be enough. Also it requires a WiFi module.

To-Do:
- [ ] Re-write the entire display code with a much less power intensive [U8x8](https://github.com/olikraus/u8g2/wiki/u8x8reference) library
