## 2048

该项目是模仿2048游戏，用cocos2dx 2.6实现的一个版本。

1，游戏核心功能完全具备，可记分，重新来过等。

2，涉及到Cocos2dx中的Director、Scene、Layer、Node、LayerColor、LabelTTF、Sprite等常见元素的使用。

3，opengl坐标系以及视图坐标系的计算。

4，3个场景，主场景，菜单场景，游戏结束场景。


##注意事项：
1，本工程依赖于cocos2dx 2.6版本，在3.X版本下不可用。
2，可以按照cocos2dx的指引创建工程，然后将该项目中Classes文件夹下的文件替换。如果是Android工程，需要修改Android.mk文件
3，场景间切换传参没有找到比较优的方法，使用全局变量实现。
4，部分动画实现效果并不完美。
