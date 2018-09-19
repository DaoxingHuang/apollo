/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cybertron/class_loader/class_loader.h"
#include "cybertron/component/component.h"
#include "cybertron/cybertron.h"
#include "cybertron/message/raw_message.h"
#include "cybertron/tf2_cybertron/buffer.h"
#include "cybertron/tf2_cybertron/transform_broadcaster.h"

#include "modules/localization/proto/gps.pb.h"
#include "modules/localization/proto/imu.pb.h"
#include "modules/localization/proto/localization.pb.h"
#include "modules/localization/proto/rtk_config.pb.h"
#include "modules/localization/rtk/rtk_localization.h"

namespace apollo {
namespace localization {

class RTKLocalizationComponent final
    : public cybertron::Component<localization::Gps> {
 public:
  RTKLocalizationComponent();
  ~RTKLocalizationComponent() = default;

  bool Init() override;

  bool Proc(const std::shared_ptr<localization::Gps> &gps_msg) override;

  void ImuCallback(const std::shared_ptr<localization::CorrectedImu> &imu_msg);

 private:
  bool InitConfig();
  bool InitIO();

  void PublishPoseBroadcastTF(const LocalizationEstimate &localization);
  void PublishPoseBroadcastTopic(const LocalizationEstimate &localization);

 private:
  std::shared_ptr<cybertron::Reader<localization::CorrectedImu>>
      corrected_imu_listener_ = nullptr;

  std::shared_ptr<cybertron::Writer<LocalizationEstimate>>
      localization_talker_ = nullptr;

  std::string localization_topic_ = "";
  std::string gps_topic_ = "";
  std::string imu_topic_ = "";

  std::string broadcast_tf_frame_id_ = "";
  std::string broadcast_tf_child_frame_id_ = "";
  cybertron::tf2_cybertron::TransformBroadcaster tf2_broadcaster_;

  std::unique_ptr<RTKLocalization> localization_;
};

CYBERTRON_REGISTER_COMPONENT(RTKLocalizationComponent);

}  // namespace localization
}  // namespace apollo