#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <boost/filesystem.hpp>
#include <regex>

using namespace boost::filesystem;
//#include "DLib/FileFunctions.h"

using namespace std;

int main(int argc, char** argv)
{
    string TEST_DEPTH_IMG_TOPIC = "/camera/aligned_depth_to_color/image_raw";
    string TEST_COLOR_IMG_TOPIC = "/camera/color/image_raw";

    ros::init(argc, argv, "sceneNN_seq_publisher");
    if(argc!=2) {
        cout << "rosrun image_transport_tutorial sceneNN_seq_publisher <color image folder name>" << endl;
        return 0;
    }

    cout << "Read image folder path " << argv[1] << endl;
    typedef vector<path> vec;
    vec depthImagePaths;

    copy(directory_iterator(argv[1]), directory_iterator(), back_inserter(depthImagePaths));
    sort(depthImagePaths.begin(), depthImagePaths.end());

    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pubColor = it.advertise(TEST_COLOR_IMG_TOPIC, 1000);
    image_transport::Publisher pubDepth = it.advertise(TEST_DEPTH_IMG_TOPIC, 1000);
    ros::Rate loop_rate(30);

    for (vec::const_iterator it(depthImagePaths.begin()), it_end(depthImagePaths.end()); it != it_end; ++it)
    {
        string readColorPath = it->string();
        string readDepthPath = std::regex_replace(readColorPath, std::regex("image"), "depth");

        cv::Mat colorImage = cv::imread(readColorPath, cv::IMREAD_COLOR);
        cv::Mat depthImage = cv::imread(readDepthPath, cv::IMREAD_ANYDEPTH);

        std_msgs::Header fakeHeader = std_msgs::Header();
        fakeHeader.frame_id = "camera_link";
        fakeHeader.stamp = ros::Time::now();
        sensor_msgs::ImagePtr depthMsg = cv_bridge::CvImage(fakeHeader, "16UC1", depthImage).toImageMsg();
        sensor_msgs::ImagePtr colorMsg = cv_bridge::CvImage(fakeHeader, "bgr8", colorImage).toImageMsg();
        if (nh.ok()){
            pubColor.publish(colorMsg);
            pubDepth.publish(depthMsg);
        }
        usleep(115*1000);
    }

//    while (nh.ok()) {
//    pub.publish(msg);
//    ros::spinOnce();
//    loop_rate.sleep();
//    }
}

