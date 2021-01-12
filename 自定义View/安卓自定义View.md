# 安卓中的坐标系

## 屏幕坐标系和数学坐标系的区别
由于移动设备一般定义屏幕左上角为坐标原点，向右为 x 轴增大方向，向下为 y 轴增大方向，
所以在手机屏幕上的坐标系与数学中常见的坐标系是稍微有点差别的。

## View 的坐标系

**注意：View 的坐标系统是相对于父控件而言的。**

``` java
getTop();       // 获取子 View 左上角距父 View 顶部的距离
getLeft();      // 获取子 View 左上角距父 View 左侧的距离
getBottom();    // 获取子 View 右下角距父 View 顶部的距离
getRight();     // 获取子 View 右下角距父 View 左侧的距离
```

## MotionEvent 中 get 和 getRaw 的区别

```
event.getX();       // 触摸点相对于其所在组件坐标系的坐标
event.getY();

event.getRawX();    // 触摸点相对于屏幕默认坐标系的坐标
event.getRawY();

```


## 核心要点

序号 | 要点
:---:|----
  1  | 在数学中常见的坐标系与屏幕默认坐标系的差别
  2  | View 的坐标系是相对于父控件而言的
  3  | MotionEvent 中 get 和 getRaw 的区别
