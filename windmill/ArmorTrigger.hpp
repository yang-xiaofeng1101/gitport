#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/ml.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "usb_capture_with_thread.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <cmath>


// #define DEBUG
// #define RMVIDEO
// #define BLUE
#define PI 3.141592654
namespace wmj
{
struct vane
{
    cv::Mat src;
    double rate;
    cv::Point2f center;
    cv::RotatedRect rect;
};

struct ObjectArmor
{
    double obj_width;
    double obj_height;
};

class ArmorTrigger
{
  public:
    int64 num;
    std::vector<int>dirbuffer;
    int OKnum=0;
    cv::Point2f Center;
    std::vector<cv::Point2f>center_cal;
    std::vector<vane> wind; //每个风车叶片

    double arr_h; //目标装甲板图中的像素宽和高
    double arr_w;

    ObjectArmor objarmor;   //装甲板物理尺寸
    vane AIM; //目标风车叶片

    double radius;
    int towards;
    cv::Mat tostrike; // 未处理图像
    cv::Mat And;      // 已处理图像
    cv::Rect windarea;
    bool local=0;
	  bool local_old=0;
    int local_ok=0;

    bool rune_color;

    cv::Point2f aim;
    cv::Point2f AIM_old;

    cv::RotatedRect tar;
    cv::Point2f firstarmor;

    ArmorTrigger(const boost::filesystem::path &ws_path);
    void preprocess();
    void reset();
    cv::Point3f Find_TargetArmor(cv::Mat &tostrike,int T);
    void predict();
    void guess();


    cv::Point3f CalcArmorDeepth();

  private:
    double total_time;
    cv::Mat m_CameraMat, m_DistMat;
    std::vector<cv::Point2f>Center_old;
    std::vector<cv::Point2f> ipoints_v;
    
    int thred_rd,thred_gray;  //二值化的阈值
    double speed;
	double max_rune_rate,min_rune_rate,max_rune_area,min_rune_area;
    double Rune_max_center_area,Rune_min_center_area;

    void ReadFrame(cv::Mat &);
    void cut();
    void readCameraSetting(cv::FileStorage &fs);
    void readRuneSetting(cv::FileStorage &fs);
    void make_circle(std::vector<std::vector<cv::Point>> &);
    double GetDis(cv::Point2f &a, cv::Point2f &b);
    std::vector<cv::Point2f> Adj();
    std::vector<cv::Point2f> cmp();
    void GetObjPoints(double _width, double _height, std::vector<cv::Point3f> &op_v);

    std::vector<cv::RotatedRect> arr; //存储多个目标rotatedrect

    void calculate(cv::Point2f &center, cv::Point2f &p_, cv::Point2f &p, double angle);
    double getOrientation(std::vector<cv::Point> &pts, cv::Point2f &pos, cv::Mat &img);
    cv::Point GetPointAfterRotate(cv::Point inputpoint, cv::Point center, double angle);
    cv::Point2f GetPointAfterRotate(cv::Point2f inputpoint, cv::Point2f center, double angle);
    bool IsPointInCircularSector(cv::Point2f testp);

    double ROICut(cv::RotatedRect &rect, cv::Mat &store); //裁剪风车叶片
    double ArmorCut(cv::RotatedRect &tect);               //裁剪装甲板区域
    double judge(cv::Mat &);                              //计算 亮/暗 并返回比值
};
}
