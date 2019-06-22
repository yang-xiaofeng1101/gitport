程序入口在Find_TargetArmor中，返回3f坐标。

* 识别装甲：`cv::RotatedRect ArmorTrigger::Find_TargetArmor(cv::Mat &tostrike, int n)`

  预处理之后找到轮廓进行分析，进行角度的变化，通过面积和比例筛选出目标装甲。识别出的装甲。

  通过ROICut函数以装甲板向四向扩展，筛选出叶片，并用judge（Mat）计算亮/总比例rate，选最小作为目标装甲板。

  找到目标装甲之后可以确定圆心，半径。



* 判断目标装甲：`cv::RotatedRect ArmorTrigger::select_tar()`

  先找装甲所属模块，然后判断模块是否被击打过，找到没被击打过的则为目标装甲。

  调用Number_armor，strike

  需要用到arr



* 找装甲所属模块：`int ArmorTrigger::Number_armor(cv::Point2f testp)`

  参考：https://www.cnblogs.com/miloyip/archive/2013/04/19/3029852.html

  需要用到marmors[i].appro，radius，Center，（即为模型建立完成）

  余弦定理

* 判断旋转方向：`void ArmorTrigger::Find_Something()`

  参考：https://blog.csdn.net/wangxianse/article/details/49642549?utm_source=blogxgwz4

  这个潘巨说要手动看旋转方向再传入。程序里这个会有偏差，但是计算出现次数多的为旋转方向刚开始有偏差但是后面就会基本准确。

* 判断击打：`int ArmorTrigger::judge(MArmor a)`

  参考：https://www.cnblogs.com/changxiang/p/7533294.html

  二值图像颜色面积

* 调整四个点：`std::vector<cv::Point2f>  ArmorTrigger::Adj()`

  用长和宽计算四个点坐标。

