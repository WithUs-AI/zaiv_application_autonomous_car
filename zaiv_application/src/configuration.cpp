
#include <iostream>
#include "configuration.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////
Classes::Classes()
{
    index = 0;
    name = "";
    show = CLASS_SHOW_DEFAULT;
}
Classes::Classes(int i, std::string n, bool s)
{
    index = i;
    name = n;
    show = s;
}
Classes::~Classes()
{

}

void Classes::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();
    writer->String("index");
    writer->Int(index);
    writer->String("name");
    writer->String(name.c_str(), static_cast<rapidjson::SizeType>(name.length()));
    writer->String("show");
    writer->Bool(show);
    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Controls
/////////////////////////////////////////////////////////////////////////////////////////////////
Controls::Controls()
{
    ref_center = 400;
    pwm_stratight_gd = pwm_stratight_god = 70;
    pwm_turn_gd = 70;
    pwm_turn_god = 250;
    cal_gd_gain = 1.0;
    cal_gd_offset = 0.0;
    cal_god_gain = 1.0;
    cal_god_offset = 0.0;
}
Controls::Controls(int rc, int p_s, int p_t_gd, int p_t_god, float gd_g, float gd_o, float god_g, float god_o)
{
    ref_center = rc;
    pwm_stratight_gd = p_s;
    pwm_stratight_god = p_s;
    pwm_turn_gd = p_t_gd;
    pwm_turn_god = p_t_god;
    cal_gd_gain = gd_g;
    cal_gd_offset = gd_o;
    cal_god_gain = god_g;
    cal_god_offset = god_o;
}
Controls::~Controls()
{
}

void Controls::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();
        writer->String("ref_center");
        writer->Double(ref_center);
        writer->String("pwm");
        writer->StartObject();
            writer->String("straight");
            writer->Int(pwm_stratight_gd);
            writer->String("turn");
                writer->StartObject();
                    writer->String("going_direction");
                    writer->Int(pwm_turn_gd);
                    writer->String("going_opposide_direction");
                    writer->Int(pwm_turn_god);
                writer->EndObject();
        writer->EndObject();
        writer->String("calibration");
        writer->StartObject();
            writer->String("going_direction");
            writer->StartObject();
                writer->String("gain");
                writer->Double(pwm_turn_gd);
                writer->String("offset");
                writer->Double(pwm_turn_god);
            writer->EndObject();
            writer->String("going_opposide_direction");
            writer->StartObject();
                writer->String("gain");
                writer->Double(pwm_turn_gd);
                writer->String("offset");
                writer->Double(pwm_turn_god);
            writer->EndObject();
        writer->EndObject();
    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Model
/////////////////////////////////////////////////////////////////////////////////////////////////
Model::Model()
{
    model_path = MODEL_PATH_DEFAULT;
    target_model_path = TARGET_MODEL_PATH_DEFAULT;
    input_count = ACCELERATOR_INPUT_INPUT_COUNT_DEFAULT;
    output_count = ACCELERATOR_INPUT_OUTPUT_COUNT_DEFAULT;
}

Model::~Model()
{
    auto iter = classes.begin();
    while (iter != classes.end())
    {
        delete (*iter);
        classes.erase(iter++);
    }
}

void Model::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();
    writer->String("model_path");
    writer->String(model_path.c_str(), static_cast<rapidjson::SizeType>(model_path.length()));
    writer->String("target_model_path");
    writer->String(target_model_path.c_str(), static_cast<rapidjson::SizeType>(target_model_path.length()));
    writer->String("input_count");
    writer->Int(input_count);
    writer->String("output_count");
    writer->Int(output_count);
    writer->String("classes");
    writer->StartArray();
    auto iter = classes.begin();
    while (iter != classes.end())
    {
        Classes *classes = *iter;
        classes->Serialize(writer);

        iter++;
    }
    writer->EndArray();
    writer->EndObject();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// Colors
/////////////////////////////////////////////////////////////////////////////////////////////////
Colors::Colors()
{
    name = "";
    memset(&code_rgb, 0, sizeof(code_rgb));
}

Colors::Colors(std::string n, int r, int g, int b)
{
    name = n;
    code_rgb[0] = r;
    code_rgb[1] = g;
    code_rgb[2] = b;
}
Colors::~Colors()
{

}

void Colors::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();

    writer->String("name");
    writer->String(name.c_str(), static_cast<rapidjson::SizeType>(name.length()));
    writer->String("code");
    writer->StartObject();
    writer->String("rgb");
    writer->StartArray();
    writer->Int(code_rgb[0]);
    writer->Int(code_rgb[1]);
    writer->Int(code_rgb[2]);
    writer->EndArray();
    writer->EndObject();
    writer->EndObject();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// Inference
/////////////////////////////////////////////////////////////////////////////////////////////////
Inference::Inference()
{
    model = "yolo";
    max_boxes = INFERNCE_MAX_BOX_COUNT;
    confidence = INFERNCE_MAX_BOX_COUNT;
    image_size[WIDTH] = INFERENCE_IMAGE_DEFAULT_WIDTH;
    image_size[HEIGHT] = INFERENCE_IMAGE_DEFAULT_HEIGHT;
}
Inference::~Inference()
{

    auto iter = colors.begin();
    while (iter != colors.end())
    {
        delete (*iter);
        colors.erase(iter++);
    }
}

void Inference::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();

    writer->String("model");
    writer->String(model.c_str(), static_cast<rapidjson::SizeType>(model.length()));
    writer->String("max_boxes");
    writer->Int(max_boxes);
    writer->String("confidence");
    writer->Double(confidence);
    writer->String("image");
    writer->StartArray();
    writer->Int(image_size[WIDTH]);
    writer->Int(image_size[HEIGHT]);
    writer->EndArray();
    writer->String("colors");
    writer->StartArray();
    auto iter = colors.begin();
    while (iter != colors.end())
    {
        Colors *colors = *iter;
        colors->Serialize(writer);

        iter++;
    }
    writer->EndArray();
    writer->EndObject();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// Accelerator
/////////////////////////////////////////////////////////////////////////////////////////////////
Accelerator::Accelerator()
{
    type = ACCELERATOR_TYPE_HAILO8;
    interface_type = INTERFACE_TYPE_DEFULT;
    interface_name = INTERFACE_NAME_DEFULT;
    ip = INTERFACE_IP_DEFULT;
}
Accelerator::~Accelerator()
{

}

void Accelerator::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();

    writer->String("type");
    writer->String(type.c_str(), static_cast<rapidjson::SizeType>(type.length()));
    writer->String("interface_type");
    writer->String(interface_type.c_str(), static_cast<rapidjson::SizeType>(interface_type.length()));
    writer->String("interface_name");
    writer->String(interface_name.c_str(), static_cast<rapidjson::SizeType>(interface_name.length()));
    writer->String("ip");
    writer->String(ip.c_str(), static_cast<rapidjson::SizeType>(ip.length()));

    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// AI
/////////////////////////////////////////////////////////////////////////////////////////////////
AI::AI()
{

}

AI::~AI()
{

}

void AI::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();

    writer->String("accelerator");
    accelerator.Serialize(writer);
    writer->String("inference");
    inference.Serialize(writer);
    writer->String("model");
    model.Serialize(writer);

    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Car
/////////////////////////////////////////////////////////////////////////////////////////////////
Car::Car()
{
    device_path = "/dev/ttyACM0";
}
Car::~Car()
{

}

void Car::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();
    writer->String("device_path");
    writer->String(device_path.c_str(), static_cast<rapidjson::SizeType>(device_path.length()));
    writer->String("controls");
    controls.Serialize(writer);
    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Extentions
/////////////////////////////////////////////////////////////////////////////////////////////////
Extentions::Extentions()
{
}

Extentions::~Extentions()
{
}

void Extentions::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();

    writer->String("car");
    car.Serialize(writer);

    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Websocket
/////////////////////////////////////////////////////////////////////////////////////////////////
Websocket::Websocket()
{

}

Websocket::~Websocket()
{

}

void Websocket::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
{
    writer->StartObject();

    writer->String("server_port");
    writer->Int(server_port);

    writer->EndObject();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ZaivCfg
/////////////////////////////////////////////////////////////////////////////////////////////////
ZaivCfg::ZaivCfg()
{

}

ZaivCfg::~ZaivCfg()
{

}

bool ZaivCfg::Deserialize(const std::string &s)
{
    rapidjson::Document doc;

    if (!InitDocument(s, doc)) {
        std::cout << "Failed to init document" << std::endl;
        return false;
    }
    std::cout << "Success init document" << std::endl;

    return Deserialize(doc);
}

bool ZaivCfg::Deserialize(const rapidjson::Value &obj)
{

    std::cout << "Start deserialize" << std::endl;

    if (obj.HasMember("ai"))
    {
        const rapidjson::Value &ai = obj["ai"];

        if (ai.HasMember("accelerator"))
        {
            const rapidjson::Value &accelerator = ai["accelerator"];

            if (accelerator.HasMember("type") &&
                accelerator.HasMember("interface_type") &&
                accelerator.HasMember("interface_name") &&
                accelerator.HasMember("ip"))
            {
                m_Ai.accelerator.type = accelerator["type"].GetString();
                m_Ai.accelerator.interface_type = accelerator["interface_type"].GetString();
                m_Ai.accelerator.interface_name = accelerator["interface_name"].GetString();
                m_Ai.accelerator.ip = accelerator["ip"].GetString();
            }
            else
            {
                std::cout << "Failed to get ai/accelerator's member" << std::endl;
                return false;
            }
        } // accelerator
        else
        {
            std::cout << "Failed to get ai/accelerator" << std::endl;
            return false;
        } // accelerator

        if (ai.HasMember("inference"))
        {
            const rapidjson::Value &inference = ai["inference"];

            if (inference.HasMember("model") &&
                inference.HasMember("max_boxes") &&
                inference.HasMember("confidence") &&
                inference.HasMember("image") &&
                inference.HasMember("colors"))
            {
                m_Ai.inference.model = inference["model"].GetString();
                m_Ai.inference.max_boxes = inference["max_boxes"].GetInt();
                m_Ai.inference.confidence = (float)inference["confidence"].GetDouble();

                if (inference["image"].IsArray() && inference["image"].Size() == 2)
                {
                    m_Ai.inference.image_size[WIDTH] = inference["image"][WIDTH].GetInt();
                    m_Ai.inference.image_size[HEIGHT] = inference["image"][HEIGHT].GetInt();
                } // image
                else
                {
                    std::cout << "Failed to get ai/inference/image's member" << std::endl;
                    return false;
                } // image

                if (inference["colors"].IsArray())
                {
                    for (size_t sz = 0; sz < inference["colors"].Size(); sz++)
                    {
                        if (inference["colors"][sz].HasMember("name") == false)
                        {
                            std::cout << "Failed to get ai/inference/color's name member" << std::endl;
                            return false;
                        }
                        if (inference["colors"][sz].HasMember("code"))
                        {
                            if (inference["colors"][sz]["code"].HasMember("rgb") == false)
                            {
                                std::cout << "Failed to get ai/inference/color/code's rgb member" << std::endl;
                                return false;
                            }
                            if (inference["colors"][sz]["code"]["rgb"].IsArray() == false)
                            {
                                std::cout << "ai/inference/color/code's rgb is not array." << std::endl;
                                return false;
                            }
                            if (inference["colors"][sz]["code"]["rgb"].Size() != 3)
                            {
                                std::cout << "ai/inference/color/code's rgb array size is not 3." << std::endl;
                                return false;
                            }
                        }
                        else
                        {
                            std::cout << "Failed to get ai/inference/color's code member" << std::endl;
                            return false;
                        }

                        m_Ai.inference.colors.push_back(new Colors(inference["colors"][sz]["name"].GetString(),
                                                                   inference["colors"][sz]["code"]["rgb"][0].GetInt(),
                                                                   inference["colors"][sz]["code"]["rgb"][1].GetInt(),
                                                                   inference["colors"][sz]["code"]["rgb"][2].GetInt()));
                    }
                } // colors
                else
                {
                    std::cout << "Failed to get ai/inference/colors's member" << std::endl;
                    return false;
                } // colors
            }
            else
            {
                std::cout << "Failed to get ai/inference's member" << std::endl;
                return false;
            }
        } // inference
        else
        {
            std::cout << "Failed to get ai/inference" << std::endl;
            return false;
        } // inference

        if (ai.HasMember("model"))
        {
            const rapidjson::Value &model = ai["model"];

            if (model.HasMember("model_path") &&
                model.HasMember("target_model_path") &&
                model.HasMember("input_count") &&
                model.HasMember("output_count") &&
                model.HasMember("classes"))
            {
                m_Ai.model.model_path = model["model_path"].GetString();
                m_Ai.model.target_model_path = model["target_model_path"].GetString();
                m_Ai.model.input_count = model["input_count"].GetInt();
                m_Ai.model.output_count = model["output_count"].GetInt();
                if (model["classes"].IsArray())
                {
                    for (size_t sz = 0; sz < model["classes"].Size(); sz++)
                    {
                        if ((model["classes"][sz].HasMember("index") == false) ||
                            (model["classes"][sz].HasMember("name") == false) ||
                            (model["classes"][sz].HasMember("show") == false))
                        {
                            std::cout << "Failed to get ai/model/classes's member" << std::endl;
                            return false;
                        }

                        m_Ai.model.classes.push_back(new Classes(model["classes"][sz]["index"].GetInt(),
                                                                 model["classes"][sz]["name"].GetString(),
                                                                 (bool)model["classes"][sz]["show"].GetBool()));
                    }
                }
                else
                {
                    std::cout << "ai/model/classes is not array" << std::endl;
                    return false;
                }
            }
            else
            {
                std::cout << "Failed to get ai/model's member" << std::endl;
                return false;
            }

        } // model
        else
        {
            std::cout << "Failed to get ai/model" << std::endl;
            return false;
        } // model
    }
    else
    {
        std::cout << "Failed to get ai" << std::endl;
        return false;
    }

    if (obj.HasMember("extentions"))
    {
        const rapidjson::Value &extentions = obj["extentions"];

        std::cout << "OK extentions" << std::endl;
        if (extentions.HasMember("car"))
        {
            std::cout << "OK car" << std::endl;
            const rapidjson::Value &car = obj["extentions"]["car"];
            if (car.HasMember("device_path"))
            {
                std::cout << "OK device_path" << std::endl;
                m_extentions.car.device_path = car["device_path"].GetString();
            }

            if (car.HasMember("controls"))
            {
                std::cout << "OK controls" << std::endl;
                const rapidjson::Value &controls = obj["extentions"]["car"]["controls"];
                if (controls.HasMember("ref_center"))
                {
                    m_extentions.car.controls.ref_center = controls["ref_center"].GetInt();
                }

                if (controls.HasMember("pwm"))
                {
                    const rapidjson::Value &pwm = obj["extentions"]["car"]["controls"]["pwm"];

                    if (pwm.HasMember("straight"))
                    {
                        m_extentions.car.controls.pwm_stratight_gd = pwm["straight"].GetInt();
                        m_extentions.car.controls.pwm_stratight_god = pwm["straight"].GetInt();
                    }
                    if (pwm.HasMember("turn"))
                    {
                        const rapidjson::Value &turn = obj["extentions"]["car"]["controls"]["pwm"]["turn"];
                        if (turn.HasMember("going_direction"))
                            m_extentions.car.controls.pwm_turn_gd = turn["going_direction"].GetInt();
                        if (turn.HasMember("going_opposide_direction"))
                            m_extentions.car.controls.pwm_turn_god = turn["going_opposide_direction"].GetInt();
                    }
                }

                if (controls.HasMember("calibraion"))
                {
                    std::cout << "OK calibraion" << std::endl;
                    const rapidjson::Value &calibraion = obj["extentions"]["car"]["controls"]["calibraion"];

                    if (calibraion.HasMember("going_direction"))
                    {
                        const rapidjson::Value &going_direction = obj["extentions"]["car"]["controls"]["calibraion"]["going_direction"];
                        if (going_direction.HasMember("gain"))
                            m_extentions.car.controls.cal_gd_gain = (float)going_direction["gain"].GetDouble();
                        if (going_direction.HasMember("offset"))
                            m_extentions.car.controls.cal_gd_offset = (float)going_direction["offset"].GetDouble();
                    }

                    if (calibraion.HasMember("going_opposide_direction"))
                    {
                        const rapidjson::Value &going_opposide_direction = obj["extentions"]["car"]["controls"]["calibraion"]["going_opposide_direction"];
                        if (going_opposide_direction.HasMember("gain"))
                            m_extentions.car.controls.cal_god_gain = (float)going_opposide_direction["gain"].GetDouble();
                        if (going_opposide_direction.HasMember("offset"))
                            m_extentions.car.controls.cal_god_offset = (float)going_opposide_direction["offset"].GetDouble();
                    }
                }
            }
            else
            {
                std::cout << "Failed to get extentions/car/controls" << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Failed to get extentions/car's member" << std::endl;
            return false;
        }
    }     // car
    else
    {
        std::cout << "Failed to get extentions" << std::endl;
        return false;
    } // extentions

    if (obj.HasMember("websocket"))
    {
        const rapidjson::Value &websocket = obj["websocket"];

        if (websocket.HasMember("server_port"))
        {
            m_wsServer.server_port = websocket["server_port"].GetInt();
        }
        else
        {
            std::cout << "Failed to get websocket's server_port" << std::endl;
            return false;
        } // server_port
    }     // websocket
    else
    {
        std::cout << "Failed to get websocket" << std::endl;
        return false;
    } // websocket

    return true;
}

bool ZaivCfg::Serialize(rapidjson::Writer<rapidjson::StringBuffer> * writer) const
{
    writer->StartObject();

    writer->String("ai");
    m_Ai.Serialize(writer);
    writer->String("websocket");
    m_wsServer.Serialize(writer);

    writer->EndObject();

    return true;
}