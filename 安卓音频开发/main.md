Android 音频播放的一般流程是：

播放器从本地音频文件或网络加载编码后的音频数据，解码为 pcm 数据写入 AudioTrack
AudioTrack 将 pcm 数据写入 FIFO
AudioFlinger 中的 MixerThread 通过 AudioMixer 读取 FIFO 中的数据进行混音后写入 HAL 输出设备进行播放