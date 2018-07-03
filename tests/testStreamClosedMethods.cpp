/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <gtest/gtest.h>
#include <oboe/Definitions.h>
#include <oboe/Utilities.h>

/**
 * Tests needing to be written:
 *
 * getChannelCount
 * getDirection
getSampleRate
getFramesPerCallback
getFormat
getBufferSizeInFrames
getBufferCapacityInFrames
getSharingMode
getPerformanceMode
getDeviceId
getCallback
getUsage
getContentType
getInputPreset
getSessionId
getState
getXRunCount
getFramesPerBurst
isPlaying
getBytesPerFrame
getBytesPerSample
getFramesWritten
getFramesRead
getTimestamp
getAudioApi
usesAAudio

Stream state control
open
close
start
pause
flush
stop
requestStart
requestPause
requestFlush
requestStop
waitForStateChange

Setters
setBufferSizeInFrames

Helpers
calculateLatencyMillis

I/O
write
read
 */

using namespace oboe;

class StreamClosedReturnValues : public ::testing::Test {



};

TEST_F(StreamClosedReturnValues, GetChannelCountReturnsLastKnownValue){

    FAIL();
}