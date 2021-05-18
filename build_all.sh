rm -rf .pio/*
pio run  -e lilygo21
pio run  -e lilygo27
pio run  -e lilygo29
pio run  -e lilygo47
cp .pio/build/lilygo21/esp32_vorrat*.bin bin
cp .pio/build/lilygo27/esp32_vorrat*.bin bin
cp .pio/build/lilygo29/esp32_vorrat*.bin bin
cp .pio/build/lilygo47/esp32_vorrat*.bin bin
