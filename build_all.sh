rm -rf .pio/*
pio run  -e lilygo21
pio run  -e lilygo27
pio run  -e lilygo29
pio run  -e lilygo47
pio run  -e lilygo21dev
pio run  -e lilygo27dev
pio run  -e lilygo29dev
pio run  -e lilygo47dev
cp .pio/build/lilygo21dev/esp32_vorrat*.bin bin.dev
cp .pio/build/lilygo27dev/esp32_vorrat*.bin bin.dev
cp .pio/build/lilygo29dev/esp32_vorrat*.bin bin.dev
cp .pio/build/lilygo47dev/esp32_vorrat*.bin bin.dev
cp .pio/build/lilygo21/esp32_vorrat*.bin bin
cp .pio/build/lilygo27/esp32_vorrat*.bin bin
cp .pio/build/lilygo29/esp32_vorrat*.bin bin
cp .pio/build/lilygo47/esp32_vorrat*.bin bin
mv bin/*dev.version bin.dev
