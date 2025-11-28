1. 处理文件转换
```
ffmpeg -i input.mp3 \
  -c:a aac \
  -b:a 96k \
  -ar 44100 \
  -ac 1 \
  -movflags +faststart \
  output.aac
```

2. 将文件烧录到文件分区，烧录之前文件需要放到/data目录下
```
pio run --target uploadfs
```