# pcmwave
pcm数据可视化
![image](https://github.com/fw1088/pcmwave/drawPcm/output4.png)
# 开发环境
qt5.5 addin+vs2012
# 概述
使用ffmpeg的库解出pcm数据然后绘制出来，支持网络数据也可以支持本地文件
# 存在的问题
由于一秒的数据量有44100多个点，程序一个像素点需要绘制大约3000个数据，导致数据的特征不太明显
暂时还没有找到比较好的数据抽取或者滤波算法，只能大致做到数据可视化
# 工程中使用的替代方案
ffmpeg程序可以生成pcm数据可视化的图片图片的格式为png，可以用于界面效果的显示

# 联系方式
如果有任何问题可以发邮件给我fw1088@163.com
