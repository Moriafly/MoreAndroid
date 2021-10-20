# Android FFmpeg 解码音频文件为 PCM

作者：Moriafly（不要糖醋放椒盐）

#### 指向媒体源文件地址

声明不可变指针指向源音频文件地址

```cpp
const char *pMediaPath = env->GetStringUTFChars(mediaPath, nullptr); 
```

#### 申请 avFormatContext 空间

```cpp
AVFormatContext *pAVFormatContext = avformat_alloc_context();
```

要记得释放。

结构体 AVFormatContext 描述了一个媒体文件或者媒体流的构成和基本信息。该结构体在 libavformat/avformat.h 声明。指向用户提供的AVFormatContext的指针(由 avformat_alloc_context 分配)。可能是一个指向NULL的指针，在这种情况下，AVFormatContext被该函数分配并写入 ps。注意，用户提供的AVFormatContext在失败时将被释放。

#### 打开媒体文件

打开一个输入流并读取报头。此时解码器没有打开。流必须用avformat_close_input() 关闭。

```cpp
int result = avformat_open_input(&pFormatContext, pMediaPath, nullptr, nullptr);
if (result != 0) {
    // 打开错误
    return -1;
}
```



