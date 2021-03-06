/******************************************************************************
 * Copyright 2019 The Apollo Authors. All Rights Reserved.
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

#include "gflags/gflags.h"

#include "cyber/common/file.h"
#include "cyber/common/log.h"
#include "modules/common/util/string_util.h"

#include "modules/data/tools/smart_recorder/record_processor.h"
#include "modules/data/tools/smart_recorder/smart_recorder_gflags.h"

using apollo::common::util::StrCat;
using apollo::cyber::common::GetProtoFromFile;
using apollo::data::RecordProcessor;

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_restored_output_dir.empty()) {
    FLAGS_restored_output_dir = StrCat(FLAGS_source_records_dir, "_restored");
  }
  AINFO << "input dir: " << FLAGS_source_records_dir
        << ". output dir: " << FLAGS_restored_output_dir
        << ". config file: " << FLAGS_smart_recorder_config_filename;
  SmartRecordTrigger trigger_conf;
  CHECK(GetProtoFromFile(FLAGS_smart_recorder_config_filename, &trigger_conf))
      << "Failed to load triggers config file "
      << FLAGS_smart_recorder_config_filename;
  RecordProcessor record_processor(FLAGS_source_records_dir,
                                   FLAGS_restored_output_dir);
  if (!record_processor.Init(trigger_conf)) {
    AERROR << "failed to init record processor";
    return -1;
  }
  if (!record_processor.Process()) {
    AERROR << "failed to process records";
    return -1;
  }
  return 0;
}
