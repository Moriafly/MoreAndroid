# Activity

```kotlin
class DemoActivity : AppCompatActivity() {
    
    override fun onCreate(savedInstanceState: Bundle?) {
        // 调用父类的 onCreate() 方法，默认实现
        super.onCreate(savedInstanceState)
    }
    
}
```

## 逻辑与视图分离



## 销毁一个 Activity

```kotlin
finish()
```

## Activity 生命周期

掌握 Activity 生命周期十分重要。

### Activity 状态



```mermaid
graph TB
	startActivity(启动 Activity)-->onCreate(onCreate)-->
	onStart-->onResume-->Activity运行中--第一个Activity来到前台-->onPause
    id1(圆角矩形)--普通线-->id2[矩形]
    subgraph 子图表
        id2==粗线==>id3{菱形}
        id3-.虚线.->id4>右向旗帜]
        id3--无箭头---id5((圆形))
    end
```



