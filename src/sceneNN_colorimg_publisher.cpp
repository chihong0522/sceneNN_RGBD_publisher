#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
//#include "DLib/FileFunctions.h"

using namespace std;

int main(int argc, char** argv)
{
    string TEST_COLOR_IMG_TOPIC = "/camera/color/image_raw";

    ros::init(argc, argv, "test_img_publisher");
    if(argc!=2) {
        cout << "rosrun image_transport_tutorial sceneNN_colorimg_publisher <folder name>" << endl;
        return 0;
    }

    cout << "Read image folder path " << argv[1] << endl;
    typedef vector<path> vec;             // store paths,
    vec colorImagePaths;

    copy(directory_iterator(argv[1]), directory_iterator(), back_inserter(colorImagePaths));
    sort(colorImagePaths.begin(), colorImagePaths.end());

    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise(TEST_COLOR_IMG_TOPIC, 1000);
    ros::Rate loop_rate(15);

    for (vec::const_iterator it(colorImagePaths.begin()), it_end(colorImagePaths.end()); it != it_end; ++it)
    {
        string readPath = it->string();
        cv::Mat image = cv::imread(readPath, cv::IMREAD_COLOR);
        std_msgs::Header fakeHeader = std_msgs::Header();
        fakeHeader.frame_id = "camera_link";
        fakeHeader.stamp = ros::Time::now();
        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(fakeHeader, "bgr8", image).toImageMsg();
        if (nh.ok()){
            pub.publish(msg);
            cout << "Publish " << readPath << " to " << TEST_COLOR_IMG_TOPIC << endl;
        }
        usleep(100*1000*1.5/1.13);
    }

//    while (nh.ok()) {
//    pub.publish(msg);
//    ros::spinOnce();
//    loop_rate.sleep();
//    }
}

