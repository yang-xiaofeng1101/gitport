#include "ArmorTrigger.hpp"
using namespace std;

namespace wmj
{

void ArmorTrigger::reset()
{
    Center.x = 0;
    Center.y = 0;
    OKnum = 0;
    num = 0;
    towards = 0;
    local = false;
    wind.clear();
    aim.x = 0;
    aim.y = 0;
    AIM_old.x = 0;
    AIM_old.y = 0;
}

ArmorTrigger::ArmorTrigger(const boost::filesystem::path &ws_path)
{
    Center = cv::Point2f(0, 0);
    num = 0;
    cv::FileStorage Cameracfg((ws_path / "camera_chassis.yml").string(), cv::FileStorage::READ);
    cv::FileStorage Runecfg((ws_path / "main_param.yaml").string(), cv::FileStorage::READ);
    readCameraSetting(Cameracfg);
    readRuneSetting(Runecfg);
    Cameracfg.release();
    Runecfg.release();
}

void ArmorTrigger::readCameraSetting(cv::FileStorage &fs)
{
    fs["camera_matrix"] >> this->m_CameraMat;
    fs["distortion_corfficients"] >> this->m_DistMat;
}

void ArmorTrigger::readRuneSetting(cv::FileStorage &fs)
{
    //int testnum;
    //fs["test"]["num"] >> testnum;
    //if(testnum == 3){
    //    std::cout << std::endl
    //        << std::endl
    //        << "test!" 
    //        << std::endl;
    //}

    fs["ArmorDetector"]["enemy_color"] >> this->rune_color;
    rune_color = !rune_color;
    fs["ArmorDetector"]["Rune_height"] >> (this->objarmor).obj_height;
    fs["ArmorDetector"]["Rune_width"] >> (this->objarmor).obj_width;

    if (rune_color == 0)
    {
        fs["ArmorDetector"]["thre_rd_r"] >> this->thred_rd;
        fs["ArmorDetector"]["thre_gray_r"] >> this->thred_gray;
    }
    else
    {
        fs["ArmorDetector"]["thre_rd_b"] >> this->thred_rd;
        fs["ArmorDetector"]["thre_gray_b"] >> this->thred_gray;
    }

    fs["ArmorDetector"]["angle_speed"] >> this->speed;

    fs["ArmorDetector"]["rune_max_area"] >> this->max_rune_area;
    fs["ArmorDetector"]["rune_min_area"] >> this->min_rune_area;
    fs["ArmorDetector"]["rune_max_rate"] >> this->max_rune_rate;
    fs["ArmorDetector"]["rune_min_rate"] >> this->min_rune_rate;

    fs["ArmorDetector"]["rune_max_center_area"] >> this->Rune_max_center_area;
    fs["ArmorDetector"]["rune_min_center_area"] >> this->Rune_min_center_area;
}

double ArmorTrigger::ArmorCut(cv::RotatedRect &rect)
{
#ifdef DEBUG
    printf("进入ArmorCUT\n");
#endif
    cv::Mat imageSource = And;
    // cv::imshow("Source Image", imageSource);
    std::vector<cv::Point> contour;

    cv::Point2f vertices[4];

    rect.points(vertices); //外接矩形的4个顶点

    cv::Point2f center = rect.center;                               //外接矩形中心点坐标
    cv::Mat rot_mat = getRotationMatrix2D(center, rect.angle, 1.0); //求旋转矩阵
                                                                    // std::cout<<center<<"\t"<<rect.angle<<endl;
    // printf("%f\t%f\t%f",center.x,center.y,rect.angle);
    cv::Mat rot_image;
    cv::Size dst_sz(imageSource.size());
    warpAffine(imageSource, rot_image, rot_mat, dst_sz); //原图像旋转
    // imshow("rot_image", rot_image);
    cv::Mat result1;
    if (center.x - rect.size.width * 0.35 > 0 && center.y - rect.size.height * 0.35 > 0 &&
        center.x - rect.size.width * 0.35 + rect.size.width * 0.7 < rot_image.cols &&
        center.y - rect.size.height * 0.35 + rect.size.height * 0.7 < rot_image.rows)
        result1 = rot_image(cv::Rect(center.x - rect.size.width * 0.35, center.y - rect.size.height * 0.35, rect.size.width * 0.7, rect.size.height * 0.7)); //提取ROI
    if (result1.empty())
        return -1;
    double rate = judge(result1);
    // cv::imshow("armor",result1);
    // cv::waitKey(0);
    return rate;
}
double ArmorTrigger::ROICut(cv::RotatedRect &rect, cv::Mat &store)
{
#ifdef DEBUG
    printf("进入ROICUT\n");
#endif
    cv::Mat imageSource = And;
    
    std::vector<cv::Point> contour;

    cv::Point2f vertices[4];

    rect.points(vertices); //外接矩形的4个顶点
    
    cv::Point2f center = rect.center;                               //外接矩形中心点坐标
    cv::Mat rot_mat = getRotationMatrix2D(center, rect.angle, 1.0); //求旋转矩阵
                                                                    // std::cout<<center<<"\t"<<rect.angle<<endl;
    // printf("%f\t%f\t%f",center.x,center.y,rect.angle);
    cv::Mat rot_image;
    cv::Size dst_sz(imageSource.size());
    warpAffine(imageSource, rot_image, rot_mat, dst_sz); //原图像旋转

    cv::Mat result1, result2, result3, result4;

    double W, H; // W > H
    double Max = -1;
    int maxi = -1;
    std::vector<cv::Mat> result;
    std::vector<double> rate;

    if (rect.size.width < rect.size.height)
    {
        W = rect.size.height;
        H = rect.size.width;
    }
    else
    {
        H = rect.size.height;
        W = rect.size.width;
    }
    if (center.x - (rect.size.width * 0.8) > 0 && center.y - (rect.size.height * 0.8) > 0 &&
        center.y - (rect.size.height * 0.8) + W * 1.8 < rot_image.rows &&
        center.x - (rect.size.width * 0.8) + H * 4.5 < rot_image.cols)
    {
        result1 = rot_image(cv::Rect(center.x - (rect.size.width * 0.8),
                                     center.y - (rect.size.height * 0.8), H * 4.5, W * 1.8)); //提取ROI
        rate.push_back(judge(result1));
        result.push_back(result1);
    }
    if (center.x - (rect.size.width * 3.5) > 0 && center.y - (rect.size.height * 0.8) > 0 &&
        center.x - (rect.size.width * 3.5) + H * 4.5 < rot_image.cols &&
        center.y - (rect.size.height * 0.8) + W * 1.8 < rot_image.rows)
    {
        result2 = rot_image(cv::Rect(center.x - (rect.size.width * 3.5),
                                     center.y - (rect.size.height * 0.8), H * 4.5, W * 1.8));
        rate.push_back(judge(result2));
        result.push_back(result2);
    }

    if (center.x - (rect.size.width * 0.8) > 0 && center.y - (rect.size.height * 0.8) > 0 &&
        center.x - (rect.size.width * 0.8) + W * 1.8 < rot_image.cols &&
        center.y - (rect.size.height * 0.8) + H * 4.5 < rot_image.rows)
    {
        result3 = rot_image(cv::Rect(center.x - (rect.size.width * 0.8),
                                     center.y - (rect.size.height * 0.8), W * 1.8, H * 4.5));
        rate.push_back(judge(result3));
        result.push_back(result3);
    }

    if (center.x - (rect.size.width * 0.8) > 0 && center.y - (rect.size.height * 3.5) > 0 &&
        center.x - (rect.size.width * 0.8) + W * 1.8 < rot_image.cols &&
        center.y - (rect.size.height * 3.5) + H * 4.5 < rot_image.rows)
    {
        result4 = rot_image(cv::Rect(center.x - (rect.size.width * 0.8),
                                     center.y - (rect.size.height * 3.5), W * 1.8, H * 4.5));
        rate.push_back(judge(result4));
        result.push_back(result4);
    }

#ifdef DEBUG
    imshow("rot_image", rot_image);
    cout << rect.size.height << "\t" << rect.size.width << endl;
    if (!result1.empty())
        imshow("result1", result1);
    if (!result2.empty())
        imshow("result2", result2);
    if (!result3.empty())
        imshow("result3", result3);
    if (!result4.empty())
        imshow("result4", result4);
    cv::waitKey(0);
#endif

    std::vector<double>::iterator maxgest = std::max_element(std::begin(rate), std::end(rate));
    
    if (!rate.empty())
        store = result[std::distance(std::begin(rate), maxgest)];
#ifdef DEBUG
    if (!rate.empty())
        printf("rate : %lf\n", *maxgest);
    else
    {
        printf("rate容器为空\n");
    }
    printf("离开ROICUT\n");
#endif
    if (!rate.empty())
        return *maxgest;
    else
        return -1;
}
void ArmorTrigger::preprocess()
{
#ifdef DEBUG
    printf("进入preprocess\n");
    printf("windarea.area=%d\t", windarea.area());
    cout << "windarea.tl = " << windarea.tl() << std::endl;
#endif

    cv::Mat img;
    if (local)
    {
        img = this->tostrike(windarea);
    }
    else
    {
        img = this->tostrike;
    }

    cv::Mat bw1, bw;
    cv::cvtColor(img, bw1, cv::COLOR_BGR2GRAY);
    //阈值处理

    std::vector<cv::Mat> imgchannels;
    cv::split(img, imgchannels);

    if (this->rune_color)
    {
        bw = imgchannels.at(0) - imgchannels.at(2);
    }
    else
    {
        bw = imgchannels.at(2) - imgchannels.at(0);
    }

    threshold(bw1, bw1, thred_gray, 255, CV_THRESH_BINARY);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    cv::Mat element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    dilate(bw1, bw1, element);
    erode(bw1, bw1, element1);

    cv::threshold(bw, bw, thred_rd, 255, CV_THRESH_BINARY);

    dilate(bw, bw, element);
    // erode(bw, bw, element1);

    bitwise_and(bw, bw1, And);
    // And=bw;
    dilate(And, And, element);
    cv::GaussianBlur(And, And, cv::Size(3, 3), 0);

#ifdef DEBUG
    cv::imshow("color", bw1);
    cv::imshow("r-d", bw);
    cv::imshow("and", And);
    cv::waitKey(1);
#endif
}

cv::Point3f ArmorTrigger::Find_TargetArmor(cv::Mat &src, int T)
{

    if (T == 1)
    {
        towards = 2;
    }
    else
    {
        towards = 1;
    }

    if (OKnum > 8 && local == false)
    {
        if (windarea.tl().x > 0 && windarea.tl().y > 0 && windarea.br().x < tostrike.cols && windarea.br().y < tostrike.rows)
            local = true;
        else
        {
            local = false;
        }
    }
    printf("\t\t\t\t\t\t\t local = %d\n", local);

    cv::Mat img = src;
    this->tostrike = src(cv::Rect(cv::Point2f(0, 0), cv::Point2f(src.cols, src.rows / 2)));
    double t1, t2, t5, t6;
    t1 = clock();

    //cv::copyMakeBorder(this->tostrike, this->tostrike, 50, 50, 50, 50, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    // this->tostrike = img;

    preprocess();
    if (!local)
    {
        cut();
    }

    //寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(And, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    // 轮廓分析，找到
    cv::Mat store;
    std::vector<double> rate;
    double ra;
    cv::RotatedRect re;

    t5 = clock();
    // printf("preprocess_time : %lfms\n", (t5 - t1) / 1000);

#ifdef DEBUG
    printf("new\n");
#endif

    struct vane win;

#ifdef DEBUG
    std::cout << "size=" << contours.size() << std::endl;
#endif

    for (size_t i = 0; i < contours.size(); ++i)
    {
        //计算轮廓大小
        double area = cv::contourArea(contours[i]);

#ifdef DEBUG
        printf("%lf\n", area);
#endif

        // 去除过小或者过大的轮廓区域（科学计数法表示）
        if (area < this->min_rune_area || area > this->max_rune_area)
            continue;

//绘制轮廓
// cv::drawContours(img, contours, i, CV_RGB(0, 255, 0), 2, 8, hierarchy, 0);
#ifdef DEBUG
        printf("1\n");
#endif

        // 获得轮廓的角度
        cv::Point2f *pos = new cv::Point2f();
        double dOrient = getOrientation(contours[i], *pos, img);
        //转换轮廓,并获得极值
        for (size_t j = 0; j < contours[i].size(); j++)
            contours[i][j] = GetPointAfterRotate(contours[i][j], (cv::Point)*pos, dOrient);
        cv::Rect rect = cv::boundingRect(contours[i]); //轮廓最小外接矩形

        cv::RotatedRect rotateRect = cv::RotatedRect((cv::Point2f)rect.tl(), cv::Point2f(rect.br().x, rect.tl().y), (cv::Point2f)rect.br());
        if (rotateRect.size.area() > max_rune_area + 50 || rotateRect.size.area() < min_rune_area + 50)
            continue;

        // printf("re.area/area= %lf\n",rotateRect.size.area()/area);
        // if(rotateRect.size.area()/area>1.5){
        //     continue;
        // }
        double size = rotateRect.size.width / rotateRect.size.height;
        if (size > max_rune_rate || size < min_rune_rate)
            continue;
        // if (size > 1.4 && area < 2e2)
        //     continue;
        double t7, t8;

        cv::RotatedRect rc = cv::minAreaRect(contours[i]);

        double rat;
        rat = 0;
        rat = ArmorCut(rc);
        if (rat > 0.3 || rat == -1)
            continue;
        // printf("light-rate: %lf\n",rat);

        if (local)
        {
            cv::putText(this->tostrike, std::to_string(size),
                        cv::Point2f(windarea.tl().x + rotateRect.center.x - 30, windarea.tl().y + rotateRect.center.y - 30),
                        cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 255, 255));

            cv::putText(this->tostrike, std::to_string(area),
                        cv::Point2f(windarea.tl().x + rotateRect.center.x, windarea.tl().y + rotateRect.center.y),
                        cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255));
        }
        else
        {
            cv::putText(this->tostrike, std::to_string(size),
                        cv::Point2f(rotateRect.center.x - 30, rotateRect.center.y - 30),
                        cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 255, 255));

            cv::putText(this->tostrike, std::to_string(area),
                        rotateRect.center, cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255));
        }

        //将角度转换回去并绘图
        cv::Point2f rect_points[4];
        rotateRect.points(rect_points);

        for (size_t j = 0; j < 4; j++)
            rect_points[j] = GetPointAfterRotate((cv::Point)rect_points[j], (cv::Point)*pos, -dOrient);

        cv::Point2f a1, a2;
        for (size_t j = 0; j < 4; j++)
        {
            if (local)
            {
                a1 = cv::Point2f(windarea.tl().x + rect_points[j].x, windarea.tl().y + rect_points[j].y);
                a2 = cv::Point2f(windarea.tl().x + rect_points[(j + 1) % 4].x, windarea.tl().y + rect_points[(j + 1) % 4].y);
            }
            else
            {
                a1 = cv::Point2f(rect_points[j].x, rect_points[j].y);
                a2 = cv::Point2f(rect_points[(j + 1) % 4].x, rect_points[(j + 1) % 4].y);
            }

            line(this->tostrike, a1, a2, cv::Scalar(0, 0, 255), 2);
        }

        std::vector<cv::Point2f> po;
        for (int i = 0; i < 4; i++)
        {
            po.push_back(rect_points[i]);
        }
#ifdef DEBUG
        printf("2\n");
#endif
        double t3, t4;
        t3 = clock();
        cv::RotatedRect re = minAreaRect(po);
        // cout<<"re.cen="<<re.center;
        if (re.size.area() > max_rune_area)
            continue;

        ra = ROICut(re, store);
        if (ra != -1)
            rate.push_back(ra);
        t4 = clock();
        // printf("ROICUT_time: %lf ms\n", (t4 - t3) / 1000);
#ifdef DEBUG
        printf("re.witdth=%lf,re.height=%lf\n", re.size.width, re.size.height);
        if (!store.empty())
        {
            imshow("store", store);
            cv::waitKey(1);
        }

        printf("3\n");
#endif
        win.rate = ra;
        win.rect = re;
        win.src = store;
        win.center = re.center;
        wind.push_back(win);

#ifdef DEBUG
        printf("4\n");
#endif
    }

    t6 = clock();
    // printf("Loop_time: %lf ms\n", (t6 - t5) / 1000);
#ifdef DEBUG
    printf("5\n");
#endif

    double min_rate, all_rate;

    all_rate = 0;
    min_rate = 9.0e4;
    int min_i;
    min_i = -1;
    for (int i = 0; i < wind.size(); i++)
    { //寻找明亮区域最小风车叶片
        all_rate += wind[i].rate;
        if (wind[i].rate < min_rate)
        {
            min_rate = wind[i].rate;
            min_i = i;
        }
    }

    if (min_i == -1)
    { //无叶片返回（-1，,1）
        // this->aim = cv::Point2f(-1, -1);
        // cv::imshow("res", this->tostrike);
        // cv::waitKey(1);
        wind.clear();
        OKnum = 0;
        return cv::Point3f(-1, -1, -1);
    }

    all_rate -= wind[min_i].rate;
    double rrr = wind[min_i].rate / (all_rate / (wind.size() - 1));

    num++;
    AIM = wind[min_i];

    if (rrr > 0.8 && rrr < 1.2 && wind.size() == 5)
    {
        //cv::imshow("res", this->tostrike);
        //cv::waitKey(1);
        wind.clear();
        return cv::Point3f(-8, -8, -8);
    }
    else if (rrr > 0.8 && rrr < 1.2)
    {
        guess();
    }

#ifdef DEBUG
    printf("\t\t\t\t\trrr=%lf\n", rrr);
    printf("min_rate: %lf", min_rate);
    printf("wind_rate: ");
    for (int i = 0; i < wind.size(); i++)
    {
        printf("1:%lf ", wind[i].rate);
    }
    printf("wind.size: %d\n", wind.size());
    printf("6\n");
#endif

    if (local)
    {
        cv::circle(this->tostrike, cv::Point2f(AIM.center + cv::Point2f(windarea.tl())), 0, cv::Scalar(255, 255, 0), 12); //绘制目标中心
    }
    else
    {
        cv::circle(this->tostrike, AIM.center, 0, cv::Scalar(255, 255, 0), 12); //绘制目标中心
    }

    cv::Point2f rect_points[4];
    AIM.rect.points(rect_points);
    double width = GetDis(rect_points[0], rect_points[1]);
    double height = GetDis(rect_points[2], rect_points[1]);
    if (width < height)
        std::swap(width, height);
    arr_w = width;
    arr_h = height;

    if ((num % 4 == 0 || num < 15))
        make_circle(contours);

    cv::circle(this->tostrike, Center, 0, cv::Scalar(255, 0, 0), 20); //绘制圆心

    //PNP解算所需工作
    predict();
    cv::circle(this->tostrike, this->aim, 0, cv::Scalar(255, 255, 255), 10); //绘制预测目标中心
    this->AIM_old = this->AIM.center;

    Adj();

#ifdef DEBUG
    printf("7\n");
#endif

    cv::Point3f res = CalcArmorDeepth();

#ifdef DEBUG
    printf("10\n");
    std::cout << "objet point : " << res << std::endl;
#endif

    ipoints_v.clear();

    //图像窗口
    //cv::imshow("res", this->tostrike);
    //cv::waitKey(1);

    t2 = clock();
    // printf("later_time = %lf\n", (t2 - t6) / 1000);
    // printf("total time = %lf\n", (t2 - t1) / 1000);
    total_time = (t2 - t1) / 1000;
    t1 = t2;

    wind.clear();
    // return cv::Point3f(-8,-8,-8);
    //记录局部查找成功次数
    OKnum++;
    if (local)
        local_ok++;
    
    // if (num < 30)
    if (local == false)
        windarea = cv::Rect((int)(Center.x - 1.5 * radius),
                            (int)(Center.y - 1.5 * radius),
                            (int)(radius * 3),
                            (int)(radius * 3));

    return res; //返回装甲板中心世界坐标

    // printf("\n");
}

void ArmorTrigger::cut()
{
    cv::Mat image = And;
    int colornum = 0;
    double rate;

    int nr = image.rows;
    int nc = image.cols;

    for (int i = 0; i < nr; i++)
    {
        for (int j = 0; j < nc/5; j++)
        {
            image.at<uchar>(i, j) = 0;
        }
    }
    for (int i = 0; i < nr; i++)
    {
        for (int j = (nc/5)*4; j < nc; j++)
        {
            image.at<uchar>(i, j) = 0;
        }
    }

}
double ArmorTrigger::judge(cv::Mat &src)
{
#ifdef DEBUG
    printf("进入judge\n");
#endif

    cv::Mat image = src;
    int colornum = 0;
    double rate;

    int nr = image.rows;
    int nc = image.cols;

    if (image.isContinuous())
    {
        std::cout << "quickly" << std::endl;
        nr = 1;
        nc = nc * image.rows * image.channels();
    }
    for (int i = 0; i < nr; i++)
    {
        const uchar *inData = image.ptr<uchar>(i);
        for (int j = 0; j < nc; j++)
        {
            // if (*inData >= 200)
            // {
            //     colornum++;
            // }
            // printf("%d ",(*inData)%200);
            colornum += (*inData / 200);
            inData++;
        }
    }
    // for (int i = 0; i < image.rows; i++)
    // {
    //     for (int j = 0; j < image.cols; j++)
    //     {
    //         if (image.at<uchar>(i, j) >= 200)
    //             colornum++;
    //     }
    // }

    rate = (double)colornum / ((double)image.cols * (double)image.rows);
    // printf("rate : %lf\n", rate);
    // arr_rate.push_back(rate);
    // cv::putText(tostrike, std::to_string(rate), a.appro, cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 255, 0));
    return rate;
}

void ArmorTrigger::calculate(cv::Point2f &center, cv::Point2f &p_, cv::Point2f &p, double angle)
{
    double rad = angle * 3.1415926f / 180.0;
    p.x = (p_.x - center.x) * cos(-rad) - (p_.y - center.y) * sin(-rad) + center.x;
    p.y = (p_.y - center.x) * sin(-rad) + (p_.y - center.y) * cos(-rad) + center.y;
}

cv::Point2f ArmorTrigger::GetPointAfterRotate(cv::Point2f inputpoint, cv::Point2f center, double angle)
{
    cv::Point2d preturn;
    preturn.x = (inputpoint.x - center.x) * cos(-angle) - (inputpoint.y - center.y) * sin(-angle) + center.x;
    preturn.y = (inputpoint.x - center.x) * sin(-angle) + (inputpoint.y - center.y) * cos(-angle) + center.y;
    return preturn;
}

cv::Point ArmorTrigger::GetPointAfterRotate(cv::Point inputpoint, cv::Point center, double angle)
{
    cv::Point preturn;
    preturn.x = (inputpoint.x - center.x) * cos(-1 * angle) - (inputpoint.y - center.y) * sin(-1 * angle) + center.x;
    preturn.y = (inputpoint.x - center.x) * sin(-1 * angle) + (inputpoint.y - center.y) * cos(-1 * angle) + center.y;
    return preturn;
}

double ArmorTrigger::getOrientation(std::vector<cv::Point> &pts, cv::Point2f &pos, cv::Mat &img)
{
    //Construct a buffer used by the pca analysis
    cv::Mat data_pts = cv::Mat(pts.size(), 2, CV_64FC1);
    for (int i = 0; i < data_pts.rows; ++i)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }
    //Perform PCA analysis
    cv::PCA pca_analysis(data_pts, cv::Mat(), CV_PCA_DATA_AS_ROW);
    //Store the position of the object
    pos = cv::Point2f(pca_analysis.mean.at<double>(0, 0),
                      pca_analysis.mean.at<double>(0, 1));
    //Store the eigenvalues and eigenvectors
    std::vector<cv::Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);
    for (int i = 0; i < 2; ++i)
    {
        eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                    pca_analysis.eigenvectors.at<double>(i, 1));
        eigen_val[i] = pca_analysis.eigenvalues.at<double>(i, 0);
    }
    // Draw the principal components
    //在轮廓/图像中点绘制小圆
    //circle(img, pos, 3, CV_RGB(255, 0, 255), 2);
    ////计算出直线，在主要方向上绘制直线
    //line(img, pos, pos + 0.02 * Point2f(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]) , CV_RGB(255, 255, 0));
    //line(img, pos, pos + 0.02 * Point2f(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]) , CV_RGB(0, 255, 255));
    return atan2(eigen_vecs[0].y, eigen_vecs[0].x);
}

double ArmorTrigger::GetDis(cv::Point2f &a, cv::Point2f &b)
{
    double d = powf(fabs(a.x - b.x), 2) + powf(fabs(a.y - b.y), 2);
    return sqrtf(d);
}

std::vector<cv::Point2f> ArmorTrigger::Adj()
{
    ipoints_v.push_back(cv::Point2f(aim.x - arr_w / 2, aim.y - arr_h / 2));
    ipoints_v.push_back(cv::Point2f(aim.x - arr_w / 2, aim.y + arr_h / 2));
    ipoints_v.push_back(cv::Point2f(aim.x + arr_w / 2, aim.y + arr_h / 2));
    ipoints_v.push_back(cv::Point2f(aim.x + arr_w / 2, aim.y - arr_h / 2));
    return ipoints_v;
}

cv::Point3f ArmorTrigger::CalcArmorDeepth()
{
#ifdef DEBUG
    printf("8\n");
#endif

    std::vector<cv::Point3f> op_v;
    GetObjPoints(objarmor.obj_width, objarmor.obj_height, op_v);

#ifdef DEBUG
    printf("9\n");
#endif
    cv::Mat opoints(op_v);

    cv::Mat rvec, tvec;
    bool ret = cv::solvePnP(opoints, cv::Mat(ipoints_v), m_CameraMat, m_DistMat, rvec, tvec);

#ifdef DEBUG
    // std::cout << "rvec: " << rvec << std::endl;
    // std::cout << "tvec: " << tvec << std::endl;
    std::cout << tvec.at<double>(2, 0) * cos(rvec.at<double>(1, 0 / 2 / CV_PI)) << std::endl;

#endif
    if (ret)
        return cv::Point3f(tvec.at<double>(2, 0), -tvec.at<double>(0, 0), -tvec.at<double>(1, 0));
    else
        return cv::Point3f(500.f, 500.f, 500.f);
}

void ArmorTrigger::GetObjPoints(double _width, double _height, std::vector<cv::Point3f> &op_v)
{
    double width = _width * 1., height = _height * 1.;
    op_v.push_back(cv::Point3f(-width / 2.f, -height / 2, 0.0));
    op_v.push_back(cv::Point3f(-width / 2.f, height / 2, 0.0));
    op_v.push_back(cv::Point3f(width / 2.f, height / 2, 0.0));
    op_v.push_back(cv::Point3f(width / 2.f, -height / 2, 0.0));
}
void ArmorTrigger ::make_circle(std::vector<std::vector<cv::Point>> &contours)
{
    printf("进入make_circle\n");

    double len;
    cv::Point2f Center1, Center2;

    if (!local)
    {
        double K;
        double coss, sinn;

        len = std::max(AIM.rect.size.width, AIM.rect.size.height) * 4.5;
        cv::Point2f poi[4];
        cv::Point2f a, b, c;
        AIM.rect.points(poi);
        a = poi[0];
        b = poi[1];
        c = poi[2];
        if (GetDis(a, b) < GetDis(b, c))
        {
            K = (a.y - b.y) / (a.x - b.x);
            coss = (a.x - b.x) / sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
            sinn = (a.y - b.y) / sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        }
        else
        {
            K = (c.y - b.y) / (c.x - b.x);
            coss = (c.x - b.x) / sqrt((c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y));
            sinn = (c.y - b.y) / sqrt((c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y));
        }
        if (K != 0)
        {
            Center1 = cv::Point2f(AIM.center.x + len * coss, AIM.center.y + len * sinn);
            Center2 = cv::Point2f(AIM.center.x - len * coss, AIM.center.y - len * sinn);
        }
        else
        {
            Center1 = cv::Point2f(AIM.center.x, AIM.center.y + len);
            Center2 = cv::Point2f(AIM.center.x, AIM.center.y - len);
        }
        cv::circle(tostrike, Center1, 0, cv::Scalar(0, 255, 255), 15);   //绘制目标中心
        cv::circle(tostrike, Center2, 0, cv::Scalar(255, 255, 255), 15); //绘制目标中心
        // std::cout << "Center1=" << Center1 << "\t";
        // std::cout << "Center2=" << Center2 << std::endl;

        center_cal.push_back(Center);
        cv::Mat s1, s2;
        if (Center1.x - len / 3 > 0 && Center1.y - len / 3 > 0 &&
            Center1.x + len * 1.0 / 3 < And.cols &&
            Center1.y + len * 1.0 / 3 < And.rows)
        {
            s1 = And(cv::Rect(Center1.x - len / 3, Center1.y - len / 3, len * 2.0 / 3, len * 2.0 / 3));
        }
        if (Center2.x - len / 3 > 0 && Center2.y - len / 3 > 0 &&
            Center2.x + len * 1.0 / 3 < And.cols &&
            Center2.y + len * 1.0 / 3 < And.rows)
        {
            s2 = And(cv::Rect(Center2.x - len / 3, Center2.y - len / 3, len * 2.0 / 3, len * 2.0 / 3));
        }
        // cv::imshow("tos",tostrike);
        // cv::imshow("s1", s1);
        // cv::imshow("s2", s2);

        if (s1.empty() || s2.empty())
        {
            if (s1.empty())
            {
                s1 = s2;
                Center1 = Center2;
            }
        }
        if (!s1.empty() && !s2.empty())
        {
            if (judge(s2) > judge(s1))
            {
                s1 = s2;
                Center1 = Center2;
            }
        }
    }

    double w, h;
    double rat;
    vector<double> le;

    // printf("contours.size()=%d\n",contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        //计算轮廓大小
        double area = cv::contourArea(contours[i]);
        printf("area=%lf\n", area);

        // 去除过小或者过大的轮廓区域（科学计数法表示）

        if (area < Rune_min_center_area || area > Rune_max_center_area)
            continue;

        cv::RotatedRect rc = cv::minAreaRect(contours[i]);
        if (rc.size.area() > Rune_max_center_area + 200)
            continue;
        w = max(rc.size.width, rc.size.height);
        h = min(rc.size.width, rc.size.height);
        if (w / h > 1.4)
            continue;
        printf("w/h=%lf\n", w / h);
        rat = -1;
        rat = ArmorCut(rc);
        if (rat < 0.6)
            continue;

        // std::cout << "dcen = " << GetDis(Center, rc.center) << std::endl;

        if (Center == cv::Point2f(0, 0))
        {
            Center = rc.center;
            this->radius = GetDis(Center, AIM.center);
            if (local)
            {
                Center += cv::Point2f(windarea.tl());
            }
            break;
        }
        // if(GetDis(Center,rc.center)>10)continue;
        double r1;
        r1 = GetDis(AIM.center, rc.center);
        // printf("\t\t\t\t\t\t\t\tr/r1= %lf\n", radius / r1);

        // if ((radius / r1 > 1.2 || radius / r1 < 0.8) && radius != 0)
        //     continue;

        for (int i = 0; i < wind.size(); i++)
        {
            le.push_back(GetDis(wind[i].center, rc.center));
        }

        //圆心到各装甲板中心的距离最大与最小的比值<1.1
        std::vector<double>::iterator maxgest = std::max_element(std::begin(le), std::end(le));
        std::vector<double>::iterator mingest = std::min_element(std::begin(le), std::end(le));

        printf("\t\t\t\t*maxgest) / (*mingest) = %lf\n", *maxgest / (*mingest));
        if ((*maxgest) / (*mingest) < 1.2)
        {
            if (!local)
            {
                if (GetDis(rc.center, Center1) > len / 2)
                    continue;
            }

            Center = rc.center;
            this->radius = GetDis(Center, AIM.center);
            if (local)
            {
                Center += cv::Point2f(windarea.tl());
            }
        }

        le.clear();
    }

    // if (num < 15 && Center != cv::Point2f(0, 0));
    //
}
void ArmorTrigger::predict()
{
    double speed_angle;
    speed_angle = speed;

    double coss, sinn, angle1, angle2, angle;
    cv::Point2f cen;

    if (local)
    {
        cen = Center - cv::Point2f(windarea.tl());
    }
    else
    {
        cen = Center;
    }

    coss = (AIM.center.x - cen.x) /
           sqrt((AIM.center.x - cen.x) * (AIM.center.x - cen.x) +
                (AIM.center.y - cen.y) * (AIM.center.y - cen.y));

    sinn = (AIM.center.y - cen.y) /
           sqrt((AIM.center.x - cen.x) * (AIM.center.x - cen.x) +
                (AIM.center.y - cen.y) * (AIM.center.y - cen.y));

    angle1 = asin(sinn);
    angle2 = acos(coss);
    // printf("\t\t\t\t\t\tangle1=%lf度\n\t\t\t\t\t\tangle2=%lf度\n", angle1 * 180 / PI, angle2 * 180 / PI);
    if (angle1 > 0 && angle2 < PI / 2 && angle1 < PI / 2)
        angle = angle1;
    else if (angle1 < 0 && angle2 < PI / 2)
        angle = angle1;
    else if (angle1 < PI / 2 && angle1 > 0 && angle2 > PI / 2)
        angle = angle2;
    else
    {
        angle = -angle1 - PI;
    }
    if (towards == 1)
        angle -= speed_angle;
    else
    {
        angle += speed_angle;
    }

    aim.x = Center.x + radius * 1.05 * cos(angle);
    aim.y = Center.y + radius * 1.05 * sin(angle);
}

void ArmorTrigger::guess()
{

    double xx;
    double yy;

#ifdef DEBUG
    cout << "guess: \tAIM_old = " << AIM_old << "\t aim_old = " << aim;
#endif

    cv::Point2f aimlocal;

    if (local)
    {
        aimlocal = aim - cv::Point2f(windarea.tl());
    }
    else
    {
        aimlocal = aim;
    }

    xx = aimlocal.x - AIM_old.x;
    yy = aimlocal.y - AIM_old.y;

    if (local)
    {
        AIM.center = cv::Point2f(AIM_old.x + xx / 20, AIM_old.y + yy / 20);
    }
    else
    {
        AIM.center = cv::Point2f(AIM_old.x + xx / 20, AIM_old.y + yy / 20);
    }

#ifdef DEBUG
    cout << "\t AIM_new = " << AIM.center << std::endl;
#endif
}
} // namespace wmj
