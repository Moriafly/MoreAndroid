Kotlin 在 for 循环做了大量修改。

Java 中的 for-i 在 Kotlin 中舍弃了。

## for-in

### 区间

Koltin 中表示一个区间：

```kotlin
val range = 0..10
```

表示 [0, 10] 这个闭区间。

```kotlin
fun main() {
	for (i in 0..10) {
		println(i)
	}
}
```

左闭右开区间：

```kotlin
val range = 0 until 10
```

表示 [0, 10)

