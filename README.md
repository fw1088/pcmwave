# pcmwave
pcm数据可视化
![image](http://thumbnail0.baidupcs.com/thumbnail/712b16be750940a8d8505621f371230a?fid=17709960-250528-197595827664375&time=1472274000&rt=pr&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-AjTy3WssXDCZZBu67A9Bghsqr%2bs%3d&expires=8h&chkbd=0&chkv=0&dp-logid=5556372593844183354&dp-callid=0&size=c1280_u720&quality=90)
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
