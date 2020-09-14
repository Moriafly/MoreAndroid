第一个 Activity：FirstActivity

第二个 Activity：SecondActivity

## 显式 Intent

```kotlin
btn1.setOnClickListener {
    val intent = Intent(this, secondActivity::class.java)
    startActivity(intent)
}
```

## 隐式 Intent

不明确指出想要启动哪一个 Activity，而是指定了一系列更抽象的 action 和 category 信息，然后交给系统去分析这个 Intent，并帮我们找出合适的 Activity 去启动。

设定一个可以响应这个隐式 Intent 的 Activity：

打开 AndroidManifest.xml，添加：

```xml
<activity android:name=".SecondActivity">
	<intent-filter>
    	<action android:name="com.example.demo.ACTION_START" />
        <category android:name="android.intent.category.DEFAULT" />
    </intent-filter>
</activity>
```

在 action 标签中指明了此 Activity 可以响应 com.example.demo.ACTION_START 这个 action，而 category 标签包含一些附加信息。

同时匹配 action 和 category，这个 Activity 才能响应该 Intent。

```kotlin
btn1.setOnClickListener {
    val intent = Intent("com.example.demo.ACTION_START")
    startActivity(intent)
}
```

android.intent.category.DEFAULT 是一种默认的 category，会自动添加，所以不用写。

每个 Intent 中只能有一个 action，但能有多个 category。

```xml
<activity android:name=".SecondActivity">
	<intent-filter>
    	<action android:name="com.example.demo.ACTION_START" />
        <category android:name="android.intent.category.DEFAULT" />
        <category android:name="com.example.demo.MY_CATEGORY" />
    </intent-filter>
</activity>
```

```kotlin
btn1.setOnClickListener {
    val intent = Intent("com.example.demo.ACTION_START")
    intent.addCategory("com.example.demo.MY_CATEGORY")
    startActivity(intent)
}
```

## 更多隐式 Intent 用法

隐式 Intent 不仅可以启动自己程序内的 Activity，还可以启动其他程序的 Activity。

用浏览器打开网页：

```kotlin
btn1.setOnClickListener {
    val intent = Intent(Intent.ACTION_VIEW)
    intent.data = Uri.parse("https://www.baidu.com")
    startActivity(intent)
}
```

