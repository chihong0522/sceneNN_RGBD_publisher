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
    string TEST_COLOR_IMG_TOPIC = "/test/camera/color/image_raw";
    string TEST_DEPTH_IMG_TOPIC = "/test/camera/color/image_raw";

    ros::init(argc, argv, "image_publisher");
    if(argc==0) {
        cout << "<folder name>" << endl;
        return 0;
    }

    cout << "Read image folder path " << argv[1] << endl;
    typedef vector<path> vec;             // store paths,
    vec colorImagePaths;
    vec depthImagePaths;

    copy(directory_iterator(argv[1]), directory_iterator(), back_inserter(colorImagePaths));
    sort(colorImagePaths.begin(), colorImagePaths.end());

    copy(directory_iterator(argv[2]), directory_iterator(), back_inserter(depthImagePaths));
    sort(depthImagePaths.begin(), depthImagePaths.end());

    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise(TEST_COLOR_IMG_TOPIC, 1);
    ros::Rate loop_rate(30);

    for (vec::const_iterator it(colorImagePaths.begin()), it_end(colorImagePaths.end()); it != it_end; ++it)
    {
        string readPath = it->string();
        cv::Mat image = cv::imread(readPath, cv::IMREAD_COLOR);
        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
        if (nh.ok()){
            pub.publish(msg);
            cout << "Publish " << readPath << " to " << TEST_COLOR_IMG_TOPIC << endl;
        }
    }

//    while (nh.ok()) {
//    pub.publish(msg);
//    ros::spinOnce();
//    loop_rate.sleep();
//    }
}

