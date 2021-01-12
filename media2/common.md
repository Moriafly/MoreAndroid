# androidx.media2.common

## 接口
Interfaces

Rating	An interface to encapsulate rating information used as content metadata. 

## 类
Classes

CallbackMediaItem	Structure for media item descriptor for DataSourceCallback. 
CallbackMediaItem.Builder	This Builder class simplifies the creation of a CallbackMediaItem object. 
DataSourceCallback	For supplying media data, implement this if your app has special requirements for the way media data is obtained. 
FileMediaItem	Structure for media item for a file. 
FileMediaItem.Builder	This Builder class simplifies the creation of a FileMediaItem object. 
MediaItem	A class with information on a single media item with the metadata information. 
MediaItem.Builder	Builder for MediaItem. 
MediaMetadata	Contains metadata about an item, such as the title, artist, etc. 
MediaMetadata.Builder	Use to build MediaMetadatax objects. 
SessionPlayer	Base interface for all media players that want media session. 
SessionPlayer.PlayerCallback	A callback class to receive notifications for events on the session player. 
SessionPlayer.PlayerResult	Result class of the asynchronous APIs. 
SessionPlayer.TrackInfo	Class for the player to return each audio/video/subtitle track's metadata. 
SubtitleData	Class encapsulating subtitle data, as received through the SessionPlayer.PlayerCallback.onSubtitleData(SessionPlayer, MediaItem, SessionPlayer.TrackInfo, SubtitleData) interface. 
UriMediaItem	Structure for media item descriptor for Uri. 
UriMediaItem.Builder	This Builder class simplifies the creation of a UriMediaItem object. 
VideoSize	Immutable class for describing video size. 

# MediaPlayer

```kotlin
import androidx.media2.player.MediaPlayer
```