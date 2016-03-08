////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2016 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
////////////////////////////////////////////////////////////////////////////////

#include "ApplicationFeatures/LanguageFeature.h"

#include "Basics/Utf8Helper.h"
#include "Logger/Logger.h"
#include "ProgramOptions2/ProgramOptions.h"
#include "ProgramOptions2/Section.h"

using namespace arangodb;
using namespace arangodb::basics;
using namespace arangodb::options;

LanguageFeature::LanguageFeature(
    application_features::ApplicationServer* server)
    : ApplicationFeature(server, "LanguageFeature") {
  setOptional(false);
  requiresElevatedPrivileges(false);
  startsAfter("LoggerFeature");
}

void LanguageFeature::collectOptions(
    std::shared_ptr<options::ProgramOptions> options) {
  LOG_TOPIC(TRACE, Logger::STARTUP) << name() << "::collectOptions";

  options->addHiddenOption("--default-language", "ISO-639 language code",
                           new StringParameter(&_language));
}

void LanguageFeature::prepare() {
  LOG_TOPIC(TRACE, Logger::STARTUP) << name() << "::prepare";

  if (!Utf8Helper::DefaultUtf8Helper.setCollatorLanguage(_language)) {
    std::string msg =
        "cannot initialize ICU; please make sure ICU*dat is available; "
        "the variable ICU_DATA='";
    if (getenv("ICU_DATA") != nullptr) {
      msg += getenv("ICU_DATA");
    }
    msg += "' should point the directory containing the ICU*dat file.";

    LOG(FATAL) << msg;
    FATAL_ERROR_EXIT();
  }
}
