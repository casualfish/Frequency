# Frequency
simple frequency analyzer for linux
程序整体的设计比较简单，如下图所示，由三个组成部分，其中箭头所指是数据的流动方向。
SampleProducer负责对声音进行采集的模块，采集到的数据交给SoundTransformer进行处理，处理完成的数据最终交给FrequencyDisplay进行显示。
程序中SampleProducer使用了gstreamer(http://gstreamer.freedesktop.org/)对音频数据进行采集，
因此不必再通过最原始的alsa库获取声音，gstreamer抽象出的管道和插件的概念很符合视音频数据的流的特征，
其api使用起来也很方便。SoundTransformer主要实现了快速傅立叶转换功能，将模拟数据转换为可以显示的离散型数据。
FrequencyDisplay使用了gdk的2d矢量绘图库cairo(http://cairographics.org/)进行绘图。
在实现中为auidosrc注册了一个回调函数，每当有新的数据生成就调用transformer的进行转换，
同时在FrequencyDisplay中注册了一个定时器，每隔一定时间进行重新绘图，动态显示声音的变化。


![image](https://user-images.githubusercontent.com/1918039/170303774-8eb68da1-622b-4e90-80d2-ad2f97dbf726.png)
