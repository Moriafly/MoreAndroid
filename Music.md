# 音乐播放器开发

## 使用 MediaPlayer



## MediaPlayer 生命周期

![image-20200915103012457](C:\Users\24568\AppData\Roaming\Typora\typora-user-images\image-20200915103012457.png)

1. Idle 创建后 `setDataSource()` 设置播放路径，进入初始化阶段，初始化阶段资源是没有加载进入内存的，没开始播放。

2. 把资源加载进内存中。有两种准备方式：

   - `prepare()` 准备

     prepare 后进入 prepared 状态，说明完全加载进内存了。

   - `prepareAsync()` 异步准备，文件比较大的话，使用这个，不会阻塞 UI 线程。设置一个 `OnPreparedListener.onPrepared` 方法监听，执行到这个监听说明加载完成。

3. `start()` 方法 Start 播放。

4. Paused 方法暂停。Paused 状态下 start() 重新播放。

5. stop() 停止，把资源从内存中移除。再次播放要重新 `prepareAsync()` 或者  `prepare()`。



外部网址，流式传输

```kotlin
    val url = "http://........" // 网址
    val mediaPlayer: MediaPlayer? = MediaPlayer().apply {
        setAudioStreamType(AudioManager.STREAM_MUSIC) // 媒体音频
        setDataSource(url)
        prepare() // 可能会花很长时间！(缓冲等)
        start()
    }
```

**注意**：如果您传递某个网址以流式传输在线媒体文件，则该文件必须能够进行渐进式下载。

**注意**：使用 `setDataSource()` 时，您必须捕获或传递 `IllegalArgumentException` 和 `IOException`，因为您引用的文件可能并不存在。

### 异步准备

原则上，使用 `MediaPlayer` 会非常简单。不过，请务必注意，要正确地将其与典型的 Android 应用集成，还需执行一些额外操作。例如，对 `prepare()` 的调用可能需要很长时间来执行，因为它可能涉及获取和解码媒体数据。因此，与任何可能需要很长时间来执行的方法一样，**切勿从应用的界面线程（主线程）中调用它**。这样做会导致界面挂起，直到系统返回该方法，这是一种非常糟糕的用户体验，并且可能会导致 ANR（应用无响应）错误。即使您预计资源会快速加载，但也请记得，界面中任何响应时间超过十分之一秒的操作都会导致明显的暂停，并让用户觉得您的应用运行缓慢。

为避免界面线程挂起，请生成其他线程来准备 `MediaPlayer`，并在准备工作完成后通知（写个接口，Callback）主线程。

不过，尽管您可以自行编写线程逻辑，但此模式在使用 `MediaPlayer` 时非常普遍，因此框架通过 `prepareAsync()` （异步准备）方法提供了一种完成此任务的便捷方式。此方法会在后台开始准备媒体，并立即返回。当媒体准备就绪后，系统会调用通过 `setOnPreparedListener()` 配置的 `MediaPlayer.OnPreparedListener` 的 `onPrepared()` 方法。

## 在 Service 中使用 MediaPlayer

如果您希望即使当应用未在屏幕上显示时，应用仍会在后台播放媒体内容（也就是说，您希望应用在用户与其他应用互动时继续播放媒体内容），则您必须启动一个 Service 并由此控制 `MediaPlayer` 实例。您需要将 MediaPlayer 嵌入到 `MediaBrowserServiceCompat` Service 中，并使其在其他 Activity 中与 `MediaBrowserCompat` 进行互动。

您应该注意这种客户端/服务器设置。我们对在后台服务中运行的播放器如何与系统的其他部分进行互动设定了预期。如果您的应用未满足这些预期，则可能会导致用户体验不佳。要了解完整详情，请参阅[构建音频应用](https://developer.android.google.cn/guide/topics/media-apps/audio-app/building-an-audio-app)。

本部分介绍了在 Service 内部实现 MediaPlayer 时如何对其进行管理的特殊说明。

### 异步运行

首先，与 `Activity` 类似，`Service` 中的所有工作均默认在单个线程中完成 - 实际上，如果您从同一应用中运行 Activity 和 Service，则它们会默认使用相同的线程（“主线程”）。因此，Service 需要快速处理传入的 Intent，并且在响应它们时避免执行冗长的计算。如果需要完成大量的工作或可能会阻塞调用，则您必须异步执行这些任务：从您自己实现的其他线程异步执行，或使用框架的诸多工具进行异步处理。

例如，从您的主线程中使用 `MediaPlayer` 时，您应该调用 `prepareAsync()` 而非 `prepare()`，并实现 `MediaPlayer.OnPreparedListener`，以便在准备工作完成后获得通知并开始播放。例如：

```kotlin
private const val ACTION_PLAY: String = "com.example.action.PLAY" // 定义一个 Action

class MusicService: Service(), MediaPlayer.OnPreparedListener {

    private var mediaPlayer: MediaPlayer? = null

    override fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int {
        ...
        val action: String = intent.action
        when(action) {
            ACTION_PLAY -> {
                mediaPlayer = ... // initialize it here
                mediaPlayer?.apply {
                    setOnPreparedListener(this@MyService)
                    prepareAsync() // prepare async to not block main thread
                }

            }
        }
        ...
    }

    /** Called when MediaPlayer is ready */
    override fun onPrepared(mediaPlayer: MediaPlayer) {
        mediaPlayer.start()
    }
}
   
```

#### MediaPlayerNative: error (-38, 0) 错误

暂时未找到解决方法



引起这个异常的原因有几个：
一是如果mediaPlayer对象为null,那么在执行这个方法的时候会报异常
二是在mediaPlayer对象为不可用状态的时候也会报这个错

getDuration必须在prepared回调完成后才可以调用。



QQ 最新音乐排行榜

一个月内发布的新歌，在7天内的有效播放次数的涨幅，由高到低取前100，默认100首最新歌曲

[https://c.y.qq.com/v8/fcg-bin/fcg_v8_toplist_cp.fcg?g_tk=5381&uin=0&format=json&inCharset=utf-8&outCharset=utf-8%C2%ACice=0&platform=h5&needNewCode=1&tpl=3&page=detail&type=top&topid=27&_=1519963122923](https://c.y.qq.com/v8/fcg-bin/fcg_v8_toplist_cp.fcg?g_tk=5381&uin=0&format=json&inCharset=utf-8&outCharset=utf-8¬ice=0&platform=h5&needNewCode=1&tpl=3&page=detail&type=top&topid=27&_=1519963122923)

