#pragma once

#ifdef ETH_HAILO_USE

#define ETH_HAILO_INTERFACE_NAME "eth1"
#define SCAN_TIMEOUT_MILLISECONDS (2000)
#define MAX_NUM_OF_DEVICES (5)


#define HEF_FILE ("./coco_nonms_eth_nonms_eth_hailo0913.hef")

#define YOLOV5M_IMAGE_WIDTH 512
#define YOLOV5M_IMAGE_HEIGHT 512


#else


#define HEF_FILE ("./yolov5m_wo_spp_60p.hef")

#define YOLOV5M_IMAGE_WIDTH 640
#define YOLOV5M_IMAGE_HEIGHT 640


#endif


#define CONFIG_FILE ("./yolov5.json")
#define INPUT_VIDEO_FILE "detection.mp4"