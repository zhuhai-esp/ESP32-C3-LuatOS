#include <Arduino.h>
#include <AudioFileSourceLittleFS.h>
#include <AudioGeneratorAAC.h>
#include <AudioOutputI2S.h>
#include <LittleFS.h>

#define PIN_LED1 12
#define PIN_LED2 13

class MultiAudioPlayer {
private:
  AudioGeneratorAAC *aac = nullptr;
  AudioFileSourceLittleFS *file = nullptr;
  AudioOutputI2S *out = nullptr;
  
  // 多文件播放相关变量
  const char **playlist = nullptr;  // 播放列表
  int playlistSize = 0;             // 播放列表大小
  int currentTrackIndex = 0;        // 当前播放的曲目索引
  bool isPlaying = false;

public:
  bool initialize() {
    Serial.begin(115200);
    delay(1000);
    if (!LittleFS.begin(true)) {
      Serial.println("LittleFS挂载失败!");
      return false;
    }
    Serial.printf("LittleFS总空间: %.2f KB\n", LittleFS.totalBytes() / 1024.0);
    Serial.printf("LittleFS可用空间: %.2f KB\n",
                  (LittleFS.totalBytes() - LittleFS.usedBytes()) / 1024.0);
    Serial.println("LittleFS初始化成功!");
    return true;
  }

  // 扫描指定扩展名的文件并创建播放列表
  void createPlaylist(const char* extension = ".aac") {
    Serial.println("\n=== 扫描音频文件 ===");
    
    // 第一次扫描：计算文件数量
    int fileCount = 0;
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
      String filename = file.name();
      if (filename.endsWith(extension)) {
        fileCount++;
      }
      file = root.openNextFile();
    }
    root.close();
    
    Serial.printf("找到 %d 个 %s 文件\n", fileCount, extension);
    
    if (fileCount == 0) {
      Serial.println("未找到音频文件!");
      return;
    }
    
    // 分配内存给播放列表
    if (playlist) {
      delete[] playlist;
    }
    playlist = new const char*[fileCount];
    playlistSize = fileCount;
    
    // 第二次扫描：填充播放列表
    int index = 0;
    root = LittleFS.open("/");
    file = root.openNextFile();
    while (file) {
      String filename = file.name();
      if (filename.endsWith(extension)) {
        // 确保路径以/开头
        if (!filename.startsWith("/")) {
          filename = "/" + filename;
        }
        
        // 复制文件名到播放列表
        char* filepath = new char[filename.length() + 1];
        strcpy(filepath, filename.c_str());
        playlist[index++] = filepath;
        Serial.printf("添加到播放列表: %s\n", filepath);
      }
      file = root.openNextFile();
    }
    root.close();
  }

  void listFiles() {
    Serial.println("\n=== 文件列表 ===");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
      Serial.printf("文件: %s, 大小: %.2f KB\n", file.name(),
                    file.size() / 1024.0);
      file = root.openNextFile();
    }
    root.close();
  }

  bool startPlaylist() {
    if (playlistSize == 0) {
      Serial.println("播放列表为空!");
      return false;
    }
    
    currentTrackIndex = 0;
    return playCurrentTrack();
  }

  bool playCurrentTrack() {
    if (playlistSize == 0 || currentTrackIndex >= playlistSize) {
      return false;
    }
    
    if (isPlaying) {
      stopAudio();
    }
    
    const char* filename = playlist[currentTrackIndex];
    
    // 初始化音频输出
    out = new AudioOutputI2S();
    out->SetPinout(4, 8, 9);
    out->SetGain(0.6);
    
    // 打开音频文件
    file = new AudioFileSourceLittleFS(filename);
    aac = new AudioGeneratorAAC();
    
    // 开始播放
    if (aac->begin(file, out)) {
      isPlaying = true;
      Serial.printf("开始播放: [%d/%d] %s\n", 
                    currentTrackIndex + 1, playlistSize, filename);
      return true;
    } else {
      Serial.printf("音频播放器初始化失败: %s\n", filename);
      cleanupAudio();
      return false;
    }
  }

  bool update() {
    if (!isPlaying || !aac) {
      return false;
    }
    
    if (aac->isRunning()) {
      if (!aac->loop()) {
        // 当前音频播放完成
        aac->stop();
        cleanupAudio();
        isPlaying = false;
        
        // 播放下一首（顺序循环）
        playNextTrack();
        return true;
      }
      return true;  // 仍在播放中
    }
    return false;   // 播放已停止
  }

  bool playNextTrack() {
    if (playlistSize == 0) return false;
    
    // 顺序循环播放
    currentTrackIndex++;
    if (currentTrackIndex >= playlistSize) {
      currentTrackIndex = 0;  // 回到第一首，实现循环
      Serial.println("=== 播放列表循环 ===");
    }
    
    return playCurrentTrack();
  }

  void stopAudio() {
    if (aac && aac->isRunning()) {
      aac->stop();
    }
    cleanupAudio();
    isPlaying = false;
    Serial.println("音频播放已停止");
  }

  bool isAudioPlaying() {
    return isPlaying;
  }

  int getCurrentTrackIndex() {
    return currentTrackIndex;
  }

  int getPlaylistSize() {
    return playlistSize;
  }

  const char* getCurrentTrackName() {
    if (playlistSize > 0 && currentTrackIndex < playlistSize) {
      return playlist[currentTrackIndex];
    }
    return nullptr;
  }

  ~MultiAudioPlayer() {
    // 清理播放列表内存
    if (playlist) {
      for (int i = 0; i < playlistSize; i++) {
        delete[] playlist[i];
      }
      delete[] playlist;
    }
  }

private:
  void cleanupAudio() {
    if (aac) {
      delete aac;
      aac = nullptr;
    }
    if (file) {
      delete file;
      file = nullptr;
    }
    if (out) {
      delete out;
      out = nullptr;
    }
  }
};

MultiAudioPlayer audioPlayer;

// LED状态管理
class LEDManager {
private:
  uint8_t led1Pin;
  uint8_t led2Pin;
  bool led1State = false;
  bool led2State = false;
  unsigned long lastToggleTime = 0;
  unsigned long toggleInterval = 200;  // 闪烁间隔(ms)

public:
  LEDManager(uint8_t pin1, uint8_t pin2) : led1Pin(pin1), led2Pin(pin2) {}
  
  void begin() {
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    setLEDs(false, false);  // 初始状态都关闭
  }
  
  void update() {
    unsigned long currentTime = millis();
    if (currentTime - lastToggleTime >= toggleInterval) {
      // 交替闪烁
      led1State = !led1State;
      led2State = !led1State;  // 两个LED状态相反
      
      digitalWrite(led1Pin, led1State ? HIGH : LOW);
      digitalWrite(led2Pin, led2State ? HIGH : LOW);
      
      lastToggleTime = currentTime;
    }
  }
  
  void setLEDs(bool state1, bool state2) {
    led1State = state1;
    led2State = state2;
    digitalWrite(led1Pin, led1State ? HIGH : LOW);
    digitalWrite(led2Pin, led2State ? HIGH : LOW);
  }
  
  void setToggleInterval(unsigned long interval) {
    toggleInterval = interval;
  }
};

LEDManager ledManager(PIN_LED1, PIN_LED2);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  
  // 初始化LED管理器
  ledManager.begin();
  
  // 初始化音频播放器
  if (audioPlayer.initialize()) {
    audioPlayer.listFiles();
    
    // 自动扫描所有AAC文件创建播放列表
    audioPlayer.createPlaylist(".aac");
    
    // 开始顺序循环播放
    if (!audioPlayer.startPlaylist()) {
      Serial.println("无法开始播放音频!");
    }
  } else {
    Serial.println("音频播放器初始化失败，系统无法启动");
    while (1) {
      delay(1000);
    }
  }
}

void loop() {
  // 更新音频播放状态
  audioPlayer.update();
  
  // 只有在播放音频时才闪烁LED
  if (audioPlayer.isAudioPlaying()) {
    ledManager.update();
  } else {
    // 如果没有播放音频，关闭LED
    ledManager.setLEDs(false, false);
    
    // 如果播放停止，尝试重新开始播放列表
    static unsigned long lastRetryTime = 0;
    if (millis() - lastRetryTime > 5000) {  // 每5秒重试一次
      Serial.println("尝试重新开始播放列表...");
      audioPlayer.startPlaylist();
      lastRetryTime = millis();
    }
  }
  
  // 短暂延迟以避免占用太多CPU时间
  delay(1);
}