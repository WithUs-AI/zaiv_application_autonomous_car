/**
* Copyright (c) 2021-2022 Hailo Technologies Ltd. All rights reserved.
* Distributed under the LGPL license (https://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt)
**/
#pragma once
#include "../../hailos/hailo_objects.hpp"
#include "../../hailos/hailo_common.hpp"

__BEGIN_DECLS
void filter(HailoROIPtr roi);
void mobilenet_ssd_withus(HailoROIPtr roi);
void mobilenet_ssd(HailoROIPtr roi);
void mobilenet_ssd_merged(HailoROIPtr roi);
void mobilenet_ssd_visdrone(HailoROIPtr roi);
__END_DECLS