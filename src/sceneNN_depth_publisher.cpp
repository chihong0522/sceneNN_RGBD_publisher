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
    string TEST_DEPTH_IMG_TOPIC = "/test/camera/depth/image_raw";

    ros::init(argc, argv, "test_depth_publisher");
    if(argc!=2) {
        cout << "rosrun image_transport_tutorial sceneNN_depth_publisher <folder name>" << endl;
        return 0;
    }

    cout << "Read image folder path " << argv[1] << endl;
    typedef vector<path> vec;
    vec depthImagePaths;

    copy(directory_iterator(argv[1]), directory_iterator(), back_inserter(depthImagePaths));
    sort(depthImagePaths.begin(), depthImagePaths.end());

    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise(TEST_DEPTH_IMG_TOPIC, 1);
    ros::Rate loop_rate(15);

    for (vec::const_iterator it(depthImagePaths.begin()), it_end(depthImagePaths.end()); it != it_end; ++it)
    {
        string readPath = it->string();
        cv::Mat image = cv::imread(readPath, cv::IMREAD_ANYDEPTH);
        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "16UC1", image).toImageMsg();
        if (nh.ok()){
            pub.publish(msg);
            cout << "Publish " << readPath << " to " << TEST_DEPTH_IMG_TOPIC << endl;
        }
        usleep(100);
    }

//    while (nh.ok()) {
//    pub.publish(msg);
//    ros::spinOnce();
//    loop_rate.sleep();
//    }
}

