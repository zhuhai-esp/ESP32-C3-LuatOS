ESP32-C3-LuatOS
=========

### ESP32-C3
基于合宙ESP32-C3开发板做一些有趣的实验

### 解锁GPIO11作为IO
* 默认输出高电平，解锁后才会受程序控制，不可逆
```shell
espefuse.py -p 端口 burn_efuse VDD_SPI_AS_GPIO 1
```
* 如何查看端口，使用以下命令
```shell
esptool.py read_mac
```

### TFT Info:
```
#define TFT_WIDTH  80
#define TFT_HEIGHT 160

#define TFT_MISO -1
#define TFT_MOSI 3
#define TFT_SCLK 2
#define TFT_CS    7  // Chip select control pin
#define TFT_DC    6  // Data Command control pin
#define TFT_RST   10  // Reset pin (could connect to RST pin)
```

### Links
* [ESP32-C3-LuatOS](https://wiki.luatos.com/chips/esp32c3/index.html)

