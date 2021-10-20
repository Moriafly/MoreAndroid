# RecyclerView

RecyclerView 介绍

[官方文档](https://developer.android.google.cn/jetpack/androidx/releases/recyclerview)

## ListAdapter

# 

```kotlin
package moriafly.widget

import android.content.Context
import android.util.AttributeSet
import android.view.MotionEvent
import androidx.recyclerview.widget.RecyclerView
import kotlin.math.abs

/**
 * RecyclerViewAtViewPager2
 * 解决 ViewPager2 和 RecyclerView滑动冲突
 * @author Moriafly
 */
class RecyclerViewAtViewPager2 : RecyclerView {
    constructor(context: Context) : super(context)
    constructor(context: Context, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    private var startX = 0
    private var startY = 0
    override fun dispatchTouchEvent(motionEvent: MotionEvent): Boolean {
        when (motionEvent.action) {
            MotionEvent.ACTION_DOWN -> {
                startX = motionEvent.x.toInt()
                startY = motionEvent.y.toInt()
                parent.requestDisallowInterceptTouchEvent(true)
            }
            MotionEvent.ACTION_MOVE -> {
                val endX = motionEvent.x.toInt()
                val endY = motionEvent.y.toInt()
                val differenceX = abs(endX - startX)
                val differenceY = abs(endY - startY)
                if (differenceX > differenceY) {
                    parent.requestDisallowInterceptTouchEvent(canScrollHorizontally(startX - endX))
                } else {
                    parent.requestDisallowInterceptTouchEvent(canScrollVertically(startY - endY))
                }
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> parent.requestDisallowInterceptTouchEvent(
                false
            )
        }
        return super.dispatchTouchEvent(motionEvent)
    }
}
```