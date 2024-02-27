#pragma once

#include <string>
#include <list>
#include <functional> //iterator header
#include "interfaces/json/JSONBase.h"

#define EC_NO_JSON_OBJ  -1
#define EC_NO_DATA      0
#define EC_OK           1

#define ACCELERATOR_TYPE_HAILO8 "hailo8"
#define ACCELERATOR_TYPE_DEFAULT ACCELERATOR_TYPE_HAILO8

#define ITERFACE_TYPE_ETHERNET "ethernet"
#define INTERFACE_TYPE_PCIE "pcie"
#define INTERFACE_TYPE_DEFULT ITERFACE_TYPE_ETHERNET

#define INTERFACE_NAME_DEFULT "eth1"

#define INTERFACE_IP_DEFULT "192.168.30.11"

#define ACCELERATOR_SCAN_TIMEOUT_DEFAULT (2000)
#define ACCELERATOR_SCAN_MAX_DEVICE_DEFAULT (5)
#define ACCELERATOR_INPUT_INPUT_COUNT_DEFAULT (1)
#define ACCELERATOR_INPUT_OUTPUT_COUNT_DEFAULT (1)

#define WIDTH 0
#define HEIGHT 1
// #define STRAIGHT 0
// #define TURN 1
// #define GOING_DIRECTION 0
// #define GOING_OPPOSIDE_DIRECTION 1
#define INFERENCE_IMAGE_DEFAULT_WIDTH 640
#define INFERENCE_IMAGE_DEFAULT_HEIGHT 640

#define INFERENCE_CONFIDENCE_DEFULT (0.8)

#define INFERNCE_MAX_BOX_COUNT (50)

#define INFERNCE_MAX_INPUT_COUNT (1)
#define INFERNCE_MAX_OUTPUT_COUNT (3)

#define MODEL_PATH_DEFAULT "~/.zaiv/models"
#define TARGET_MODEL_PATH_DEFAULT "~/.zaiv/target_model"

#define CLASS_SHOW_DEFAULT (1)

#define MSG_MODULE_TYPE_ZAIV "zaiv"
#define MSG_MODULE_TYPE_VIDEO "video"
#define MSG_MODULE_TYPE_ACTION "action"

#define MSG_METHOD_TYPE_POST "post"
#define MSG_METHOD_TYPE_GET "get"
#define MSG_METHOD_TYPE_PUSH "push"

#define MSG_COMMAND_TYPE_CONFIG "config"
#define MSG_COMMAND_TYPE_START "start"
#define MSG_COMMAND_TYPE_STOP "stop"
#define MSG_COMMAND_TYPE_SUBSCRIBE "subscribe"
#define MSG_COMMAND_TYPE_UNSUBSCRIBE "unsubscribe"
#define MSG_COMMAND_TYPE_STATUS "status"
#define MSG_COMMAND_TYPE_ON "on"
#define MSG_COMMAND_TYPE_OFF "off"
#define MSG_COMMAND_TYPE_PLAY "play"
#define MSG_COMMAND_TYPE_MOVE "move"

#define MSG_ATTRIBUTE_TYPE_CLASS "class"
#define MSG_ATTRIBUTE_TYPE_CONFIDENCE "confidence"
#define MSG_ATTRIBUTE_TYPE_HEF "hef"
#define MSG_ATTRIBUTE_TYPE_INTERFACE "interface"
#define MSG_ATTRIBUTE_TYPE_INFERENCE "inference"
#define MSG_ATTRIBUTE_TYPE_INFER_METADATA "infer-metadata"
#define MSG_ATTRIBUTE_TYPE_STREAMING "streming"
#define MSG_ATTRIBUTE_TYPE_CAM_OPTIONS "cam-options"
#define MSG_ATTRIBUTE_TYPE_CAM_CAPABILITY "cam-capability"
#define MSG_ATTRIBUTE_TYPE_SWITCH "switch"
#define MSG_ATTRIBUTE_TYPE_AUDIO "audio"
#define MSG_ATTRIBUTE_TYPE_CAR "car"

class Classes
{
public:
    Classes();
    Classes(int i, std::string n, bool s);
    virtual ~Classes();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    int index;
    std::string name;
    bool show;
};

class Controls
{
public:
    Controls();
    Controls(int rc, int p_s, int p_t_gd, int p_t_god, float gd_g, float gd_o, float god_g, float god_o);
    virtual ~Controls();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    int ref_center;
    int pwm_stratight_gd;
    int pwm_stratight_god;
    int pwm_turn_gd;
    int pwm_turn_god;
    float cal_gd_gain;
    float cal_gd_offset;
    float cal_god_gain;
    float cal_god_offset;
};

class Model
{
public:
    Model();
    virtual ~Model();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    std::string model_path;
    std::string target_model_path;
    int input_count;
    int output_count;
    std::list<Classes*> classes;
};

class Colors
{
public:
    Colors();
    Colors(std::string n, int r, int g, int b);
    virtual ~Colors();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
    std::string name;
    int code_rgb[3];
};

class Inference
{
public:
    Inference();
    virtual ~Inference();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    std::string model;
    int max_boxes;
    float confidence;
    int image_size[2];
    std::list<Colors*> colors;
};

class Accelerator
{
public:
    Accelerator();
    virtual ~Accelerator();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    std::string type;
    std::string interface_type;
    std::string interface_name;
    std::string ip;
};

class AI
{
public:
    AI();
    virtual ~AI();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    Accelerator accelerator;
    Inference inference;
    Model model;
};

class Car
{
public:
    Car();
    virtual ~Car();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    std::string device_path;
    Controls controls;
};

class Extentions
{
public:
    Extentions();
    virtual ~Extentions();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
    Car car;
};

class Websocket
{
public:
    Websocket();
    virtual ~Websocket();

    void Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
    int server_port;
};

class ZaivCfg : public JSONBase
{
public:
    ZaivCfg();
    virtual ~ZaivCfg();

    bool Deserialize(const std::string &s);
    bool Deserialize(const rapidjson::Value &obj);
    bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;

    // private:
    Extentions m_extentions;
    Websocket m_wsServer;
    AI m_Ai;
};